################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/fillbands/fillbands.o \
../src/bin/transform/fillbands/headers.o \
../src/bin/transform/fillbands/main.o \
../src/bin/transform/fillbands/newlqh.o 

C_SRCS += \
../src/bin/transform/fillbands/fillbands.c \
../src/bin/transform/fillbands/headers.c \
../src/bin/transform/fillbands/main.c \
../src/bin/transform/fillbands/newlqh.c 

OBJS += \
./src/bin/transform/fillbands/fillbands.o \
./src/bin/transform/fillbands/headers.o \
./src/bin/transform/fillbands/main.o \
./src/bin/transform/fillbands/newlqh.o 

C_DEPS += \
./src/bin/transform/fillbands/fillbands.d \
./src/bin/transform/fillbands/headers.d \
./src/bin/transform/fillbands/main.d \
./src/bin/transform/fillbands/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/fillbands/%.o: ../src/bin/transform/fillbands/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


