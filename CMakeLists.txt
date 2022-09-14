cmake_minimum_required(VERSION 3.16)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
project(MetaOS LANGUAGES C CXX ASM)

# ASM sources
set(ASM_SOURCES ${CMAKE_SOURCE_DIR}/Project/MetaOS/startup_stm32f767xx.s)

# linker scripts interrupt vector table offset
set(LINKER_SCRIPTS ${CMAKE_SOURCE_DIR}/Project/MetaOS/STM32F767IGTx_FLASH.ld)

# FreeRTOS config
set(FREEROTS_CFG_INCLUDES  ${CMAKE_SOURCE_DIR}/Project/Driver/Core/Inc)

# cmsis includes
set(CMSIS_INCLUDES
${CMAKE_SOURCE_DIR}/Bsp/CMSIS/Include
${CMAKE_SOURCE_DIR}/Bsp/CMSIS/Device/ST/STM32F7xx/Include)

# C includes
set(C_INCLUDES 
  ${CMSIS_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Project/MetaOS/Core/Inc
)

# C sources
aux_source_directory(${CMAKE_SOURCE_DIR}/Project/MetaOS/Core/Src C_SOURCES)

######################################
# cross compile option
######################################
add_compile_options(
  -Wall
  $<$<CONFIG:DEBUG>:-g>
  $<$<CONFIG:DEBUG>:-gdwarf-2>
  $<$<CONFIG:DEBUG>:-O0>
  $<$<CONFIG:RELEASE>:-O1>
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

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
  -specs=nano.specs
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

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${C_INCLUDES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE STM32F7xx_HAL_Driver Error DataStructure)

######################################
# convert format
######################################
if (EXISTS ${CMAKE_OBJCOPY})
  add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_NAME:${CMAKE_PROJECT_NAME}>.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_NAME:${CMAKE_PROJECT_NAME}>.bin
  )
else()
  message(STATUS "'objcopy' not found: cannot generate .hex and .bin file")
endif()