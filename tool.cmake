# set CMAKE_SYSTEM_NAME to define build as CMAKE_CROSSCOMPILING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION Cortex-M)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(ARM_TOOL_SUFFIX)

# tool path need suffix
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  set(ARM_TOOL_SUFFIX .exe)
endif()

# find tool chain
find_program(CMAKE_C_COMPILER arm-none-eabi-gcc${ARM_TOOL_SUFFIX}) 
if (NOT CMAKE_C_COMPILER) 
  message(FATAL_ERROR "Cannot find ARM GCC compiler: arm-none-eabi-gcc") 
endif() 
get_filename_component(ARM_TOOL_PATH ${CMAKE_C_COMPILER} PATH) 

# find st-flash
find_program(ST_FLASH st-flash${ARM_TOOL_SUFFIX}) 
if (NOT ST_FLASH) 
  message(FATAL_ERROR "Cannot find stlink tool") 
endif() 

# save tool path to cache
set(CMAKE_Cxx_COMPILER ${ARM_TOOL_PATH}/arm-none-eabi-g++${ARM_TOOL_SUFFIX} CACHE STRING "arm-none-eabi-g++")
set(ARM_TOOL_OBJCOPY ${ARM_TOOL_PATH}/arm-none-eabi-objcopy${ARM_TOOL_SUFFIX} CACHE STRING "arm-none-eabi-objcopy")
set(ARM_TOOL_AS ${ARM_TOOL_PATH}/arm-none-eabi-as${ARM_TOOL_SUFFIX} CACHE STRING "arm-none-eabi-as")
set(ARM_TOOL_LD ${ARM_TOOL_PATH}/arm-none-eabi-ld${ARM_TOOL_SUFFIX} CACHE STRING "arm-none-eabi-ld")
set(ARM_TOOL_SIZE ${ARM_TOOL_PATH}/arm-none-eabi-size${ARM_TOOL_SUFFIX} CACHE STRING "arm-none-eabi-size")

add_compile_options(
  -Wall
  -fdata-sections
  -ffunction-sections
  $<$<CONFIG:DEBUG>:-g>
  $<$<CONFIG:DEBUG>:-gdwarf-2>
  $<$<CONFIG:DEBUG>:-O0>
  $<$<CONFIG:RELEASE>:-O1>
)

# use these options to verify the linker can create an ELF file
# when not doing a static link
add_link_options(
  -specs=nosys.specs
  -Wl,--gc-sections 
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)