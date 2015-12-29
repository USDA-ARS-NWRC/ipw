################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/rmhdr/main.o \
../src/bin/header/rmhdr/rmhdr.o 

C_SRCS += \
../src/bin/header/rmhdr/main.c \
../src/bin/header/rmhdr/rmhdr.c 

OBJS += \
./src/bin/header/rmhdr/main.o \
./src/bin/header/rmhdr/rmhdr.o 

C_DEPS += \
./src/bin/header/rmhdr/main.d \
./src/bin/header/rmhdr/rmhdr.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/rmhdr/%.o: ../src/bin/header/rmhdr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


