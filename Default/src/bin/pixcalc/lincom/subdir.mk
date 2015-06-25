################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/lincom/coeffs.c \
../src/bin/pixcalc/lincom/headers.c \
../src/bin/pixcalc/lincom/lincom.c \
../src/bin/pixcalc/lincom/main.c \
../src/bin/pixcalc/lincom/output.c \
../src/bin/pixcalc/lincom/parm.c \
../src/bin/pixcalc/lincom/re_map.c 

OBJS += \
./src/bin/pixcalc/lincom/coeffs.o \
./src/bin/pixcalc/lincom/headers.o \
./src/bin/pixcalc/lincom/lincom.o \
./src/bin/pixcalc/lincom/main.o \
./src/bin/pixcalc/lincom/output.o \
./src/bin/pixcalc/lincom/parm.o \
./src/bin/pixcalc/lincom/re_map.o 

C_DEPS += \
./src/bin/pixcalc/lincom/coeffs.d \
./src/bin/pixcalc/lincom/headers.d \
./src/bin/pixcalc/lincom/lincom.d \
./src/bin/pixcalc/lincom/main.d \
./src/bin/pixcalc/lincom/output.d \
./src/bin/pixcalc/lincom/parm.d \
./src/bin/pixcalc/lincom/re_map.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/lincom/%.o: ../src/bin/pixcalc/lincom/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


