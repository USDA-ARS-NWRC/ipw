################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/mkgeoh/main.o \
../src/bin/header/mkgeoh/mkgeoh.o 

C_SRCS += \
../src/bin/header/mkgeoh/main.c \
../src/bin/header/mkgeoh/mkgeoh.c 

OBJS += \
./src/bin/header/mkgeoh/main.o \
./src/bin/header/mkgeoh/mkgeoh.o 

C_DEPS += \
./src/bin/header/mkgeoh/main.d \
./src/bin/header/mkgeoh/mkgeoh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mkgeoh/%.o: ../src/bin/header/mkgeoh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


