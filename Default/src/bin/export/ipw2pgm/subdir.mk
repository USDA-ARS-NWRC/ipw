################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/export/ipw2pgm/headers.c \
../src/bin/export/ipw2pgm/ipw2pgm.c \
../src/bin/export/ipw2pgm/main.c \
../src/bin/export/ipw2pgm/qipw2pgm.c 

OBJS += \
./src/bin/export/ipw2pgm/headers.o \
./src/bin/export/ipw2pgm/ipw2pgm.o \
./src/bin/export/ipw2pgm/main.o \
./src/bin/export/ipw2pgm/qipw2pgm.o 

C_DEPS += \
./src/bin/export/ipw2pgm/headers.d \
./src/bin/export/ipw2pgm/ipw2pgm.d \
./src/bin/export/ipw2pgm/main.d \
./src/bin/export/ipw2pgm/qipw2pgm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2pgm/%.o: ../src/bin/export/ipw2pgm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


