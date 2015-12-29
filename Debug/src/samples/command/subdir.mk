################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/samples/command/foo.c \
../src/samples/command/globals.c \
../src/samples/command/headers.c \
../src/samples/command/main.c 

OBJS += \
./src/samples/command/foo.o \
./src/samples/command/globals.o \
./src/samples/command/headers.o \
./src/samples/command/main.o 

C_DEPS += \
./src/samples/command/foo.d \
./src/samples/command/globals.d \
./src/samples/command/headers.d \
./src/samples/command/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/samples/command/%.o: ../src/samples/command/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


