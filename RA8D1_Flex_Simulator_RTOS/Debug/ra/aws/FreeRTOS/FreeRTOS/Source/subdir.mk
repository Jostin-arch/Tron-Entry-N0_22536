################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/list.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/queue.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/tasks.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/timers.c 

C_DEPS += \
./ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/list.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/queue.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/tasks.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/timers.d 

OBJS += \
./ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/list.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/queue.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/tasks.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/timers.o 

SREC += \
RA8D1_Flex_Simulator_RTOS.srec 

MAP += \
RA8D1_Flex_Simulator_RTOS.map 


# Each subdirectory must supply rules for building sources it contributes
ra/aws/FreeRTOS/FreeRTOS/Source/%.o: ../ra/aws/FreeRTOS/FreeRTOS/Source/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_gen" -I"." -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/fsp_cfg/bsp" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/fsp_cfg" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/aws" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/src" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc/api" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc/instances" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/src/rm_freertos_port" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/aws/FreeRTOS/FreeRTOS/Source/include" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -w -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

