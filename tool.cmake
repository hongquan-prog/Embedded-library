# set CMAKE_SYSTEM_NAME to define build as CMAKE_CROSSCOMPILING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION Cortex-M7-STM32F767IGT6)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# --specs=nano.specs is both a compiler and linker option
set(ARM_OPTIONS -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard)

add_compile_options(
  ${ARM_OPTIONS}
  -Wall
  -fdata-sections
  -ffunction-sections
)

add_compile_definitions(
  USE_HAL_DRIVER
  STM32F767xx
)

# use these options to verify the linker can create an ELF file
# when not doing a static link
add_link_options(
  ${ARM_OPTIONS}
  -specs=nosys.specs
  -Wl,--gc-sections)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
