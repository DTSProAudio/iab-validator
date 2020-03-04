cmake_minimum_required (VERSION 3.1.0)

project (common-stream)
set (TOKEN COMMONSTREAM)

if (${TOKEN}_CREATE_SHARED_LIBS)
    SET(LIB_TYPE SHARED)
else ()
    SET(LIB_TYPE STATIC)
endif ()

MESSAGE(STATUS "${PROJECT_NAME} - configured as ${LIB_TYPE} lib.")

# Specify project header file list
set (PUBLIC_HEADERS
    bitstream/BitStreamIOHelpers.h
    bitstream/BitStreamReaderT.h
    bitstream/BitStreamWriterT.h
    bitstream/BitStreamStateT.h
    bytestream/ByteStreamIOHelpers.h
    bytestream/ByteStreamReaderT.h
    bytestream/ByteStreamWriterT.h
    hash/CRC16.h
    hash/CRCGenerator.h
    hash/HashGenerator.h
    stream/StreamTypes.h
    stream/StreamBasicTypes.h
    stream/StreamDefines.h
    utils/iitersbuf.h
    utils/opbacksbuf.h
    utils/StringUtils.h
    utils/Namespace.h
    # Needed for build with GCC 4.4.7 (pre C++11)
    ../include/nullptrOldCompilers.h
    hash/MD5Generator.h
    utils/PCMUtilities.h
)

# Source files are in these subdirectories
set (SOURCE_DIRS bitstream bytestream hash rawstream stream utils)

set (bitstream_SOURCE_FILES
    BitStreamStateT.cpp
    BitStreamReaderT.cpp
    BitStreamWriterT.cpp
    )

set (bytestream_SOURCE_FILES
    ByteStreamStateT.cpp
    ByteStreamReaderT.cpp
    ByteStreamWriterT.cpp
    )

set (hash_SOURCE_FILES
    CRC16.cpp
    MD5Generator.cpp
    )

set (rawstream_SOURCE_FILES
    RawIStream.cpp
    RawOStream.cpp
    RawStream.cpp
    )

set (stream_SOURCE_FILES
    StreamDefines.cpp
    StreamTypes.cpp
    )

set (utils_SOURCE_FILES
    ByteSwap.cpp
    StringUtils.cpp
    )

# Make a list of all source files with their subdirectories
set (SOURCES)
foreach (SOURCE_DIR ${SOURCE_DIRS})
    set (${SOURCE_DIR}_SOURCE_PATHS)
    foreach (SOURCE_FILE ${${SOURCE_DIR}_SOURCE_FILES})
        list (APPEND ${SOURCE_DIR}_SOURCE_PATHS ${SOURCE_DIR}/${SOURCE_FILE})
    endforeach()
    list (APPEND SOURCES ${${SOURCE_DIR}_SOURCE_PATHS})
endforeach()

# Define for any parent that builds with common-stream sources
set (${TOKEN}_SOURCE_FILES ${SOURCES} PARENT_SCOPE)

# Add common-stream library target
add_library (
    ${PROJECT_NAME}
    ${LIB_TYPE}
    ${SOURCES}
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}"
    )

# Specify compiler include search path
target_include_directories (${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stream
    ${CMAKE_CURRENT_SOURCE_DIR}/bitstream
    ${CMAKE_CURRENT_SOURCE_DIR}/hash
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${${TOKEN}_INC_INSTALL_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bytestream
    ${CMAKE_CURRENT_SOURCE_DIR}/rawstream
    )

# Collect header files so that they are indexed by IDE projects/solutions.
set (GLOB_LIST)
foreach (INC_DIR ${SOURCE_DIRS})
    list(APPEND GLOB_LIST "${INC_DIR}/*.h")
endforeach()
file (GLOB HEADER_FILES ${GLOB_LIST})
target_sources (${PROJECT_NAME} PRIVATE ${HEADER_FILES})
