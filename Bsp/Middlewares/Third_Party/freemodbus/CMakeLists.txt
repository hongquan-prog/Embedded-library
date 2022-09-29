cmake_minimum_required(VERSION 3.16)

# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set(TARGET_NAME ${CURRENT_FOLDER})

# project
project(${TARGET_NAME} LANGUAGES C)

# c sources
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ascii SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/functions SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/port SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rtu SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tcp SRCS)

# c includes
set (INCS
${CMAKE_CURRENT_SOURCE_DIR}/ascii
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/port
${CMAKE_CURRENT_SOURCE_DIR}/rtu
${CMAKE_CURRENT_SOURCE_DIR}/tcp)

add_library(${TARGET_NAME} STATIC ${SRCS})

target_include_directories(${TARGET_NAME} INTERFACE ${INCS})

target_include_directories(${TARGET_NAME} PRIVATE ${INCS})