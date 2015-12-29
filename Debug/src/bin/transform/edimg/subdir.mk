################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/edimg/edimg.o \
../src/bin/transform/edimg/getedit.o \
../src/bin/transform/edimg/main.o 

C_SRCS += \
../src/bin/transform/edimg/edimg.c \
../src/bin/transform/edimg/getedit.c \
../src/bin/transform/edimg/main.c 

OBJS += \
./src/bin/transform/edimg/edimg.o \
./src/bin/transform/edimg/getedit.o \
./src/bin/transform/edimg/main.o 

C_DEPS += \
./src/bin/transform/edimg/edimg.d \
./src/bin/transform/edimg/getedit.d \
./src/bin/transform/edimg/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/edimg/%.o: ../src/bin/transform/edimg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


