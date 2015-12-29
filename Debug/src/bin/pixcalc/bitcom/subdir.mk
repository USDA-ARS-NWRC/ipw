################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/bitcom/bitcom.o \
../src/bin/pixcalc/bitcom/bitio.o \
../src/bin/pixcalc/bitcom/bx_and.o \
../src/bin/pixcalc/bitcom/bx_or.o \
../src/bin/pixcalc/bitcom/bx_xor.o \
../src/bin/pixcalc/bitcom/m_expand.o \
../src/bin/pixcalc/bitcom/main.o 

C_SRCS += \
../src/bin/pixcalc/bitcom/bitcom.c \
../src/bin/pixcalc/bitcom/bitio.c \
../src/bin/pixcalc/bitcom/bx_and.c \
../src/bin/pixcalc/bitcom/bx_or.c \
../src/bin/pixcalc/bitcom/bx_xor.c \
../src/bin/pixcalc/bitcom/m_expand.c \
../src/bin/pixcalc/bitcom/main.c 

OBJS += \
./src/bin/pixcalc/bitcom/bitcom.o \
./src/bin/pixcalc/bitcom/bitio.o \
./src/bin/pixcalc/bitcom/bx_and.o \
./src/bin/pixcalc/bitcom/bx_or.o \
./src/bin/pixcalc/bitcom/bx_xor.o \
./src/bin/pixcalc/bitcom/m_expand.o \
./src/bin/pixcalc/bitcom/main.o 

C_DEPS += \
./src/bin/pixcalc/bitcom/bitcom.d \
./src/bin/pixcalc/bitcom/bitio.d \
./src/bin/pixcalc/bitcom/bx_and.d \
./src/bin/pixcalc/bitcom/bx_or.d \
./src/bin/pixcalc/bitcom/bx_xor.d \
./src/bin/pixcalc/bitcom/m_expand.d \
./src/bin/pixcalc/bitcom/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/bitcom/%.o: ../src/bin/pixcalc/bitcom/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


