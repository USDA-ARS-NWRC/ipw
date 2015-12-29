################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/mksunh/main.o \
../src/bin/header/mksunh/mksunh.o 

C_SRCS += \
../src/bin/header/mksunh/main.c \
../src/bin/header/mksunh/mksunh.c 

OBJS += \
./src/bin/header/mksunh/main.o \
./src/bin/header/mksunh/mksunh.o 

C_DEPS += \
./src/bin/header/mksunh/main.d \
./src/bin/header/mksunh/mksunh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mksunh/%.o: ../src/bin/header/mksunh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


