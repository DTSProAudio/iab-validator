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

#ifndef VALIDATEIABSTREAM_H_
#define VALIDATEIABSTREAM_H_

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <set>

#include "IABElementsAPI.h"
#include "IABParserAPI.h"
#include "IABValidatorAPI.h"
#include "JSONNode.h"

#if (__GNUC__)
#include <errno.h>
#endif

#ifdef _WIN32
#include <direct.h>		// for _mkdir
#define SEPARATOR ("\\")
#ifndef S_IRWXU
#define S_IRWXU		0x700	// note that S_IRWXU is not defined in _WIN32, also not used for _mkdir call, include here as a dummy for compilation
#endif
#define MKDIR(fn, mode) _mkdir(fn)
#else
#include <sys/stat.h> // for mkdir
#define SEPARATOR ("/")
#define MKDIR(fn, mode) mkdir(fn, mode)
#endif

#ifdef USE_MDA_NULLPTR
/* support lack of nullptr on older C++ compilers such as g++-4.4.7 */
#include "nullptrOldCompilers.h"
#endif

using namespace SMPTE::ImmersiveAudioBitstream;

/**
 *
 * Exit Status Code
 *
 * Exit Status Code returned by IAB Validator.
 *
 */
enum ExitStatusCode
{
   kIABValidatorSuccessful = 0,     // Validation complete and result is avaliable
    
    // Exit codes below represent incomplete validation
   kIABValidatorParsingIABFrameFromBitStreamFailed = 1,
   kIABValidatorParsedIABFrameFromParserFailed = 2,
   kIABValidatorIABParsedFrameValidationFailed = 3,
   kIABValidatorCannotOpenInputFile = 4,
   kIABValidatorCommandLineUsageError = 5,
   kIABValidatorInstanceCannotBeCreated = 6,
   kIABValidatorSetupFailed = 7,
   kIABValidatorOutputFolderDoesNotExist = 8,
   kIABValidatorOutputFileAlreadyExists = 9,
   kIABValidatorOutputFileCannotBeSetup = 10,
   kIABValidatorIssuesExceeded = 11
};

/**
 *
 * Report Levels
 *
 */
enum ReportLevel
{
    kIABValidatorReportNone    = 0,
    kIABValidatorReportSummary = 1,
    kIABValidatorReportFull    = 2
};

/**
 *
 * Structure stores the occurances of error and warning for particular constraint.
 *
 */
struct ErrorItem
{
    std::string constraint_;
    SupportedConstraintsSet constraintId_;
    iabError errorCode_;
    size_t errorOccurrences_;
    size_t warningOccurrences_;

    ErrorItem()
    {
        constraint_ = "";
        constraintId_ = kConstraints_set_IMF_ST2098_2_2019;
        errorCode_ = kIABNoError;
        errorOccurrences_ = 0;
        warningOccurrences_ = 0;
    }
    
};

/**
 *
 * Linked list to store occurances of errors and warnings.
 *
 */
struct ErrorList
{
    ErrorItem node_;
    ErrorList *next_;
    
    ErrorList()
    {
        next_ = nullptr;
    }
};


// Structure for validation settings
struct ValidationSettings
{
    ValidationSettings()
    {
        validationConstraintSets_.clear();
        inputFileStem_ = "";
        inputFileExt_ = "";
        outputPath_ = "";
        numIssuesToReport_ = 0;
        reportAllIssues_ = true;
        multiFilesInput_ = true;
        generateReport_ = kIABValidatorReportNone;
        showUsage_ = false;
        showCommandExamples_ = false;
        force_write_ = false;
        do_all_ = false;
        output_file_specified_ = false;
    }
    
    std::set<SupportedConstraintsSet> validationConstraintSets_;
    std::string inputFileStem_;             // Stores input file name stem, including relative or full path
    std::string inputFileExt_;              // Stores IAB bitstream file extension
    std::string outputPath_;
    uint32_t numIssuesToReport_;            // indicates number of errors reported in the log file.
    bool reportAllIssues_;                  // In default it reports all errors.
    bool force_write_;                      // overwrites the output if already exists without warning messages.
    bool output_file_specified_;            // indicates user specified output file
    bool multiFilesInput_;                  // indicates IA bitstream is split into multiple files, one IAB frame per file
    ReportLevel generateReport_;            // identifies which level of report to be generated
    bool showUsage_;                        // true to show application usage
    bool showCommandExamples_;              // true to show application command-line examples
    bool do_all_;                           // Validates all constraintsets.
};

class ValidateIABStream
{
    
public:
    
    // Constructor
    ValidateIABStream();
    
    // Destructor
    ~ValidateIABStream();
    
    ExitStatusCode Validate(ValidationSettings& iSettings);
    
    // Writes validation results into report.
    void GenerateValidationReport(ReportLevel iReportLevel);
    
    // Returns the json report
    void GetValidatedResultInJson(JSONNODE** oNode);
    
    // Display bitstream summary and validation summary into console.
    void DisplayValidationSummary();
    
	// Display parser fail state  into console.
	void DisplayParserFailState();

private:
    
    // Checks validation settings passed in by client
    bool AreSettingsValid(ValidationSettings& iSettings);

    // Opens the specified file, creates a stream and assigns pointer to file_
    iabError OpenInputFile(std::string iInputFileName);
    
    // Closes input and output files
    iabError CloseInputOutputFiles();
    
    // Record bitstream info for reporting
    void RecordBitstreamSummary(const IABFrameInterface *iFrameInterface);
    
    // Get Issue ID string for reporting
    std::string GetIssueIDString(int32_t issueId);
    
    // Get constraint set string for reporting
    std::string GetConstraintSetString(SupportedConstraintsSet iValidationConstraintSet);
    
    // Get sample rate string for reporting
    std::string GetSampleRateString(IABSampleRateType iSampleRateCode);

    // Get frame rate string for reporting
    std::string GetFrameRateString(IABFrameRateType iFrameRateCode);

    // Get bit depth string for reporting
    std::string GetBitDepthString(IABBitDepthType iBitDepthCode);
    
    // Temporary method to translate validation error to string for better reporting
    // This will be updated/replaced by final json reporting in PACL-669
    std::string GetValidationErrorString(iabError iErrorCode);
    
    // Temporary method to translate selected parser error to string for better reporting
    // This will be updated/replaced by final json reporting in PACL-669
    std::string GetParserErrorString(iabError iErrorCode);
    
    // returns true/false if number of issues reported so far excceeded expections or not.
    bool DoesNumOfIssuesExceed();

    // Writes a report for the specified constraint set
    bool WriteReportForConstrainSet(SupportedConstraintsSet iValidationConstraintSet, JSONNODE* iParentNode, JSONNODE* iSummaryNode);
    
    // Write bitstream summary into report.
    void ReportBitstreamSummary();
    
    // Finds the constraint with Errorcode existence
    ErrorList* FindListItem(std::string iConstraint, iabError iErrorCode, ErrorList** oLastNode);
    
    // Increments the specific error and warning occurance
    void AddEventToList(SupportedConstraintsSet iConstraintSetId, ValidatorEventKind iEventKind, iabError iErrorCode);
    
    // Adds to the json
    void AddErrorSummaryToReport(SupportedConstraintsSet iConstraintSetId, JSONNODE* iSummaryNode);
    
    // Pointer to IAB parser
    IABParserInterface*     iabParser_;
    
    // Pointer to IAB validator
    IABValidatorInterface*  iabValidator_;
    
    // Input file name stem for constructing input file to open for processing
    std::string             inputFileStem_;
    
    // Input file extension for constructing input file to open for processing
    std::string             inputFileExt_;
    
    // Path to the output folder for report
    std::string             outputPath_;
    
    // Input file type,  multi-file or single-file
    bool                    multiFilesInput_;
    
    // Pointer to the current input file to be processed.
    std::ifstream 			*inputFile_;
    
    // Input frame count to keep track of number of IAB frames processed, also used for progress update
    uint32_t                inputFrameCount_;
    
    // Number of errors appeared in the report.
    uint32_t                numIssuesToReport_;
    bool                    reportAllIssues_;
    
    // ConstraintSets to validate against
    std::set<SupportedConstraintsSet>   validationConstraintSets_;
    
    // Overall validation state, to be reviewed
    bool                    hasInvalidSets_;
    bool                    hasValidationIssues_;
    
    // Bitstream summary
    IABSampleRateType           bitstreamSampleRate_;
    IABFrameRateType            bitstreamFrameRate_;
    IABBitDepthType             bitstreamBitDepth_;
    IABMaxRenderedRangeType     bitstreamMaxRendered_;
  
    // Stores the complete json tree.
    JSONNODE  *jsonTree_;
    JSONNODE  *validationResultSummaryInJson_;
    JSONNODE  *validationIssuesSummaryInJson_;
    JSONNODE  *parserResultInJson_;
    
    // Exit status code
    ExitStatusCode          status_code_;
    
    // Issues error and warnigns linked list
    struct ErrorList        *error_warnings_list_;
    
};

#endif /* VALIDATEIABSTREAM_H_ */
