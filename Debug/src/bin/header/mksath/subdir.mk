################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/mksath/main.o \
../src/bin/header/mksath/mksath.o 

C_SRCS += \
../src/bin/header/mksath/main.c \
../src/bin/header/mksath/mksath.c 

OBJS += \
./src/bin/header/mksath/main.o \
./src/bin/header/mksath/mksath.o 

C_DEPS += \
./src/bin/header/mksath/main.d \
./src/bin/header/mksath/mksath.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mksath/%.o: ../src/bin/header/mksath/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


