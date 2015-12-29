################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/units/_unitvars.o \
../src/lib/libipw/units/units_as_str.o \
../src/lib/libipw/units/units_match.o 

C_SRCS += \
../src/lib/libipw/units/_unitvars.c \
../src/lib/libipw/units/units_as_str.c \
../src/lib/libipw/units/units_match.c 

OBJS += \
./src/lib/libipw/units/_unitvars.o \
./src/lib/libipw/units/units_as_str.o \
./src/lib/libipw/units/units_match.o 

C_DEPS += \
./src/lib/libipw/units/_unitvars.d \
./src/lib/libipw/units/units_as_str.d \
./src/lib/libipw/units/units_match.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/units/%.o: ../src/lib/libipw/units/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


