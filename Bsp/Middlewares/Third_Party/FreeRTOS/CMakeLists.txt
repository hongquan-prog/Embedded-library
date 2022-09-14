cmake_minimum_required(VERSION 3.16)

# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set(TARGET_NAME ${CURRENT_FOLDER})

# project
project(${TARGET_NAME} LANGUAGES C)

# c sources
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Source SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/MemMang SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/GCC/ARM_CM7/r0p1 SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Source/CMSIS_RTOS_V2 SRCS)

# c includes
set (INCS
${CMSIS_INCLUDES}
${FREEROTS_CFG_INCLUDES}
${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/GCC/ARM_CM7/r0p1
${CMAKE_CURRENT_SOURCE_DIR}/Source/include
${CMAKE_CURRENT_SOURCE_DIR}/Source/CMSIS_RTOS_V2)

add_library(${TARGET_NAME} STATIC ${SRCS})

target_include_directories(${TARGET_NAME} INTERFACE ${INCS})

target_include_directories(${TARGET_NAME} PRIVATE ${INCS})