################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/g_data_processing_thread.c \
../ra_gen/g_graphics_update_thread.c \
../ra_gen/g_sensor_acquisition_thread.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/g_data_processing_thread.d \
./ra_gen/g_graphics_update_thread.d \
./ra_gen/g_sensor_acquisition_thread.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/g_data_processing_thread.o \
./ra_gen/g_graphics_update_thread.o \
./ra_gen/g_sensor_acquisition_thread.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
RA8D1_Flex_Simulator_RTOS.srec 

MAP += \
RA8D1_Flex_Simulator_RTOS.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_gen" -I"." -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/fsp_cfg/bsp" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/fsp_cfg" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra_cfg/aws" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/src" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc/api" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/inc/instances" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/fsp/src/rm_freertos_port" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/aws/FreeRTOS/FreeRTOS/Source/include" -I"C:/jostin/reneses/RA8D1_Flex_Simulator_RTOS/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

