/* Copyright (c) 2020 Xperi Corporation (and its subsidiaries). All rights reserved.
 
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ValidateIABStream.h"
#include "libjson.h"

#ifdef _LOG
#define LOG_ERR(x) (std::cerr << (x))
#define LOG_OUT(x) (std::cout << (x))
#else
#define LOG_ERR(x)
#define LOG_OUT(x)
#endif

std::string intToString(int32_t value)
{
    char ch[32];
    sprintf(ch, "%d", value);
    return std::string(ch);
}

// Constructor
ValidateIABStream::ValidateIABStream()
{
    iabParser_ = nullptr;
    iabValidator_ = nullptr;
    inputFile_ = nullptr;
    
    hasInvalidSets_ = false;
    hasValidationIssues_ = false;
	inputFrameCount_ = 0;
    numIssuesToReport_ = 0;
    reportAllIssues_ = true;
    jsonTree_ = json_new(JSON_NODE);
    validationResultSummaryInJson_ = nullptr;
    validationIssuesSummaryInJson_ = nullptr;
    parserResultInJson_ = nullptr;
    status_code_ = kIABValidatorSuccessful;
    error_warnings_list_ = nullptr;
    
    // Create an IAB Validator instance to validate bitstream
    iabValidator_ = IABValidatorInterface::Create();
    
}

// Destructor
ValidateIABStream::~ValidateIABStream()
{
    if (iabParser_)
    {
        IABParserInterface::Delete(iabParser_);
    }
    
    if (iabValidator_)
    {
        IABValidatorInterface::Delete(iabValidator_);
    }
    
    if (jsonTree_)
    {
        json_delete(jsonTree_);
    }
    
    // Delete error and warnings list.
    struct ErrorList *current = error_warnings_list_;
    struct ErrorList *next;
    
    while (current != nullptr)
    {
        next = current->next_;
        delete(current);
        current = next;
    }
    
    CloseInputOutputFiles();
    
}

// ValidateIABStream::AreSettingsValid() implementation
bool ValidateIABStream::AreSettingsValid(ValidationSettings& iSettings)
{
    if (iSettings.inputFileStem_.empty() ||
        iSettings.inputFileExt_.empty() ||
        iSettings.validationConstraintSets_.empty())
    {
        return false;
    }
    
    // Save settings to class internal members
    inputFileStem_ = iSettings.inputFileStem_;
    inputFileExt_ = iSettings.inputFileExt_;
    outputPath_ = iSettings.outputPath_;
    multiFilesInput_ = iSettings.multiFilesInput_;
    validationConstraintSets_ = iSettings.validationConstraintSets_;
    reportAllIssues_ = iSettings.reportAllIssues_;
    numIssuesToReport_ = iSettings.numIssuesToReport_;

    return true;
}

// ValidateIABStream::OpenInputFile() implementation
iabError ValidateIABStream::OpenInputFile(std::string iInputFileName)
{
    inputFile_ = new std::ifstream(iInputFileName.c_str(), std::ifstream::in | std::ifstream::binary);
    
    if (!inputFile_->good())
    {
        return kIABGeneralError;
    }
    
    return kIABNoError;
}

// ValidateIABStream::CloseInputOutputFiles() implementation
iabError ValidateIABStream::CloseInputOutputFiles()
{
    // Close files
    if (inputFile_)
    {
        inputFile_->close();
        delete inputFile_;
        inputFile_ = nullptr;
    }
    
    return kIABNoError;
}

// ValidateIABStream::Validate() implementation
ExitStatusCode ValidateIABStream::Validate(ValidationSettings& iSettings)
{
    // Check settings and save if valid
    if (!AreSettingsValid(iSettings))
    {
        // Invalid, return error
        return kIABValidatorSetupFailed;
    }
    
    if (!iabValidator_)
    {
        // Error : Validator not instantiated
        return kIABValidatorInstanceCannotBeCreated;
    }

    bool noProcessingError = true;
    iabError returnCode = kIABNoError;

    if (multiFilesInput_)
    {
        LOG_ERR("Processing bitstream frame sequence. This could take several minutes for complex or long bitstreams ........\n");
        
        while (1)
        {
            // Construct next input file name
            std::stringstream ss;
            ss << inputFileStem_.c_str() << std::setfill('0') << std::setw(6) << inputFrameCount_ << inputFileExt_;
            
            // open input file for processing. Break loop if there is no more input.
            if (kIABNoError != OpenInputFile(ss.str()))
            {
                if (inputFrameCount_ == 0)
                {
                    LOG_ERR("!Error in opening file : " + ss.str() + ". Input file name error or missing input file).\n");
                    noProcessingError = false;
                    status_code_ = kIABValidatorCannotOpenInputFile;
                }
                
                break;
            }
            
            // Use a new IABParser instance for each new file. If one exists, delete it
            if (iabParser_)
            {
                IABParserInterface::Delete(iabParser_);
                iabParser_ = nullptr;
            }
            
            // Create an IAB Parser instance to process current file
            iabParser_ = IABParserInterface::Create(inputFile_);
            
            // Parse the bitstream into IAB frame
            returnCode = iabParser_->ParseIABFrame();
            
            if (kIABNoError != returnCode)
            {
                if ((inputFrameCount_ == 0) || (returnCode != kIABParserEndOfStreamReached))
                {
                    LOG_ERR("The application has encountered an error when parsing a frame from the bitstream.\n");
                    LOG_ERR("Error code: " + intToString(returnCode) + GetParserErrorString(returnCode));
                    parserResultInJson_ =  json_new(JSON_NODE);
                    json_set_name(parserResultInJson_, "ParserState");
                    json_push_back(parserResultInJson_, json_new_a("Error", GetParserErrorString(returnCode).c_str()));
                    json_push_back(parserResultInJson_, json_new_i("FrameIndex", inputFrameCount_ + 1));
                    noProcessingError = false;
                    status_code_ = kIABValidatorParsingIABFrameFromBitStreamFailed;
                }
                
                break;
            }
            
            if (inputFile_)
            {
                // For multi-files input, finish with current input file, close it
                inputFile_->close();
                delete inputFile_;
                inputFile_ = nullptr;
            }
            
            const IABFrameInterface *frameInterface = nullptr;
            
            // Get the parsed IAB frame from IABParser
            if (kIABNoError != iabParser_->GetIABFrame(frameInterface) || frameInterface == nullptr)
            {
                LOG_ERR("The application is unable to get the parsed IAB frame from the parser.\n");
                noProcessingError = false;
                status_code_ = kIABValidatorParsedIABFrameFromParserFailed;
                break;
            }
            
            if (inputFrameCount_ > 0)
            {
                // Check and update maxRendered if necessary
                IABMaxRenderedRangeType maxRendered = 0;
                frameInterface->GetMaxRendered(maxRendered);
                if (maxRendered > bitstreamMaxRendered_)
                {
                    bitstreamMaxRendered_ = maxRendered;
                }
            }
            else
            {
                // First frame, record bitstream summary for reporting
                // Note that these items should be static for a valid bitstream and leave it to the validator to pick up any issues.
                RecordBitstreamSummary(frameInterface);
            }
            
            // Validate the parsed frame. The validator will keep tracks of validation state, warnings and errors
            // These will be checked when validation completes or aborted
            returnCode = iabValidator_->ValidateIABFrame(frameInterface, inputFrameCount_);
            if (kIABNoError != returnCode)
            {
                // Temporary reporting, parser error reporting will be finalised in PACL-669
                LOG_ERR("The application has encountered an error when validating a parsed IAB frame.\n");
                LOG_ERR("Error code: " + intToString(returnCode));
                noProcessingError = false;
                status_code_ = kIABValidatorIABParsedFrameValidationFailed;
                break;
            }
            
            inputFrameCount_++;
            
            if ((reportAllIssues_ == false) && (DoesNumOfIssuesExceed() == true))
            {
                status_code_ = kIABValidatorIssuesExceeded;
                noProcessingError = false;
                break;
            }
            
            // Display progress every 50 frames
            if ((inputFrameCount_ % 50) == 0)
            {
                std::cout << "Frames processed: " << inputFrameCount_ << std::endl << std::flush;
            }
        }
    }
    else    // single-file input
    {
        LOG_OUT("Processing input file : " + inputFileStem_ + inputFileExt_ + ". This could take several minutes for complex or long bitstreams ........\n" );
        
        while (1)
        {
            if (inputFrameCount_ > 0)
            {
                if (inputFile_->eof())
                {
                    // Finished processing
                    inputFile_->close();
                    delete inputFile_;
                    inputFile_ = nullptr;
                    break;
                }
            }
            else
            {
                // First frame, open the input file for the Parser to use
                std::string inputFile = inputFileStem_ + inputFileExt_;
                if (kIABNoError != OpenInputFile(inputFile))
                {
                    LOG_ERR( "!Error in opening file : " +inputFile + ". Input file name error or missing input file.\n");
                    noProcessingError = false;
                    status_code_ = kIABValidatorCannotOpenInputFile;
                    break;
                }
            }
            
            if (inputFrameCount_ == 0)
            {
                if (iabParser_)
                {
                    IABParserInterface::Delete(iabParser_);
                    iabParser_ = nullptr;
                }
                
                // Create an IAB Parser instance to process current file
                iabParser_ = IABParserInterface::Create(inputFile_);
            }
            
            // Parse the bitstream into IAB frame
            
            returnCode = iabParser_->ParseIABFrame();
            // TODO Process error code to provide more info
            
            if (kIABNoError != returnCode)
            {
                if ((inputFrameCount_ == 0) || (returnCode != kIABParserEndOfStreamReached))
                {
                    // Temporary reporting, parser error reporting will be finalised in PACL-669
                    LOG_ERR("The application has encountered an error when parsing a frame from the bitstream.\n");
                    LOG_ERR("Error code: " + intToString(returnCode) + GetParserErrorString(returnCode) + "\n");
                    parserResultInJson_ =  json_new(JSON_NODE);
                    json_set_name(parserResultInJson_, "ParserState");
                    json_push_back(parserResultInJson_, json_new_a("Error", GetParserErrorString(returnCode).c_str()));
                    json_push_back(parserResultInJson_, json_new_i("FrameIndex", inputFrameCount_ + 1));
                    noProcessingError = false;
                    status_code_ = kIABValidatorParsingIABFrameFromBitStreamFailed;
                }
                
                break;
            }
            
            const IABFrameInterface *frameInterface = nullptr;
            
            if (kIABNoError != iabParser_->GetIABFrame(frameInterface) || frameInterface == nullptr)
            {
                LOG_ERR("The application is unable to get the parsed IAB frame from the parser.\n");
                noProcessingError = false;
                status_code_ = kIABValidatorParsedIABFrameFromParserFailed;
                break;
            }
            
            if (inputFrameCount_ > 0)
            {
                // Check and update maxRendered if necessary
                IABMaxRenderedRangeType maxRendered = 0;
                frameInterface->GetMaxRendered(maxRendered);
                if (maxRendered > bitstreamMaxRendered_)
                {
                    bitstreamMaxRendered_ = maxRendered;
                }
            }
            else
            {
                // First frame, record bitstream summary for reporting
                RecordBitstreamSummary(frameInterface);
            }
            
            // Validate the parsed frame. The validator will keep tracks of validation state, warnings and errors
            // These will be checked when validation completes or aborted
            returnCode = iabValidator_->ValidateIABFrame(frameInterface, inputFrameCount_);
            if (kIABNoError != returnCode)
            {
                // Temporary reporting, parser error reporting will be finalised in PACL-669
                LOG_ERR("The application has encountered an error when validating a parsed IAB frame.\n");
                LOG_ERR("Error code: " + intToString(returnCode) + "\n");
                noProcessingError = false;
                status_code_ = kIABValidatorIABParsedFrameValidationFailed;
                break;
            }

            inputFrameCount_++;
            
            if ((reportAllIssues_ == false) && (DoesNumOfIssuesExceed() == true))
            {
                status_code_ = kIABValidatorIssuesExceeded;
                noProcessingError = false;
                break;
            }
            
            // Display progress every 50 frames
            if ((inputFrameCount_ % 50) == 0)
            {
                LOG_OUT("Frames processed: " + intToString(inputFrameCount_) + "\n");
            }
        }
    }
    
    LOG_OUT ( "Total frames processed: " + intToString(inputFrameCount_) + "\n\n");
    
    if (noProcessingError)
    {
        return kIABValidatorSuccessful;
    }
    else
    {
        return status_code_;
    }
}

// ValidateIABStream::RecordBitstreamSummary() implementation
void ValidateIABStream::RecordBitstreamSummary(const IABFrameInterface *iFrameInterface)
{
    iFrameInterface->GetSampleRate(bitstreamSampleRate_);
    iFrameInterface->GetFrameRate(bitstreamFrameRate_);
    iFrameInterface->GetBitDepth(bitstreamBitDepth_);
    iFrameInterface->GetMaxRendered(bitstreamMaxRendered_);
    
}

// Display the validation report on the console
void ValidateIABStream::DisplayValidationSummary()
{
    std::cout << "Bitstream Summary Information:" << std::endl;
    std::cout << "\tSampleRate:               " <<  GetSampleRateString(bitstreamSampleRate_) << std::endl;
    std::cout << "\tFrameRate:                " << GetFrameRateString(bitstreamFrameRate_) << std::endl;
    std::cout << "\tBitDepth:                 " << GetBitDepthString(bitstreamBitDepth_) << std::endl;
    std::cout << "\tFrameCount:               " << inputFrameCount_ << std::endl;
    std::cout << "\tMaxRenderedInStream:      " << bitstreamMaxRendered_ << std::endl << std::endl;
    
    if (validationResultSummaryInJson_)
    {
        std::cout << "Validation Summary Information:\n" << json_write_formatted(validationResultSummaryInJson_) << std::endl << std::endl;
    }
    
    if (validationIssuesSummaryInJson_)
    {
        std::cout << "Issues Summary Information:\n" << json_write_formatted(validationIssuesSummaryInJson_) << std::endl << std::endl;
    }
}

// Display parser fail state  on console.
void ValidateIABStream::DisplayParserFailState()
{
	if (parserResultInJson_)
	{
		std::cout << json_write_formatted(parserResultInJson_) << std::endl;
		std::cout << "\n====================\n\n";
	}
}

void ValidateIABStream::ReportBitstreamSummary()
{
    JSONNODE * summaryNode = json_new(JSON_NODE);
    json_set_name(summaryNode, "BitStreamSummary");
    json_push_back(summaryNode, json_new_a("SampleRate", GetSampleRateString(bitstreamSampleRate_).c_str()));
    json_push_back(summaryNode, json_new_a("FrameRate", GetFrameRateString(bitstreamFrameRate_).c_str()));
    json_push_back(summaryNode, json_new_a("BitDepth", GetBitDepthString(bitstreamBitDepth_).c_str()));
    
    // validation stopped, hence total number of frames yet to be known
    if (status_code_ == kIABValidatorIssuesExceeded)
    {
        json_push_back(summaryNode, json_new_a("FrameCount", "?"));
        json_push_back(summaryNode, json_new_a("MaxRendered", "?"));
    }
    else
    {
        json_push_back(summaryNode, json_new_a("FrameCount", intToString(inputFrameCount_).c_str()));
        json_push_back(summaryNode, json_new_a("MaxRendered", intToString(bitstreamMaxRendered_).c_str()));
    }

    json_push_back(jsonTree_, summaryNode);
}

// Returns the json report
void ValidateIABStream::GetValidatedResultInJson(JSONNODE** oNode)
{
    *oNode = jsonTree_;
}

// Check for number of issues found so far
bool ValidateIABStream::DoesNumOfIssuesExceed()
{
    // if number of issues are said to be displayed restricted, after each frame validation check for number of issues
    // captured so far.
    if (reportAllIssues_ == false)
    {
        std::set<SupportedConstraintsSet>::iterator iterCS;
        for (iterCS = validationConstraintSets_.begin(); iterCS != validationConstraintSets_.end(); iterCS++)
        {
            std::vector<ValidationIssue> validationIssues;
            std::vector<ValidationIssue>::iterator iterIssues;

            validationIssues = iabValidator_->GetValidationIssues(*iterCS);
            
            if (validationIssues.size() >= numIssuesToReport_)
            {
                return true;
            }
        }
    }
    
    return false;
}

// ValidateIABStream::WriteValidationReport() implementation
void ValidateIABStream::GenerateValidationReport(ReportLevel iReportLevel)
{
    // No parser error.
    if (status_code_ != kIABValidatorParsingIABFrameFromBitStreamFailed)
    {
        // Report Summary
        ReportBitstreamSummary();
        
        LOG_OUT("Validation result:\n\n");
        
        JSONNODE * validationResultsInJson = json_new(JSON_ARRAY);
        json_set_name(validationResultsInJson, "ValidationResult");
        
        validationResultSummaryInJson_ = json_new(JSON_ARRAY);
        json_set_name(validationResultSummaryInJson_, "ValidationResultSummary");
        
        validationIssuesSummaryInJson_ = json_new(JSON_ARRAY);
        json_set_name(validationIssuesSummaryInJson_, "IssueOccurrenceSummary");
        
        // Generate report for each constraint set specified in validation settings
        std::set<SupportedConstraintsSet>::iterator iterCS;
        
        for (iterCS = validationConstraintSets_.begin(); iterCS != validationConstraintSets_.end(); iterCS++)
        {
            JSONNODE * validationResultInJson = json_new(JSON_NODE);
            WriteReportForConstrainSet(*iterCS, validationResultInJson, validationResultSummaryInJson_);
            json_push_back(validationResultsInJson, validationResultInJson);
            
            // Get the constriant set string for reporting
            std::string constraintString;
            constraintString = GetConstraintSetString(*iterCS);
            AddErrorSummaryToReport(*iterCS, validationIssuesSummaryInJson_);
        }
   
        if (hasValidationIssues_)
        {
            LOG_OUT( "\nNote that due to the validation constraint hierrarchy design, issues from the base set(s) will be reported\n");
            LOG_OUT( "before the requested constraint set. For example since ST-429-18-2019 is a super set of ST-2098-2, a bitstream\n");
            LOG_OUT( "item could be reported as a warning issue for ST-2098-2 and additionally reported as an error issue for ST-429-18-2019.\n\n");
        }
        json_push_back(jsonTree_, validationResultSummaryInJson_);
        json_push_back(jsonTree_, validationIssuesSummaryInJson_);
        
        if (kIABValidatorReportFull == iReportLevel)
        {
            json_push_back(jsonTree_, validationResultsInJson);
        }
    }
    else
    {
        json_push_back(jsonTree_, parserResultInJson_);
    }
}

// WriteReportForConstrainSet functionality summary
// 1. Gets the issueList for the constraintSet
// 2. Generates the issues summary in Json structure (i.e indicates the valid, invalid or warning)
// 3. Generates Issue summary Linked list (Group the errors and warnings together for constraint set
// 4. Stores the issue in the JSON structure.
// ValidateIABStream::WriteReportForConstrainSet() implementation
bool ValidateIABStream::WriteReportForConstrainSet(SupportedConstraintsSet iValidationConstraintSet, JSONNODE* validationResultInJson, JSONNODE* iSummaryNode)
{
    std::string constraintString;
    ValidationResult validationResult;
    
    // Get the constriant set string for reporting
    constraintString = GetConstraintSetString(iValidationConstraintSet);
    
    // Get validation result
    validationResult = iabValidator_->GetValidationResult(iValidationConstraintSet);
    
    json_push_back(validationResultInJson, json_new_a("Constraint", constraintString.c_str()));
    
    JSONNODE * validationResultSummary = json_new(JSON_NODE);
    json_push_back(validationResultSummary, json_new_a("Constraint", constraintString.c_str()));
    
    // 1. Gets the issueList for the constraintSet

    // combined issues list is needed to find out the origin of the issue.
    std::vector<ValidationIssue> combinedIssuesList = iabValidator_->GetValidationIssues(iValidationConstraintSet);
    std::string issueOrigin = "";
    if (combinedIssuesList.size())
    {
        std::vector<ValidationIssue>::iterator iter = combinedIssuesList.begin();
        issueOrigin = GetConstraintSetString(iter->isBeingValidated_);
    }
    
    std::vector<ValidationIssue> validationIssues;
    std::vector<ValidationIssue>::iterator iterIssues;
    
    // Get only constraint set interested in. For all constraints validaiton (-cA), we dont want to report the errors repeatedly.
    // For single constraint validation, we shall report all errors in the hierarchy.
    if (validationConstraintSets_.size() > 1)
    {
        validationIssues = iabValidator_->GetValidationIssuesSingleSetOnly(iValidationConstraintSet);
    }
    else
    {
        validationIssues = combinedIssuesList;
    }
    
    // 2. Generates the issues summary in Json structure (i.e indicates the valid, invalid or warning)
    // 3. Generates Issue summary Linked list (Group the errors and warnings together for constraint set
    if (validationResult == kValid)
    {
        json_push_back(validationResultInJson, json_new_a("ValidationState", "Valid"));
        json_push_back(validationResultSummary, json_new_a("ValidationState", "Valid"));
        // Bitstream is valid against this constraint set, no additional information to report
        LOG_OUT( "Input stream complies with " + constraintString +"\n\n");
        
        json_push_back(validationResultSummary, json_new_i("NumIssues", combinedIssuesList.size()));
    }
    else
    {
        std::string issueType = "";
        if (validationResult == kValidWithWarning)
        {
            issueType = "ValidWithWarning";
            LOG_OUT( "Input stream complies with " + constraintString + " but with warnings, see additional information below:\n" );
        }
        else
        {
            hasInvalidSets_ = true;
            issueType = "Invalid";
            LOG_OUT( "Input stream does not comply with " + constraintString + ", see additional information below:\n");
        }
        json_push_back(validationResultInJson, json_new_a("ValidationState", issueType.c_str())); // added to validation results
        
        // added to summary
        json_push_back(validationResultSummary, json_new_a("ValidationState", issueType.c_str()));
        json_push_back(validationResultSummary, json_new_a("IssueRootConstraintSet", issueOrigin.c_str()));
        
        // The below logic is for classifying the NumIssues for each constraint set.
        if (iValidationConstraintSet == kConstraints_set_Cinema_ST2098_2_2018)
        {
            json_push_back(validationResultSummary, json_new_i("NumIssues", validationIssues.size()));
        }
        else if (iValidationConstraintSet == kConstraints_set_Cinema_ST429_18_2019)
        {
            std::string classification = "";
            const std::vector<ValidationIssue> set_1 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_Cinema_ST2098_2_2018);
            const std::vector<ValidationIssue> set_2 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_Cinema_ST429_18_2019);

            if (set_1.size())
            {
                classification = "ST2098-2-2018(" + intToString(set_1.size()) + ") + ";
            }
            if (classification.length())
            {
                classification += "ST429-18-2019("  + intToString(set_2.size()) + ")";
                classification = intToString(combinedIssuesList.size()) +  " (= " + classification + ")";
                json_push_back(validationResultSummary, json_new_a("NumIssues", classification.c_str()));
            }
            else
            {
                json_push_back(validationResultSummary, json_new_i("NumIssues", set_2.size()));
            }
        }
        else if (iValidationConstraintSet == kConstraints_set_DbyCinema)
        {
            std::string classification = "";
            const std::vector<ValidationIssue> set_1 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_Cinema_ST2098_2_2018);
            const std::vector<ValidationIssue> set_2 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_Cinema_ST429_18_2019);
            const std::vector<ValidationIssue> set_3 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_DbyCinema);

            if (set_1.size())
            {
                classification = "ST2098-2-2018(" + intToString(set_1.size()) + ") + ";
            }
            if (set_2.size())
            {
                 classification += "ST429-18-2019(" + intToString(set_2.size()) + ") + ";
            }
            if (classification.length())
            {
                classification += "DbyCinema("  + intToString(set_3.size()) + ")";
                classification = intToString(combinedIssuesList.size()) +  " (= " + classification + ")";
                json_push_back(validationResultSummary, json_new_a("NumIssues", classification.c_str()));
            }
            else
            {
                json_push_back(validationResultSummary, json_new_i("NumIssues", set_3.size()));
            }
    
        }
        
        /// IMF Constraint sets
        // The following logic will group together errors and warnings depending upon the constraint set.
        if (iValidationConstraintSet == kConstraints_set_IMF_ST2098_2_2019)
        {
            json_push_back(validationResultSummary, json_new_i("NumIssues", validationIssues.size()));
        }
        else if (iValidationConstraintSet == kConstraints_set_IMF_ST2067_201_2019)
        {
            std::string classification = "";
            const std::vector<ValidationIssue> set_1 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_IMF_ST2098_2_2019);
            const std::vector<ValidationIssue> set_2 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_IMF_ST2067_201_2019);

            if (set_1.size())
            {
                classification = "IMF ST2098-2-2019(" + intToString(set_1.size()) + ") + ";
            }
            if (classification.length())
            {
                classification += "IMF ST2067-201-2019("  + intToString(set_2.size()) + ")";
                classification = intToString(combinedIssuesList.size()) +  " (= " + classification + ")";
                json_push_back(validationResultSummary, json_new_a("NumIssues", classification.c_str()));
            }
            else
            {
                json_push_back(validationResultSummary, json_new_i("NumIssues", set_2.size()));
            }
        }
        else if (iValidationConstraintSet == kConstraints_set_DbyIMF)
        {
            std::string classification = "";
            const std::vector<ValidationIssue> set_1 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_IMF_ST2098_2_2019);
            const std::vector<ValidationIssue> set_2 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_IMF_ST2067_201_2019);
            const std::vector<ValidationIssue> set_3 = iabValidator_->GetValidationIssuesSingleSetOnly(kConstraints_set_DbyIMF);

            if (set_1.size())
            {
                classification = "IMF ST2098-2-2019(" + intToString(set_1.size()) + ") + ";
            }
            if (set_2.size())
            {
                classification += "IMF ST2067-201-2019(" + intToString(set_2.size()) + ") + ";
            }
            if (classification.length())
            {
                classification += "DbyIMF("  + intToString(set_3.size()) + ")";
                classification = intToString(combinedIssuesList.size()) + " (= " + classification + ")";
                json_push_back(validationResultSummary, json_new_a("NumIssues", classification.c_str()));
            }
            else
            {
                json_push_back(validationResultSummary, json_new_i("NumIssues", set_3.size()));
            }
        }
        
        // 4. Stores the issues in the JSON structure.

        SupportedConstraintsSet lastReportingConstraintSet = iValidationConstraintSet;
        std::string errorString;
        
        if (validationIssues.size() > 0)
        {
            hasValidationIssues_ = true;
            
            json_push_back(validationResultInJson, json_new_i("IssuesReported", validationIssues.size()));

            JSONNODE * IssuesJson = json_new(JSON_ARRAY);
            json_set_name(IssuesJson, "ReportedIssues");

            for (iterIssues = validationIssues.begin(); iterIssues != validationIssues.end(); iterIssues++)
            {
                JSONNODE * IssueJson = json_new(JSON_NODE);
                json_push_back(IssueJson, json_new_i("IssueNum", iterIssues - validationIssues.begin()+1));

                // Check the reporting constraint set and display once per group
                if ((iterIssues == validationIssues.begin()) || (lastReportingConstraintSet != iterIssues->isBeingValidated_))
                {
                    lastReportingConstraintSet = iterIssues->isBeingValidated_;
                    constraintString = GetConstraintSetString(lastReportingConstraintSet);
                    LOG_ERR("\n\tIssues found when validating against " + constraintString + ":\n" );
                }

                switch(iterIssues->event_)
                {
                    case ErrorEvent:
                        LOG_ERR( "\t\t- Error event found at frame #" + intToString(iterIssues->frameIndex_) + ", ID=" + intToString(iterIssues->id_));
                        if (kIABNoError != iterIssues->errorCode_)
                        {
                            LOG_ERR( ", ErrorCode=" + intToString(iterIssues->errorCode_) + GetValidationErrorString(iterIssues->errorCode_) +"\n");
                            json_push_back(IssueJson, json_new_a("EventType", "Error"));
                        }
                        else
                        {
                            LOG_OUT("\n");
                        }
                        
                        break;
                        
                    case WarningEvent:
                        LOG_ERR( "\t\t- Warning event found at frame #" + intToString(iterIssues->frameIndex_) + ", ID=" + intToString(iterIssues->id_));
                        if (kIABNoError != iterIssues->errorCode_)
                        {
                            LOG_ERR( ", ErrorCode=" + intToString(iterIssues->errorCode_) + GetValidationErrorString(iterIssues->errorCode_) + "\n");
                            json_push_back(IssueJson, json_new_a("EventType", "Warning"));
                        }
                        else
                        {
                            LOG_OUT("\n");
                        }
                        
                        break;
                        
                    default:
                        break;
                }
                
                json_push_back(IssueJson, json_new_i("FrameIndex", iterIssues->frameIndex_));
                json_push_back(IssueJson, json_new_a("MetaID", GetIssueIDString(iterIssues->id_).c_str()));
                json_push_back(IssueJson, json_new_a("ErrorText", GetValidationErrorString(iterIssues->errorCode_).c_str()));
                json_push_back(IssueJson, json_new_a("Constraint", GetConstraintSetString(iterIssues->isBeingValidated_).c_str()));
                
                AddEventToList(lastReportingConstraintSet, iterIssues->event_, iterIssues->errorCode_);

                json_push_back(IssuesJson, IssueJson);
            }
            
            json_push_back(validationResultInJson, IssuesJson);
            
            LOG_OUT("\n");
        }
    }
    
    json_push_back(iSummaryNode, validationResultSummary);

    return true;
}

// ValidateIABStream::GetIssueIDString() implementation
std::string ValidateIABStream::GetIssueIDString(int32_t issueId)
{
    switch(issueId)
    {
        case kIssueID_IAFrame:
            return "IABFrame";
            
        case kIssueID_AuthoringToolInfo:
            return "AuthoringTool Info";
            
        case kIssueID_UserData:
            return "UserData";
            
        case kIssueID_ObjectZoneDefinition19:
            return "ObjectZoneDefinition19";
            
        default:
            return intToString(issueId);
    }
}


// ValidateIABStream::GetConstraintSetString() implementation
std::string ValidateIABStream::GetConstraintSetString(SupportedConstraintsSet iValidationConstraintSet)
{
    switch(iValidationConstraintSet)
    {
        case kConstraints_set_Cinema_ST2098_2_2018:
            return "Cinema 2098-2-2018 constraint";

        case kConstraints_set_Cinema_ST429_18_2019:
            return "Cinema ST429-18-2019 constraint";

        case kConstraints_set_DbyCinema:
            return "DbyCinema constraint";

        case kConstraints_set_IMF_ST2098_2_2019:
            return "IMF ST2098-2-2019 constraint";

        case kConstraints_set_IMF_ST2067_201_2019:
            return "IMF ST2067-201-2019 constraint";

        case kConstraints_set_DbyIMF:
            return "DbyIMF constraint";
            
        default:
            return "";
    }
}

// ValidateIABStream::GetSampleRateString() implementation
std::string ValidateIABStream::GetSampleRateString(IABSampleRateType iSampleRateCode)
{
    if (iSampleRateCode == kIABSampleRate_48000Hz)
    {
        return "48kHz";
    }
    else if (iSampleRateCode == kIABSampleRate_96000Hz)
    {
        return "96kHz";
    }
    else
    {
        return "unknown";
    }
}

// ValidateIABStream::GetFrameRateString() implementation
std::string ValidateIABStream::GetFrameRateString(IABFrameRateType iFrameRateCode)
{
    switch(iFrameRateCode)
    {
        case kIABFrameRate_24FPS:
            return "24 fps";
            
        case kIABFrameRate_25FPS:
            return "25 fps";
            
        case kIABFrameRate_30FPS:
            return "30 fps";
            
        case kIABFrameRate_48FPS:
            return "48 fps";
            
        case kIABFrameRate_50FPS:
            return "50 fps";
            
        case kIABFrameRate_60FPS:
            return "60 fps";
            
        case kIABFrameRate_96FPS:
            return "96 fps";
            
        case kIABFrameRate_100FPS:
            return "100 fps";
            
        case kIABFrameRate_120FPS:
            return "120 fps";
            
        case kIABFrameRate_23_976FPS:
            return "23.976 fps";
            
        default:
            return "unknown";
    }
}

// ValidateIABStream::GetBitDepthString() implementation
std::string ValidateIABStream::GetBitDepthString(IABBitDepthType iBitDepthCode)
{
    if (iBitDepthCode == kIABBitDepth_24Bit)
    {
        return "24-bit";
    }
    else if (iBitDepthCode == kIABBitDepth_16Bit)
    {
        return "16-bit";
    }
    else
    {
        return "unknown";
    }
}

// ValidateIABStream::GetValidationErrorString() implementation
std::string ValidateIABStream::GetParserErrorString(iabError iErrorCode)
{
    std::string errorString = "!Parsing terminated: ";
    
    // Not expecting any non-validation error code but just for completeness
    if (iErrorCode < kValidateGeneralError)
    {
        switch (iErrorCode)
        {
            case kIABParserInvalidVersionNumberError:
                errorString += "Invalid IAB Version number";
                break;
                
            case kIABParserInvalidSampleRateError:
                errorString += "Invalid IAB Sample Rate";
                break;
                
            case kIABParserInvalidFrameRateError:
                errorString += "Invalid IAB Frame Rate";
                break;
                
            case kIABParserInvalidBitDepthError:
                errorString += "Invalid IAB Bit Depth";
                break;

            case kIABParserMissingPreambleError:
                errorString += "IAB Preamble sub-frame is missing from bitstream";
                break;
                
            default:
				errorString += "Input stream contains possible data corruption. Unable to continue parsing.";
				break;
        }
    }
    
    return errorString;
}

// ValidateIABStream::GetValidationErrorString() implementation
std::string ValidateIABStream::GetValidationErrorString(iabError iErrorCode)
{
    std::string errorString;
    
    // Map validation error code to error string
    switch (iErrorCode)
    {
        case kValidateGeneralError:
            errorString = "General validation error";
            break;

        case kValidateErrorIAFrameIllegalBitstreamVersion:
            errorString = "Illegal version number";
            break;

        case kValidateErrorIAFrameUnsupportedSampleRate:
            errorString = "Unsupported sample rate";
            break;
            
        case kValidateErrorIAFrameUnsupportedBitDepth:
            errorString = "Unsupported bit depth";
            break;
            
        case kValidateErrorIAFrameUnsupportedFrameRate:
            errorString = "Unsupported frame rate";
            break;

        case kValidateErrorIAFrameMaxRenderedExceeded:
            errorString = "IAFrame MaxRendered limit exceeded";
            break;
        case kValidateWarningIAFrameMaxRenderedNotMatchObjectNumbers:
            errorString = "IAFrame MaxRendered not matching number of bed channels and objects";
            break;
        case kValidateErrorIAFrameSubElementCountConflict:
            errorString = "Frame sub-element count in conflict with sub-element list";
            break;
            
        case kValidateErrorIAFrameBitstreamVersionNotPersistent:
            errorString = "Bitstream version number not persistent";
            break;
            
        case kValidateErrorIAFrameSampleRateNotPersistent:
            errorString = "Sample rate not persistent";
            break;
            
        case kValidateErrorIAFrameBitDepthNotPersistent:
            errorString = "Bit depth not persistent";
            break;
            
        case kValidateErrorIAFrameFrameRateNotPersistent:
            errorString = "Frame rate not persistent";
            break;
            
        case kValidateErrorIAFrameUndefinedElementType:
            errorString = "Undefined element found in IABFrame";
            break;
            
        case kValidateErrorIAFrameSizeLimitExceeded:
            errorString = "IABFrame size limit exceeded";
            break;
            
        case kValidateErrorBedDefinitionDuplicateMetaID:
            errorString = "Duplicate BedDefinition meta ID in frame";
            break;
            
        case kValidateErrorBedDefinitionMultiActiveSubElements:
            errorString = "BedDefinition has multiple active sub-elements";
            break;
            
        case kValidateErrorBedDefinitionHierarchyLevelExceeded:
            errorString = "BedDefinition hierarchy level exceeded";
            break;

        case kValidateErrorBedDefinitionChannelCountConflict:
            errorString = "BedDefinition channel count in conflict with channel list";
            break;
            
        case kValidateErrorBedDefinitionDuplicateChannelID:
            errorString = "BedDefinition contains duplicate channel ID";
            break;
            
        case kValidateErrorBedDefinitionUnsupportedGainPrefix:
            errorString = "BedDefinition contains unsupported gain prefix";
            break;
            
        case kValidateErrorBedDefinitionUnsupportedDecorPrefix:
            errorString = "BedDefinition contains unsupported decorrelation prefix";
            break;
            
        case kValidateErrorBedDefinitionAudioDescriptionTextExceeded:
            errorString = "BedDefinition audio description text exceeded length limit";
            break;
            
        case kValidateErrorBedDefinitionSubElementCountConflict:
            errorString = "BedDefinition sub-element count in conflict with sub-element list";
            break;
            
        case kValidateErrorBedDefinitionInvalidChannelID:
            errorString = "BedDefinition contains invalid or reserved channel ID";
            break;
            
        case kValidateErrorBedDefinitionInvalidUseCase:
            errorString = "BedDefinition contains invalid or reserved use case";
            break;
            
        case kValidateErrorBedDefinitionSubElementsNotAllowed:
            errorString = "BedDefinition cannot have sub-element";
            break;
            
        case kValidateErrorBedDefinitionCountNotPersistent:
            errorString = "BedDefinition count changes over program frames";
            break;
            
        case kValidateErrorBedDefinitionChannelCountNotPersistent:
            errorString = "BedDefinition channel count changes over program frames";
            break;
            
        case kValidateErrorBedDefinitionMetaIDNotPersistent:
            errorString = "BedDefinition meta ID changes over program frames";
            break;
            
        case kValidateErrorBedDefinitionChannelIDsNotPersistent:
            errorString = "BedDefinition channel IDs change over program frames";
            break;
            
        case kValidateErrorBedDefinitionConditionalStateNotPersistent:
            errorString = "BedDefinition conditional flag or use case changes over program frames";
            break;
        
        case kValidateErrorBedRemapDuplicateMetaID:
            errorString = "Duplicate BedRemap meta ID";
            break;
            
        case kValidateErrorBedRemapSourceChannelCountNotEqualToBed:
            errorString = "BedRemap source channel count not equal to parent bed";
            break;
            
        case kValidateErrorBedRemapSubblockCountConflict:
            errorString = "BedRemap subblock count conflict";
            break;
            
        case kValidateErrorBedRemapSourceChannelCountConflict:
            errorString = "BedRemap source channel count conflict";
            break;
            
        case kValidateErrorBedRemapDestinationChannelCountConflict:
            errorString = "BedRemap destination channel count conflict";
            break;
            
        case kValidateErrorBedRemapInvalidDestChannelID:
            errorString = "BedRemap contains invalid or reserved destination channel ID";
            break;
            
        case kValidateErrorBedRemapInvalidUseCase:
            errorString = "BedRemap contains invalid or reserved use case";
            break;
            
        case kValidateErrorBedRemapNotAnAllowedSubElement:
            errorString = "BedRemap is not an allowed sub-element";
            break;
            
        case kValidateErrorObjectDefinitionDuplicateMetaID:
            errorString = "Duplicate ObjectDefinition meta ID in frame";
            break;
            
        case kValidateErrorObjectDefinitionMultiActiveSubElements:
            errorString = "ObjectDefinition has multiple active sub-elements";
            break;
            
        case kValidateErrorObjectDefinitionHierarchyLevelExceeded:
            errorString = "ObjectDefinition hierarchy level exceeded";
            break;
            
        case kValidateErrorObjectDefinitionPanSubblockCountConflict:
            errorString = "ObjectDefinition panblock count conflict";
            break;
            
        case kValidateErrorObjectDefinitionUnsupportedGainPrefix:
            errorString = "ObjectDefinition contains unsupported gain prefix";
            break;
            
        case kValidateErrorObjectDefinitionUnsupportedZoneGainPrefix:
            errorString = "ObjectDefinition contains unsupported zone gain prefix";
            break;
            
        case kValidateErrorObjectDefinitionUnsupportedSpreadMode:
            errorString = "ObjectDefinition contains unsupported spread mode";
            break;
            
        case kValidateErrorObjectDefinitionUnsupportedDecorPrefix:
            errorString = "ObjectDefinition contains unsupported decorrelation prefix";
            break;
            
        case kValidateErrorObjectDefinitionAudioDescriptionTextExceeded:
            errorString = "ObjectDefinition audio description text exceeded length limit";
            break;
            
        case kValidateErrorObjectDefinitionSubElementCountConflict:
            errorString = "ObjectDefinition sub-element count in conflict with sub-element list";
            break;
            
        case kValidateErrorObjectDefinitionInvalidUseCase:
            errorString = "ObjectDefinition contains invalid or reserved use case";
            break;
            
        case kValidateErrorObjectDefinitionInvalidSubElementType:
            errorString = "ObjectDefinition contains invalid sub-element type";
            break;
            
        case kValidateErrorObjectDefinitionConditionalStateNotPersistent:
            errorString = "ObjectDefinition conditional flag or use case changes over program frames";
            break;
            
        case kValidateErrorObjectZoneDefinition19SubblockCountConflict:
            errorString = "Object zone19 subblock count conflict";
            break;
            
        case kValidateErrorObjectZoneDefinition19UnsupportedZoneGainPrefix:
            errorString = "Object zone19 contains unsupported zone gain prefix";
            break;
            
        case kValidateErrorAudioDataDLCAudioDataIDZero:
            errorString = "AudioDataDLC AudioDataID cannot be zero";
            break;
            
        case kValidateErrorAudioDataDLCDuplicateAudioDataID:
            errorString = "Duplicate AudioDataDLC duplicate ID";
            break;
            
        case kValidateErrorAudioDataDLCUnsupportedSampleRate:
            errorString = "AudioDataDLC has unsupported sample rate";
            break;
            
        case kValidateErrorAudioDataDLCSampleRateConflict:
            errorString = "AudioDataDLC sample rate in conflict with frame sample rate";
            break;
            
        case kValidateErrorAudioDataDLCNotAnAllowedSubElement:
            errorString = "AudioDataDLC is not allowed";
            break;
            
        case kValidateErrorAudioDataPCMAudioDataIDZero:
            errorString = "AudioDataPCM AudioDataID cannot be zero";
            break;
            
        case kValidateErrorAudioDataPCMDuplicateAudioDataID:
            errorString = "Duplicate AudioDataDPCM duplicate ID";
            break;
            
        case kValidateErrorAudioDataPCMNotAnAllowedSubElement:
            errorString = "AudioDataPCM is not allowed";
            break;
            
        case kValidateErrorMissingAudioDataEssenceElement:
            errorString = "A referenced audio essence element (DLC or PCM) is missing";
            break;
            
        case kValidateErrorUserDataNotAnAllowedSubElement:
            errorString = "UserData is not allowed";
            break;
            
        case kValidateErrorDLCUsedWithIncompatibleFrameRate:
            errorString = "AudioDataDLC cannot be used with the current frame rate";
            break;
            
        case kValidateErrorDolCinIAFrameUnsupportedSampleRate:
            errorString = "Sample rate not valid for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionSubElementsNotAllowed:
            errorString = "BedDefinition cannot have sub-element for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionInvalidChannelID:
            errorString = "BedDefinition contains invalid channel ID for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionInvalidUseCase:
            errorString = "BedDefinition contains invalid use case for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionMultipleBedsNotAllowed:
            errorString = "Multiple BedDefinitions are not allowed for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionInvalidGainPrefix:
            errorString = "BedDefinition contains invalid gain prefix for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionChannelDecorInfoExistNotZero:
            errorString = "BedDefinition channel DecorInfoExist must = 0 for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionMaxChannelCountExceeded:
            errorString = "BedDefinition channel count limit exceeded for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionCountNotPersistent:
            errorString = "BedDefinition count must be persistent for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionMetaIDNotPersistent:
            errorString = "BedDefinition meta ID must be persistent for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedDefinitionChannelListNotPersistent:
            errorString = "BedDefinition channel list must be persistent for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedRemapUnsupportedGainPrefix:
            errorString = "BedRemap contains unsupported gain prefix for DbyCinema";
            break;
            
        case kValidateErrorDolCinBedRemapNotAnAllowedSubElement:
            errorString = "BedRemap is not allowed for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionSubElementsNotAllowed:
            errorString = "ObjectDefinition cannot have sub-element for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionInvalidUseCase:
            errorString = "ObjectDefinition contains invalid use case for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionInvalidGainPrefix:
            errorString = "ObjectDefinition contains invalid gain prefix for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionInvalidZoneGainPrefix:
            errorString = "ObjectDefinition contains invalid zone gain prefix for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionInvalidSpreadMode:
            errorString = "ObjectDefinition contains invalid spread mode for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionInvalidDecorPrefix:
            errorString = "ObjectDefinition contains invalid decorrelation prefix for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionSnapTolExistsNotZero:
            errorString = "ObjectDefinition SnapTolExist must = 0 for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionMaxObjectCountExceeded:
            errorString = "ObjectDefinition count limit exceeded for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectDefinitionNonSequenctialMetaID:
            errorString = "ObjectDefinition meta ID must be sequential for DbyCinema";
            break;
            
        case kValidateErrorDolCinObjectZoneDefinition19NotAnAllowedSubElement:
            errorString = "Object zone19 is not allowed for DbyCinema";
            break;
            
        case kValidateErrorDolCinAuthoringToolInfoNotAnAllowedSubElement:
            errorString = "AuthoringToolInfo is not allowed for DbyCinema";
            break;
            
        case kValidateGeneralWarning:
            errorString = "Validation general warning";
            break;
            
        case kValidateWarningFrameContainFrame:
            errorString = "IABFrame contains an IABFrame as sub-element";
            break;
            
        case kValidateWarningFrameContainBedRemap:
            errorString = "IABFrame contains an BedRemap as sub-element";
            break;
            
        case kValidateWarningFrameContainObjectZoneDefinition19:
            errorString = "IABFrame contains an Object zone19 as sub-element";
            break;
            
        case kValidateWarningFrameContainUndefinedSubElement:
            errorString = "IABFrame contains an undefined sub-element";
            break;
            
        case kValidateWarningAuthoringToolInfoMultipleElements:
            errorString = "IABFrame contains multiple AuthoringToolInfo sub-elements";
            break;
            
        case kValidateWarningBedDefinitionUndefinedUseCase:
            errorString = "BedDefinition contains undefined or reserved use case";
            break;
            
        case kValidateWarningBedDefinitionUndefinedChannelID:
            errorString = "BedDefinition contains undefined or reserved channel ID";
            break;
            
        case kValidateWarningBedDefinitionUndefinedAudioDescription:
            errorString = "BedDefinition contains undefined audio description";
            break;
            
        case kValidateWarningBedDefinitionContainUnsupportedSubElement:
            errorString = "BedDefinition contains unsupported sub-element";
            break;
            
        case kValidateWarningBedDefinitionAlwaysActiveSubElement:
            errorString = "BedDefinition contains unsupported sub-element";
            break;
            
        case kValidateWarningBedRemapUndefinedUseCase:
            errorString = "BedRemap contains undefined or reserved use case";
            break;
            
        case kValidateWarningBedRemapUndefinedChannelID:
            errorString = "BedRemap contains undefined or reserved channel ID";
            break;
            
        case kValidateWarningObjectDefinitionUndefinedUseCase:
            errorString = "ObjectDefinition contains undefined or reserved use case";
            break;
            
        case kValidateWarningObjectDefinitionUndefinedAudioDescription:
            errorString = "ObjectDefinition contains undefined audio description";
            break;
            
        case kValidateWarningObjectDefinitionMultipleZone19SubElements:
            errorString = "ObjectDefinition contains multiple zone19 sub-elements";
            break;
            
        case kValidateWarningObjectDefinitionContainUnsupportedSubElement:
            errorString = "ObjectDefinition contains unsupported sub-elements";
            break;
            
        case kValidateWarningObjectDefinitionAlwaysActiveSubElement:
            errorString = "ObjectDefinition contains an always active sub-element";
            break;
            
        case kValidateWarningUnreferencedAudioDataDLCElement:
            errorString = "AudioDataDLC element not referenced by any BedDefinition or ObjectDefinition";
            break;
            
        case kValidateWarningUnreferencedAudioDataPCMElement:
            errorString = "AudioDataPCM element not referenced by any BedDefinition or ObjectDefinition";
            break;
            
        case kValidateErrorDolCinObjectDefinitionZoneGainsNotAPreset:
            errorString = "ObjectDefinition zone gain is not a DbyCinema preset";
            break;
            
		case kValidateErrorDolIMFBedDefinitionInvalidChannelID:
			errorString = "BedDefinition contains invalid channel ID for DbyIMF";
			break;

		case kValidateErrorDolIMFBedDefinitionInvalidGainPrefix:
			errorString = "BedDefinition contains invalid gain prefix for DbyIMF";
			break;

		case kValidateErrorDolIMFBedDefinitionChannelDecorInfoExistNotZero:
			errorString = "BedDefinition channel DecorInfoExist must = 0 for DbyIMF";
			break;

		case kValidateErrorDolIMFObjectDefinitionInvalidGainPrefix:
			errorString = "ObjectDefinition contains invalid gain prefix for DbyIMF";
			break;

		case kValidateErrorDolIMFObjectDefinitionInvalidZoneGainPrefix:
			errorString = "ObjectDefinition contains invalid zone gain prefix for DbyIMF";
			break;

		case kValidateErrorDolIMFObjectDefinitionInvalidSpreadMode:
			errorString = "ObjectDefinition contains invalid spread mode for DbyIMF";
			break;

		case kValidateErrorDolIMFObjectDefinitionInvalidDecorPrefix:
			errorString = "ObjectDefinition contains invalid decorrelation prefix for DbyIMF";
			break;

		case kValidateErrorDolIMFObjectDefinitionSnapTolExistsNotZero:
			errorString = "ObjectDefinition SnapTolExist must = 0 for DbyIMF";
			break;

		case kValidateErrorDolIMFNotMeetingContinuousAudioSequence:
			errorString = "Frame sub-element order not meeting continuous audio sequence for DbyIMF";
			break;

		case kValidateErrorDolIMFContinuousAudioSequenceNotPersistent:
			errorString = "Frame sub-element continuous audio sequence not persistent for DbyIMF";
			break;

		case kValidateWarningDolIMFObjectDefinitionZoneGainsNotAPreset:
			errorString = "ObjectDefinition zone gain is not a DbyIMF preset";
			break;

		default:
            errorString = "Unknown err";
    }
    
    return errorString;
}


// Error code Map and List functions

// returns the item from the list
ErrorList* ValidateIABStream::FindListItem(std::string iConstraint, iabError iErrorCode, ErrorList** oLastNode)
{
    ErrorList *element = error_warnings_list_;
    
    while (element != nullptr)
    {
        *oLastNode = element;

        if (element->node_.constraint_ == iConstraint && element->node_.errorCode_ == iErrorCode)
        {
            return element;
        }
		else
		{
            element = element->next_;
            
        }
    }

    return element;
}


// Adds the summary to json
void ValidateIABStream::AddErrorSummaryToReport(SupportedConstraintsSet iConstraintSetId, JSONNODE* iSummaryNode)
{
    ErrorList *element = error_warnings_list_;
    
    std::string constraintName = GetConstraintSetString(iConstraintSetId);
    
    JSONNODE * errorSummary = json_new(JSON_NODE);
    json_push_back(errorSummary, json_new_a("Constraint", constraintName.c_str()));
    
    JSONNODE * errors = json_new(JSON_NODE);
    json_set_name(errors, "Errors");
    
    JSONNODE * warnings = json_new(JSON_NODE);
    json_set_name(warnings, "Warnings");

    while (element != nullptr)
    {
        // For single constraint validation, group all the errors and warnings.
        // For all constraints validation, display errors and warnings on its own constraint.
        if ((element->node_.constraintId_ == iConstraintSetId) || (validationConstraintSets_.size() == 1))
        {
            if (element->node_.errorOccurrences_)
            {
                json_push_back(errors, json_new_i(GetValidationErrorString(element->node_.errorCode_).c_str(), element->node_.errorOccurrences_));
            }
            if (element->node_.warningOccurrences_)
            {
                json_push_back(warnings, json_new_i(GetValidationErrorString(element->node_.errorCode_).c_str(), element->node_.warningOccurrences_));
            }
        }
        
        element = element->next_;
    }
    
    if (json_size(errors))
    {
        json_push_back(errorSummary, errors);
    }
    else
    {
        json_delete(errors);
        json_push_back(errorSummary, json_new_i("Errors", 0));
    }
    
    if (json_size(warnings))
    {
        json_push_back(errorSummary, warnings);
    }
    else
    {
        json_delete(warnings);
        json_push_back(errorSummary, json_new_i("Warnings", 0));
    }
    
    json_push_back(iSummaryNode, errorSummary);
}

// Increments the error code occurance.
void ValidateIABStream::AddEventToList(SupportedConstraintsSet iConstraintSetId, ValidatorEventKind iEventKind, iabError iErrorCode)
{
    ErrorList *lastNode = nullptr;
    std::string constraintString = GetConstraintSetString(iConstraintSetId);
    ErrorList *element = FindListItem(constraintString, iErrorCode, &lastNode);

    if (!element)
	{
        element = new ErrorList();
        element->node_.constraint_ = constraintString;
        element->node_.constraintId_ = iConstraintSetId;
        element->node_.errorCode_ = iErrorCode;
        element->next_ = nullptr;

        if (error_warnings_list_ == nullptr)
        {
            error_warnings_list_ = element;
        }
        
        if (lastNode)
        {
            lastNode->next_ = element;
        }
    }
    
    if (iEventKind == WarningEvent)
    {
        element->node_.warningOccurrences_++;
    }
    else if (iEventKind == ErrorEvent)
    {
        element->node_.errorOccurrences_++;
    }
}

