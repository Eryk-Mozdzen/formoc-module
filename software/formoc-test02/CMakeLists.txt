cmake_minimum_required(VERSION 3.16)

project(formoc_test02)

enable_language(C ASM)

set(TARGET ${CMAKE_PROJECT_NAME})
set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CC "arm-none-eabi-gcc")
set(AS "arm-none-eabi-as")
set(OBJCOPY "arm-none-eabi-objcopy")
set(OBJDUMP "arm-none-eabi-objdump")
set(SIZE "arm-none-eabi-size")

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_ASM_COMPILER  ${AS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L ${BASE_DIR}/linker")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T STM32L476RGTX_FLASH.ld")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -O -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -DSTM32L476xx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nosys.specs")

# floating point arithmetic
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math -fsingle-precision-constant")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdouble-promotion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(SOURCES
	"${BASE_DIR}/third-party/CMSIS/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c"
	"${BASE_DIR}/third-party/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l476xx.s"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/tasks.c"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/queue.c"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/list.c"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/portable/MemMang/heap_1.c"
	"${BASE_DIR}/code/src/main.c"
	"${BASE_DIR}/code/src/config.c"
)

include_directories(
	"${BASE_DIR}/third-party/CMSIS/Include"
	"${BASE_DIR}/third-party/CMSIS/Device/ST/STM32L4xx/Include"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/include"
	"${BASE_DIR}/third-party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F"
	"${BASE_DIR}/code/inc"
)

add_executable(${TARGET}.elf ${SOURCES})

target_link_libraries(${TARGET}.elf m)

add_custom_command(
	TARGET ${TARGET}.elf POST_BUILD
	COMMAND ${OBJCOPY} -O ihex ${TARGET}.elf ${TARGET}.hex
)

add_custom_command(
	TARGET ${TARGET}.elf POST_BUILD
	COMMAND ${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin
)

add_custom_command(
	TARGET ${TARGET}.elf POST_BUILD
	COMMAND ${SIZE} ${TARGET}.elf
)
