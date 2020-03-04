cmake_minimum_required(VERSION 3.1.0)

project(iab-validator)

# Path to JSON source files
set (JSONPATH ./thirdparty/libjson/)

list(APPEND CMAKE_PREFIX_PATH ${IABTOOL_CMAKE_DIR})

# Compiler options
if (MSVC)
    option (USE_MSVS_MT "Use static runtime libraries for visual studio." ON)
    if (USE_MSVS_MT)
        set(_CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELWITHDEBINFO
            )
        foreach(_CompilerFlag ${_CompilerFlags})
            string(REPLACE "/MD" "/MT" ${_CompilerFlag} "${${_CompilerFlag}}")
        endforeach()
    endif ()

    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DJSON_DEBUG")
    set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DJSON_DEBUG")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.6.0")
        set (CMAKE_CXX_STANDARD 98)
        add_definitions (-DUSE_MDA_NULLPTR)
    else ()
        set (CMAKE_CXX_STANDARD 11)
    endif ()
    # GCC 6, -no-pie allows static libs not compiled with -fPIC
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6" )
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    endif ()
endif ()

# Add subdirectory
add_subdirectory (src)

# Specify project source list
set (APPLICATION_FILES
    ./app/main.cpp
    ./app/ValidateIABStream.cpp
    ${JSONPATH}/_internal/Source/libjson.cpp
    ${JSONPATH}/_internal/Source/JSONNode.cpp
    ${JSONPATH}/_internal/Source/JSONStream.cpp
    ${JSONPATH}/_internal/Source/JSONWorker.cpp
    ${JSONPATH}/_internal/Source/internalJSONNode.cpp
    ${JSONPATH}/_internal/Source/JSONChildren.cpp
    ${JSONPATH}/_internal/Source/JSONValidator.cpp
    ${JSONPATH}/_internal/Source/JSONWriter.cpp
    ${JSONPATH}/_internal/Source/JSONIterators.cpp
    ${JSONPATH}/_internal/Source/JSONDebug.cpp
)

# Specify project target
add_executable(
    ${PROJECT_NAME}
    ${APPLICATION_FILES}
    )

# Specify compiler include search path
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ./app
    ${JSONPATH}/_internal/Source
    ${JSONPATH}
    )

# Specify dependent libraries
set(LIBS_LIST
    IABParser
    IABValidator
    DLC
    common-stream
    )

# Collect header files so that they are indexed by IDE projects/solutions.
file (GLOB HEADER_FILES "./app/*.h")
target_sources (${PROJECT_NAME} PRIVATE ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS_LIST})
