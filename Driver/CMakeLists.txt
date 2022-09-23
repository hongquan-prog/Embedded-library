cmake_minimum_required(VERSION 3.16)

# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set(TARGET_NAME ${CURRENT_FOLDER})

# project
project(${TARGET_NAME} LANGUAGES C)

# c sources
set(SOURCE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/bus/iic_simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/exit_interface
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/iic_interface
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/io_interface
    ${CMAKE_CURRENT_SOURCE_DIR}/module/at24cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/module/beep
    ${CMAKE_CURRENT_SOURCE_DIR}/module/dht11
    ${CMAKE_CURRENT_SOURCE_DIR}/module/ds18b20
    ${CMAKE_CURRENT_SOURCE_DIR}/module/hc-sr04
    ${CMAKE_CURRENT_SOURCE_DIR}/module/hmc5983
    ${CMAKE_CURRENT_SOURCE_DIR}/module/infrared
    ${CMAKE_CURRENT_SOURCE_DIR}/module/key
    ${CMAKE_CURRENT_SOURCE_DIR}/module/lcd1602
    ${CMAKE_CURRENT_SOURCE_DIR}/module/led
    ${CMAKE_CURRENT_SOURCE_DIR}/module/nrf24l01
    ${CMAKE_CURRENT_SOURCE_DIR}/module/ov2640
    ${CMAKE_CURRENT_SOURCE_DIR}/module/pcf8574
    ${CMAKE_CURRENT_SOURCE_DIR}/module/ov7725
    ${CMAKE_CURRENT_SOURCE_DIR}/module/rc522
    ${CMAKE_CURRENT_SOURCE_DIR}/module/sdcard
    ${CMAKE_CURRENT_SOURCE_DIR}/module/sht3x
    ${CMAKE_CURRENT_SOURCE_DIR}/module/w25qxx
    ${CMAKE_CURRENT_SOURCE_DIR}/module/ws2812
)
foreach(MODULE ${SOURCE_DIR})
    aux_source_directory(${MODULE} SRCS)
endforeach()

# c includes
set (INCS ${SOURCE_DIR})

add_library(${TARGET_NAME} STATIC ${SRCS})

target_include_directories(${TARGET_NAME} INTERFACE ${INCS})

target_include_directories(${TARGET_NAME} PRIVATE ${INCS})

target_link_libraries(${TARGET_NAME} PRIVATE Error)