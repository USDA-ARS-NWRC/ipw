################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/export/ipw2pbm/headers.c \
../src/bin/export/ipw2pbm/ipw2pbm.c \
../src/bin/export/ipw2pbm/main.c \
../src/bin/export/ipw2pbm/parm.c 

OBJS += \
./src/bin/export/ipw2pbm/headers.o \
./src/bin/export/ipw2pbm/ipw2pbm.o \
./src/bin/export/ipw2pbm/main.o \
./src/bin/export/ipw2pbm/parm.o 

C_DEPS += \
./src/bin/export/ipw2pbm/headers.d \
./src/bin/export/ipw2pbm/ipw2pbm.d \
./src/bin/export/ipw2pbm/main.d \
./src/bin/export/ipw2pbm/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2pbm/%.o: ../src/bin/export/ipw2pbm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


