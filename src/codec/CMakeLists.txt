cmake_minimum_required (VERSION 3.1.0)

# Set cmake project name to DLC
project (DLC)
set (TOKEN DLC)

if (${TOKEN}_CREATE_SHARED_LIBS)
    SET(LIB_TYPE SHARED)
else ()
    SET(LIB_TYPE STATIC)
endif ()

MESSAGE(STATUS "${PROJECT_NAME} - configured as ${LIB_TYPE} lib.")

# Specify project source file list
set (DLC_SOURCE_FILES
    src/DLCAudioData.cpp
    src/DLCFullDecoder.cpp
    src/DLCSimpleEncoder.cpp
    )

# Add DLC library target (DLC) to be build from the source list
add_library (
    ${PROJECT_NAME}
    ${LIB_TYPE}
    ${DLC_SOURCE_FILES}
    )

# Specify compiler include search path, including current folder (“src”) and ../include
target_include_directories (
  ${PROJECT_NAME} PUBLIC
  ./include
  PRIVATE
  )

# Collect header files so that they are indexed by IDE projects/solutions.
set (GLOB_LIST "*.h" "./include/*.h")
file (GLOB HEADER_FILES ${GLOB_LIST})
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})
