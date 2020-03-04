cmake_minimum_required (VERSION 3.1.0)

# Set cmake project name to IABParser
project (IABParser)
set (TOKEN IABPARSER)

if (${TOKEN}_CREATE_SHARED_LIBS)
    SET(LIB_TYPE SHARED)
else ()
    SET(LIB_TYPE STATIC)
endif ()

MESSAGE(STATUS "${PROJECT_NAME} - configured as ${LIB_TYPE} lib.")

# Specify project source file list
set (SOURCE_FILES
    ../common/IABElements.cpp
    ../common/IABUtilities.cpp
    IABParser.cpp
)

# Specify project target
add_library (
    ${PROJECT_NAME}
    ${LIB_TYPE}
    ${SOURCE_FILES}
)

# Specify compiler include search path
target_include_directories (
  ${PROJECT_NAME} PUBLIC
  ../../include
  ../common
  ../codec/include
  ../common-stream
  PRIVATE
  )

# Specify library dependencies
target_link_libraries (
    ${PROJECT_NAME} PUBLIC
    DLC
    common-stream
)

# Collect header files so that they are indexed by IDE projects/solutions.
set (GLOB_LIST "*.h" "../../include/*.h")
file (GLOB HEADER_FILES ${GLOB_LIST})
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})
