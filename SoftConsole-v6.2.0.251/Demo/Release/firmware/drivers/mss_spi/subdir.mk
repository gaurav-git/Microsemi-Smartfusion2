################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/drivers/mss_spi/mss_spi.c 

OBJS += \
./firmware/drivers/mss_spi/mss_spi.o 

C_DEPS += \
./firmware/drivers/mss_spi/mss_spi.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/drivers/mss_spi/%.o: ../firmware/drivers/mss_spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -std=gnu11 --specs=cmsis.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


