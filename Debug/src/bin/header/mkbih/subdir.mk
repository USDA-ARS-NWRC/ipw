################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/mkbih/getyi.o \
../src/bin/header/mkbih/main.o \
../src/bin/header/mkbih/mkbih.o 

C_SRCS += \
../src/bin/header/mkbih/getyi.c \
../src/bin/header/mkbih/main.c \
../src/bin/header/mkbih/mkbih.c 

OBJS += \
./src/bin/header/mkbih/getyi.o \
./src/bin/header/mkbih/main.o \
./src/bin/header/mkbih/mkbih.o 

C_DEPS += \
./src/bin/header/mkbih/getyi.d \
./src/bin/header/mkbih/main.d \
./src/bin/header/mkbih/mkbih.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mkbih/%.o: ../src/bin/header/mkbih/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


