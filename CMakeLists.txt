cmake_minimum_required(VERSION 3.16)

# default target
if(NOT CMD_TARGET)
  set(CMD_TARGET MetaOS)
endif()

# project name
project(${CMD_TARGET} LANGUAGES C CXX ASM)

# source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Project/${CMD_TARGET})

# ASM sources
set(ASM_SOURCES ${SOURCE_DIR}/startup_stm32f767xx.s)

# linker scripts interrupt vector table offset
set(LINKER_SCRIPTS ${SOURCE_DIR}/STM32F767IGTx_FLASH.ld)

# ARM_OPTIONS is both a compiler and linker option
set(ARM_OPTIONS -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard)

# FreeRTOS config
set(FREEROTS_CFG_INCLUDES  ${CMAKE_SOURCE_DIR}/Project/Driver/Core/Inc)

# cmsis includes
set(CMSIS_INCLUDES
${CMAKE_SOURCE_DIR}/Bsp/CMSIS/Include
${CMAKE_SOURCE_DIR}/Bsp/CMSIS/Device/ST/STM32F7xx/Include)

# C includes
set(C_INCLUDES 
  ${CMSIS_INCLUDES}
  ${SOURCE_DIR}/Core/Inc
)

# C sources
aux_source_directory(${SOURCE_DIR}/Core/Src C_SOURCES)

######################################
# macro
######################################
add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  USE_HAL_DRIVER
  STM32F767xx
)

######################################
# compile option
######################################
add_compile_options(
  ${ARM_OPTIONS}
)

######################################
# cross compile 
######################################
add_executable(${CMAKE_PROJECT_NAME}
  ${ASM_SOURCES}
  ${C_SOURCES}
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
  SUFFIX .elf
  LINK_DEPENDS ${LINKER_SCRIPTS}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
${C_INCLUDES}
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
  ${ARM_OPTIONS}
  -specs=nano.specs
  -u 
  _printf_float
  -T${LINKER_SCRIPTS}
  LINKER:--print-memory-usage
  LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  c_nano 
  nosys
  m
)

######################################
# compile module
######################################
set(MODULES Bsp Error DataStructure Driver)
foreach(LIB ${MODULES})
  add_subdirectory(${LIB} ${CMAKE_BINARY_DIR}/${LIB})
endforeach()

######################################
# link external library
######################################
include(${SOURCE_DIR}/component.cmake)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
  ${EXTERNAL_LIB}
)

######################################
# convert format
######################################
if (EXISTS ${ARM_TOOL_OBJCOPY})
  add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${ARM_TOOL_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_NAME:${CMAKE_PROJECT_NAME}>.hex
    COMMAND ${ARM_TOOL_OBJCOPY} -O binary -S $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_NAME:${CMAKE_PROJECT_NAME}>.bin
  )
else()
  message(STATUS "'objcopy' not found: cannot generate .hex and .bin file")
endif()

######################################
# flash
######################################
add_custom_target(flash DEPENDS ${CMAKE_PROJECT_NAME})
add_custom_command(TARGET flash
  COMMAND st-flash --reset write ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0X8000000
)