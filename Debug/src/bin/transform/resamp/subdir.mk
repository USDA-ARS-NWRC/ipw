################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/resamp/headers.o \
../src/bin/transform/resamp/main.o \
../src/bin/transform/resamp/out_line.o \
../src/bin/transform/resamp/resamp.o \
../src/bin/transform/resamp/resamp_line.o 

C_SRCS += \
../src/bin/transform/resamp/headers.c \
../src/bin/transform/resamp/main.c \
../src/bin/transform/resamp/out_line.c \
../src/bin/transform/resamp/resamp.c \
../src/bin/transform/resamp/resamp_line.c 

OBJS += \
./src/bin/transform/resamp/headers.o \
./src/bin/transform/resamp/main.o \
./src/bin/transform/resamp/out_line.o \
./src/bin/transform/resamp/resamp.o \
./src/bin/transform/resamp/resamp_line.o 

C_DEPS += \
./src/bin/transform/resamp/headers.d \
./src/bin/transform/resamp/main.d \
./src/bin/transform/resamp/out_line.d \
./src/bin/transform/resamp/resamp.d \
./src/bin/transform/resamp/resamp_line.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/resamp/%.o: ../src/bin/transform/resamp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


