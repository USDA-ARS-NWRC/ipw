################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/despike/despike.o \
../src/bin/pixcalc/despike/headers.o \
../src/bin/pixcalc/despike/main.o 

C_SRCS += \
../src/bin/pixcalc/despike/despike.c \
../src/bin/pixcalc/despike/headers.c \
../src/bin/pixcalc/despike/main.c 

OBJS += \
./src/bin/pixcalc/despike/despike.o \
./src/bin/pixcalc/despike/headers.o \
./src/bin/pixcalc/despike/main.o 

C_DEPS += \
./src/bin/pixcalc/despike/despike.d \
./src/bin/pixcalc/despike/headers.d \
./src/bin/pixcalc/despike/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/despike/%.o: ../src/bin/pixcalc/despike/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


