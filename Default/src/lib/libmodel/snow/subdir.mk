################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libmodel/snow/g_snow.c \
../src/lib/libmodel/snow/g_soil.c \
../src/lib/libmodel/snow/new_tsno.c 

OBJS += \
./src/lib/libmodel/snow/g_snow.o \
./src/lib/libmodel/snow/g_soil.o \
./src/lib/libmodel/snow/new_tsno.o 

C_DEPS += \
./src/lib/libmodel/snow/g_snow.d \
./src/lib/libmodel/snow/g_soil.d \
./src/lib/libmodel/snow/new_tsno.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libmodel/snow/%.o: ../src/lib/libmodel/snow/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


