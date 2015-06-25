################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/idewpt/headers.c \
../src/bin/phys.spat/idewpt/idewpt.c \
../src/bin/phys.spat/idewpt/main.c \
../src/bin/phys.spat/idewpt/newlqh.c \
../src/bin/phys.spat/idewpt/output.c 

OBJS += \
./src/bin/phys.spat/idewpt/headers.o \
./src/bin/phys.spat/idewpt/idewpt.o \
./src/bin/phys.spat/idewpt/main.o \
./src/bin/phys.spat/idewpt/newlqh.o \
./src/bin/phys.spat/idewpt/output.o 

C_DEPS += \
./src/bin/phys.spat/idewpt/headers.d \
./src/bin/phys.spat/idewpt/idewpt.d \
./src/bin/phys.spat/idewpt/main.d \
./src/bin/phys.spat/idewpt/newlqh.d \
./src/bin/phys.spat/idewpt/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/idewpt/%.o: ../src/bin/phys.spat/idewpt/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


