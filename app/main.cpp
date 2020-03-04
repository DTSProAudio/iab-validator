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

#include "ValidateIABStream.h"
#include <algorithm>
#include "libjson.h"

std::string errorMessages[] = {
    "Validation is complete.",
    "!Error: Parsing of input stream terminated due to critical data error in stream.",
    "!Error: Getting parsed IABFrame from parser failed.",
    "!Error: Validating parsed IABFrame failed.",
    "!Error: Cannot open input file.",
    "!Error: Command line usage error.",
    "!Error: Validator instance cannot be created (could be memory error).",
    "!Error: Validator setup failed.",
    "!Error: Output folder does not exist.",
    "!Error: Output file already exists.",
    "!Error: Output file cannot be setup.",
    "Validation stopped because number of issues exceeded the specified limit."
};

//========================================================================
void DisplayProgramID(void)
{
    std::string version = "1.0.0";
    printf("\n\t****** Xperi ProAudio SMPTE IAB Bitstream Validator Version %s  %s ******\n\n", version.c_str(), __DATE__);
    fflush(stdout);
};

static void ShowUsage(void)
{
    printf("Usage: iab-validator -i<file path> [-c<N> | -cA] [-r1 | -r2] [-s] [-o<file path>] [-w] [-l<N>] [-h] [-ex]\n\n"
           " -i<file path>                  Full or relative path to the file for validation \n\n\n"
           
           "Option Summary:\n\n"
           " -c<N>,  --constraint_set<N>    Constraint Set Number. \n"
           " -cA,    --constraint_set_all   All Constraint Sets. \n"
           " -r1,    --report1              Generate summary report file.\n"
           " -r2,    --report2              Generate detailed report file.\n"
           " -s,     --single-file          Specifies single-file input.\n"
           " -o,     --output<file path>    File path of the output JSON report.\n"
           " -w,     --force_write          Overwrite any existing output report file.\n"
           " -l<N>,  --limit<N>             Teminate validation if number of found issues exceed N.\n"
           " -h,     --help                 Show this application usage information.\n"
           " -ex,    --ShowExamples         Show command line examples for selected validation use cases.\n\n\n"
           
           
           "Validation Options:\n\n"
           " -c<N>,  --constraint_set<N>\n"
           "  Validates and reports against constraint set number N. This option is mutually exclusive with -cA.\n"
           "  Only one -c option is supported.\n"
           "  Constraint Sets   N = 1 : Cinema ST2098-2-2018\n"
           "                    N = 2 : Cinema ST429-18-2019\n"
           "                    N = 3 : DbyCinema\n"
           "                    N = 4 : IMF ST2098-2-2019\n"
           "                    N = 5 : IMF ST2067-201-2019\n"
           "                    N = 6 : DbyIMF\n\n"
           
           " -cA,    --constraint_set_all\n"
           "  Validates and reports against all 6 constraint sets. This option is mutually exclusive with -c<N>.\n"
           "  Default mode when both -c<N> and -cA are absent.\n\n\n"
           
           "Input File Options:\n\n"
           
           " -s,     --single-file\n"
           "  Specifies single-file input. When -s or --single-file is not specified, multi-file input is assumed.\n\n\n"

           "Output Options:\n\n"
           
           " -r1,     --report1\n"
           "  Generates the summary report file.\n\n"
           
           " -r2,     --report2\n"
           "  Generates the detailed report file.\n\n"
           
           " -o,     --output<file path>\n"
           "  Specifies the desired file path of the output report.\n"
           "  (Default filename: <input_file_name>.json).\n\n"
           
           " -w,     --force_write\n"
           "  Enables automatic overwrite of any existing output file.\n"
           "  If not specified, warning is issued on any existing output file. Validation not performed.\n\n"
           
           " -l,     --limit<N> \n"
           "  Validation is terminated when the number of found issues exceed limit N.\n"
           "  If not specified, all issues are logged.\n\n"
           
           " -h,     --help\n"
           "  Displays the full CLI help guide. With -h or --help, other command line parameters\n"
           "  are ignored. This option has priority over -ex and --ShowExamples.\n\n"
           
           " -ex,    --ShowExamples\n"
           "  Show command line examples for selected validation use cases. With -ex or\n"
           "  --ShowExamples, other command line parameters are ignored.\n\n\n"
           
            );
}

void ShowCommandExamples()
{
    printf("This application checks a SMPTE Immersive Audio Bitstream (IAB) against the user specified\n"
           "constraint set(s) and reports on its validity.\n\n"
           "Validation use case examples:\n\n"
           "For all examples, the working directory should contain the following:\n"
           "    a) The validator application: iab-validator-xxxx where xxxx represents the platform suffix\n"
           "    b) An empty folder named REPORT_OUT to save the validation report file, ValidationReport.json\n"
           "    c) A folder named INPUT with the single-file or multi-file bitstream per test case requirement\n\n"
           "Example (1): Command to validate a multi-file bitstream against Cinema ST2098-2-2018 with summary report:\n"
           "    iab-validator-xxx -iINPUT/bitstream_.iab -c1 -r1 -o REPORT_OUT/ValidationReport.json\n\n"
           "    Note omission of frame index in the input file path.\n\n"
           "Example (2): Command to validate single-file bitstream against all 6 constraint sets with summary report:\n"
           "    iab-validator-xxx -iINPUT/bitstream.iab -cA -s -r1 -o REPORT_OUT/ValidationReport.json\n\n"
           "Example (3): Command to validate single-file bitstream against DbyIMF. Display validation summary\n"
           "             to console only without writing any report file:\n"
           "    iab-validator-xxx -iINPUT/bitstream.iab -c6 -s\n\n"
           "======================================================================================================\n\n"
           "======================================================================================================\n\n"
           );
    
}

// Gets the corresponding constraint set enum for the specified -cN index
SupportedConstraintsSet GetConstraintSet(int32_t iConstraintSetID)
{
    switch(iConstraintSetID)
    {
        case 2:
            return kConstraints_set_Cinema_ST429_18_2019;
            break;
            
        case 3:
            return kConstraints_set_DbyCinema;
            break;
            
        case 4:
            return kConstraints_set_IMF_ST2098_2_2019;
            break;
            
        case 5:
            return kConstraints_set_IMF_ST2067_201_2019;
            break;
        case 6:
            return kConstraints_set_DbyIMF;
            break;
            
        case 1:
        default:
            return kConstraints_set_Cinema_ST2098_2_2018;
    }
}

// Collects all constraint sets
void CollectAllConstrainSets(ValidationSettings&  oValidationSettings)
{
    oValidationSettings.validationConstraintSets_.clear();
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_Cinema_ST2098_2_2018);
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_Cinema_ST429_18_2019);
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_DbyCinema);
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_IMF_ST2098_2_2019);
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_IMF_ST2067_201_2019);
    oValidationSettings.validationConstraintSets_.insert(kConstraints_set_DbyIMF);
}

// Processes command line parameter list and sets up the validation settings
bool ExtractCommandlineParams(int argc, char* argv[], ValidationSettings&  oValidationSettings)
{
    std::string tmpString;
    size_t length, iabOffset, atmosOffset;
    int32_t constraintSetID = -1;
    
    for (int i = 1; i < argc; ++i)
    {
        // Check and process help option
        if ((std::string(argv[i]).compare(0,2,"-h") == 0) || (std::string(argv[i]).compare(0,6,"--help") == 0))
        {
            oValidationSettings.showUsage_ = true;
            
            // No need to process other parameters
            return true;
        }
        // Check and process show example option
        else if ((std::string(argv[i]).compare(0,3,"-ex") == 0) || (std::string(argv[i]).compare(0,14,"--ShowExamples") == 0))
        {
            // show examples has priority, ignore all other options
            oValidationSettings.showCommandExamples_ = true;
        }
        // Check and process input path
        else if (std::string(argv[i]).compare(0, 2, "-i") == 0)
        {
            if (oValidationSettings.inputFileStem_.size() > 0)
            {
                std::cerr << "!Error:  More than one -i option specified." << std::endl << std::endl;
                return false;
            }

            oValidationSettings.inputFileStem_ = argv[i];
            oValidationSettings.inputFileStem_.erase(oValidationSettings.inputFileStem_.begin(), oValidationSettings.inputFileStem_.begin()+2);
            
        }
        // Check and process constraint set "all" option
        else if ((std::string(argv[i]).compare(0, 3, "-cA") == 0) || (std::string(argv[i]).compare(0, 20, "--constraint_set_all") == 0))
        {
            // More than one -c option not allowed.
            if (oValidationSettings.validationConstraintSets_.size() != 0)
            {
                std::cerr << "!Error:  More than one -c option specified." << std::endl << std::endl;
                return false;
            }
            oValidationSettings.do_all_ = true;
            CollectAllConstrainSets(oValidationSettings);
        }
        // Check and process single constraint set option
        else if ((std::string(argv[i]).compare(0, 2, "-c") == 0)
                 || (std::string(argv[i]).compare(0, 16, "--constraint_set") == 0))
        {
            // More than one -c option not allowed.
            if (oValidationSettings.validationConstraintSets_.size() != 0)
            {
                std::cerr << "!Error:  More than one -c option specified." << std::endl << std::endl;
                return false;
            }
            
            // Check the constraint set index
            
            std::string cOptionString = argv[i];
            const char* tmp = cOptionString.c_str();
            
            // For -c
            if ((cOptionString.compare(0, 2, "-c") == 0) && (cOptionString.size() == 3))
            {
                constraintSetID = atoi(tmp + 2);
                
            }
            // For --constraint_set
            else if ((cOptionString.compare(0, 16, "--constraint_set") == 0) && (cOptionString.size() == 17))
            {
                constraintSetID = atoi(tmp + 16);
            }
            
            if ((constraintSetID > 0) && (constraintSetID < 7))
            {
                SupportedConstraintsSet oConstraintSet = GetConstraintSet(constraintSetID);
                oValidationSettings.validationConstraintSets_.insert(oConstraintSet);
            }
            else
            {
                std::cerr << "!Error: Invalid index entered with -c or --constraint_set option." << std::endl << std::endl;
                return false;
            }
        }
        // Check and process output path option
        else if ((std::string(argv[i]).compare(0, 2, "-o") == 0) || (std::string(argv[i]).compare(0, 8, "--output") == 0))
        {
            if (oValidationSettings.outputPath_.size() > 0)
            {
                std::cerr << "!Error:  More than one -o option specified." << std::endl << std::endl;
                return false;
            }

            oValidationSettings.outputPath_ = argv[i];
            
            if (std::string(argv[i]).compare(0, 2, "-o") == 0)
            {
                oValidationSettings.outputPath_.erase(oValidationSettings.outputPath_.begin(), oValidationSettings.outputPath_.begin()+2);
            }
            else
            {
                oValidationSettings.outputPath_.erase(oValidationSettings.outputPath_.begin(), oValidationSettings.outputPath_.begin()+8);
            }
                       
            if (oValidationSettings.outputPath_.size() == 0)
            {
                std::cerr << "!Error:  No file path specified with -o option." << std::endl << std::endl;
                return false;
            }
            
            oValidationSettings.output_file_specified_ = true;
        }
        // Check and process force write option
        else if ((std::string(argv[i]).compare(0, 2, "-w") == 0) || (std::string(argv[i]).compare(0, 13, "--force_write") == 0))
        {
            oValidationSettings.force_write_ = true;
        }
        // Check and process -r1 option
        else if ((std::string(argv[i]).compare(0, 3, "-r1") == 0) || (std::string(argv[i]).compare(0, 9, "--report1") == 0))
        {
            if (oValidationSettings.generateReport_ != kIABValidatorReportNone)
            {
                std::cerr << "!Error:  More than one report option specified." << std::endl << std::endl;
                return false;
            }
            oValidationSettings.generateReport_ = kIABValidatorReportSummary;
        }
        // Check and process -r2 option
        else if ((std::string(argv[i]).compare(0, 3, "-r2") == 0) || (std::string(argv[i]).compare(0, 9, "--report2") == 0))
        {
            if (oValidationSettings.generateReport_ != kIABValidatorReportNone)
            {
                std::cerr << "!Error:  More than one report option specified." << std::endl << std::endl;
                return false;
            }
            oValidationSettings.generateReport_ = kIABValidatorReportFull;
        }
        // Check and process single-file, multi-file option
        else if ((std::string(argv[i]).compare(0, 2, "-s") == 0) || (std::string(argv[i]).compare(0, 8, "--single") == 0))
        {
            oValidationSettings.multiFilesInput_ = false;
        }
        else if ((std::string(argv[i]).compare(0, 2, "-l") == 0) || (std::string(argv[i]).compare(0, 7, "--limit") == 0))
        {
            if (oValidationSettings.numIssuesToReport_ != 0)
            {
                std::cerr << "!Error:  More than one limit option specified." << std::endl << std::endl;
                return false;
            }
            
            std::string limitOptionString = argv[i];
            const char* tmp = limitOptionString.c_str();
            
            // Extract number specified with -l
            if ((limitOptionString.compare(0, 2, "-l") == 0) && (limitOptionString.size() > 2))
            {
                if (limitOptionString.find_first_not_of("0123456789",2) != std::string::npos)
                {
                    std::cerr << "!Error:  Invalid number used with -l option." << std::endl << std::endl;
                    return false;
                }
                
                oValidationSettings.numIssuesToReport_ = atoi(tmp + 2);
            }
            // Extract number specified with --limit
            else if ((limitOptionString.compare(0, 7, "--limit") == 0) && (limitOptionString.size() > 7))
            {
                if (limitOptionString.find_first_not_of("0123456789",7) != std::string::npos)
                {
                    std::cerr << "!Error:  Invalid number used with --limit option." << std::endl << std::endl;
                    return false;
                }

                oValidationSettings.numIssuesToReport_ = atoi(tmp + 7);
            }
            
            // Validate number
            if (oValidationSettings.numIssuesToReport_ > 0)
            {
                oValidationSettings.reportAllIssues_ = false;
            }
            else
            {
                std::cerr << "!Error:  Invalid number used with -l or --limit option." << std::endl << std::endl;
                return false;
            }
        }
        // Command line parameter not matching any option
        else
        {
            std::cerr << "!Error: Unknown command line parameter : " << argv[i] << std::endl << std::endl;
            return false;
        }
    }
    
    if (oValidationSettings.showCommandExamples_)
    {
        // Don't check other parameters
        return true;
    }
        
    if (oValidationSettings.validationConstraintSets_.size() == 0)
    {
        // if no constraintset is added, then all constraint sets are validated.
        CollectAllConstrainSets(oValidationSettings);
    }
    
    if (oValidationSettings.inputFileStem_.size() == 0)
    {
        std::cerr << "!Error: Input file path is not specified." << std::endl << std::endl;
        return false;
    }
    
    tmpString = oValidationSettings.inputFileStem_;
    length = tmpString.size();
    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
    
    iabOffset = tmpString.find(".iab");
    atmosOffset = tmpString.find(".atmos");
    
    // Check if inputFileStem_ ends with .iab or .atmos
    // Offset is the character position of the dot, i.e "."
    if ((length < 5) ||
        ((iabOffset != (length - 4)) && (atmosOffset != (length - 6))))
    {
        std::cerr << "!Error: Invalid input file path or file extension." << std::endl;
        return false;
    }
    
    // Save file extension and name stem (without extension)
    if (iabOffset == (length - 4))
    {
        oValidationSettings.inputFileExt_ = ".iab";
        oValidationSettings.inputFileStem_.erase(oValidationSettings.inputFileStem_.begin() + iabOffset, oValidationSettings.inputFileStem_.end());
    }
    else
    {
        oValidationSettings.inputFileExt_ = ".atmos";
        oValidationSettings.inputFileStem_.erase(oValidationSettings.inputFileStem_.begin() + atmosOffset, oValidationSettings.inputFileStem_.end());
    }

    return true;
}

int main(int argc, char* argv[])
{
    ExitStatusCode statusCode = kIABValidatorSuccessful;
    
    ValidationSettings validationSettings;
    
    // ****************************
    // Display application information
    // ****************************
     DisplayProgramID();
    
    if (argc < 2)
    {
        ShowUsage();
        return kIABValidatorCommandLineUsageError;
    }
    
    // ****************************
    // Process command line options
    // ****************************
    
    if (!ExtractCommandlineParams(argc, argv, validationSettings))
    {
        std::cerr << std::endl << "Use 'iab-validator -h' for help info on command usage." << std::endl;
        return kIABValidatorCommandLineUsageError;
    }

    if (validationSettings.showUsage_)
    {
        ShowUsage();
        return kIABValidatorSuccessful;
    }

    //ShowCommandExamples
    if (validationSettings.showCommandExamples_)
    {
        ShowCommandExamples();
        return kIABValidatorSuccessful;
    }
    
    // ****************************
    // Report validation results
    // ****************************
    
    std::string sep = "/";
    
#ifdef _WIN32
    sep = "\\";
#endif

    std::ofstream *oFile = nullptr;
    
    if (validationSettings.generateReport_ != kIABValidatorReportNone)
    {
        // Output file is not given by the user, then form the output filename from input filename and its folder.
        if (validationSettings.output_file_specified_ == false)
        {
            std::cerr << "Note: No -o specified. Default output filename will be used and it will be saved in the current input file directory." << std::endl << std::endl;
            std::string outputFolder = "";
            std::string outputFile = "";
            std::string outputFilename = "";
            size_t indexOfRightMostSeperator = outputFile.rfind(sep, outputFile.length());

            outputFile = validationSettings.inputFileStem_;

            // Creates the output filename from the input filename.
            if (indexOfRightMostSeperator != std::string::npos) {
                outputFolder = (validationSettings.inputFileStem_.substr(0, indexOfRightMostSeperator)) + sep;
                outputFilename = (validationSettings.inputFileStem_.substr(indexOfRightMostSeperator+1, validationSettings.inputFileStem_.size()));
            } else {
                outputFilename = (validationSettings.inputFileStem_.substr(0, validationSettings.inputFileStem_.size()));
            }
            outputFile = outputFolder + outputFilename + ".json";
            validationSettings.outputPath_ = outputFile;
        }
        
        // Output file is specified by the user, then check for its existence.
        if (validationSettings.output_file_specified_ && !validationSettings.force_write_)
        {
            std::ifstream *file = new std::ifstream(validationSettings.outputPath_.c_str(), std::ifstream::in);
            if (file->good())
            {
                std::cerr << "Output file already exists." << std::endl << std::endl;
                std::cerr << "Program ended with exit code: " << kIABValidatorOutputFileAlreadyExists << std::endl;
                file->close();
                delete file;
                return kIABValidatorOutputFileAlreadyExists;
            }
        }
       
        // opens the output file to write.
        oFile = new std::ofstream(validationSettings.outputPath_.c_str(), std::ofstream::out);
        
        if (!oFile->good())
        {
            std::cerr << "Output file cannot be created. Check path, folder permission(s), etc." << std::endl << std::endl;
            std::cerr << "Program ended with exit code: " << kIABValidatorOutputFileCannotBeSetup << std::endl;
            return kIABValidatorOutputFileCannotBeSetup;
        }
    }
    
    // Instantiate bitstream validator
    ValidateIABStream *bitstreamValidator = new ValidateIABStream();
    
    if (!bitstreamValidator)
    {
        std::cerr << "!Error: Unable to create an IAB Validator instance to process the bitstream." << std::endl;
        std::cerr << "This could be due to a memory error. Restart the computor and try again." << std::endl << std::endl;
        std::cerr << "Program ended with exit code: " << kIABValidatorInstanceCannotBeCreated << std::endl;
        statusCode = kIABValidatorInstanceCannotBeCreated;
    }
    else
    {
        statusCode = bitstreamValidator->Validate(validationSettings);
        
        // ****************************
        // Generate output
        // Clean up and exit
        // ****************************
        
        if (validationSettings.generateReport_ != kIABValidatorReportNone)
        {
            if (statusCode == kIABValidatorSuccessful || statusCode == kIABValidatorIssuesExceeded)
            {
                std::cerr << errorMessages[statusCode] << " Check results in report file." << std::endl << std::endl;
                bitstreamValidator->GenerateValidationReport(validationSettings.generateReport_);
            }
			else if (statusCode == kIABValidatorParsingIABFrameFromBitStreamFailed)
			{
                std::cerr <<  errorMessages[statusCode] << " Check results in report file."<< std::endl << std::endl;
                bitstreamValidator->GenerateValidationReport(validationSettings.generateReport_);
                std::cerr << "" << std::endl << std::endl;
            }
            else
            {
                std::cerr << "The tool exits early and validation is incomplete. " << std::endl << std::endl << errorMessages[statusCode] << std::endl << std::endl;
            }
       
            if (oFile && oFile->good())
            {
                JSONNODE* nodeTree = nullptr;
                bitstreamValidator->GetValidatedResultInJson(&nodeTree);
                
                // Writes into file.
                json_char* jc = json_write_formatted(nodeTree);
                {
                    std::string str = jc;
                    size_t length = str.length();
                    oFile->write(str.c_str(), length);
                }
                
                json_free(jc);
            }
        }
        else
        {
            if (statusCode == kIABValidatorSuccessful || statusCode == kIABValidatorIssuesExceeded)
            {
                bitstreamValidator->GenerateValidationReport(validationSettings.generateReport_);
                bitstreamValidator->DisplayValidationSummary();
                std::cerr << errorMessages[statusCode] << std::endl << std::endl;
            }
			else if (statusCode == kIABValidatorParsingIABFrameFromBitStreamFailed)
			{
                std::cerr << errorMessages[statusCode] << std::endl << std::endl;
				bitstreamValidator->DisplayParserFailState();
			}
			else
            {
                std::cerr << "The tool exits early and validation is incomplete. "<< std::endl << std::endl << errorMessages[statusCode] << std::endl << std::endl;
            }
        }
    }
        
     // clean up.
    delete bitstreamValidator;
    
    if (oFile && oFile->good())
    {
        oFile->close();
        delete oFile;
    }
    
    // In console window, the exit code wont be displayed in default.
    // So it has to be printed explicitly.
    std::cerr << "Program ended with exit code: " << statusCode << std::endl;

    return statusCode;
    
}
