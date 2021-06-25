################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/usart_dma/source/usart2_dma.c 

OBJS += \
./Drivers/usart_dma/source/usart2_dma.o 

C_DEPS += \
./Drivers/usart_dma/source/usart2_dma.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/usart_dma/source/usart2_dma.o: ../Drivers/usart_dma/source/usart2_dma.c Drivers/usart_dma/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../Drivers/FLASH/include" -I"../Drivers/usart_dma/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/usart_dma/source/usart2_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

