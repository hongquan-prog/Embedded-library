cmake_minimum_required(VERSION 3.16)

add_subdirectory(STM32F7xx_HAL_Driver ${CMAKE_CURRENT_BINARY_DIR}/STM32F7xx_HAL_Driver)
add_subdirectory(Middlewares/Third_Party/FreeRTOS ${CMAKE_CURRENT_BINARY_DIR}/Middlewares/Third_Party/FreeRTOS)
add_subdirectory(Middlewares/Third_Party/freemodbus ${CMAKE_CURRENT_BINARY_DIR}/Middlewares/Third_Party/freemodbus)
