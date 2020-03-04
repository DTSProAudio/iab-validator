# Compile and Build IAB-Validator Libraries and Application using CMake

This file contains instructions for compiling and building binaries using CMake.

A "CMakeLists.txt" file can be found in the root folder to build the IAB-Validator libraries and application.
To build from source, a C++ compiler must be installed and configured on your platform.
The following instructions describe how to build these from source files using CMake.

## Step 1. Build prerequisites

  a) Obtain and install the latest version of CMake for your platform from https://cmake.org. *(Minimum version required is CMake v3.1.0.)*
  
  b) Clone a local copy from GitHub IAB-Validator repository
  
  c) In the root folder of cloned local repo, i.e. at same level as "app", create a sub-folder "thirdparty"

  d) Obtain libjson 7.6.1 from https://sourceforge.net/projects/libjson/files/libjson_7.6.1.zip/download

  e) Unzip and copy the "libjson" folder into "thirdparty".

  f) On Windows, if building with Visual Studio 2015, comment out line number 324 in JSONOptions.h file, i.e.
  
        // #define JSON_DEPRECATED_FUNCTIONS

## Step 2. Configure CMake

Note: Typically, the working build folder "TestBuildDir" referenced below, is created by users (e.g. using "*mkdir*")
  
To configure CMake, open a terminal window and change to the root folder of cloned local repo, then execute one of the following cmake 
commands that is appropriate to your platform:

### Linux

    cmake . -BTestBuildDir -DARCH=64 -DCMAKE_BUILD_TYPE=Release

The above step should generate all necessary CMake configuration files in TestBuildDir that can be used to build the 
dependency libraries and the `iab-validator` application in Step 3.
             
### MacOSX / Xcode
  
    cmake -G Xcode . -BTestBuildDir -DARCH=64 -DCMAKE_BUILD_TYPE=Release

The above step should generate an Xcode project iab-validator.xcodeproj and other CMake support files in TestBuildDir.
These files can be used to build the dependency libraries and the `iab-validator` application in Step 3.

### Windows / MSVC14 / 2015, 64-bit

    cmake -G "Visual Studio 14 2015 Win64" . -BTestBuildDir -DUSE_MSVS_MT=ON -DARCH=64 -DCMAKE_BUILD_TYPE=Release

The above MSVC14 step should generate the VS2015 file set: iab-validator.sln/.vcxproj/.vcxproj.filters, plus other CMake 
support files in TestBuildDir. These files can be used to build the dependency libraries and the `iab-validator` application in Step 3.

## Step 3. Build libraries and application

After Step 2, execute the following command to build binaries for all platforms:

    cmake --build TestBuildDir --config Release

## Additional notes

C++ language support: IAB-Validator source code is compliant to C++98.
