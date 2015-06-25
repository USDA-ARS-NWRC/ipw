################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/imath/cmdbuf.c \
../src/bin/pixcalc/imath/cmdcodes.c \
../src/bin/pixcalc/imath/command.c \
../src/bin/pixcalc/imath/compile.c \
../src/bin/pixcalc/imath/expression.c \
../src/bin/pixcalc/imath/globals.c \
../src/bin/pixcalc/imath/headers.c \
../src/bin/pixcalc/imath/imath.c \
../src/bin/pixcalc/imath/main.c \
../src/bin/pixcalc/imath/main_compile.c \
../src/bin/pixcalc/imath/main_token.c \
../src/bin/pixcalc/imath/main_xcute.c \
../src/bin/pixcalc/imath/ordinal.c \
../src/bin/pixcalc/imath/token.c \
../src/bin/pixcalc/imath/tokentypes.c 

OBJS += \
./src/bin/pixcalc/imath/cmdbuf.o \
./src/bin/pixcalc/imath/cmdcodes.o \
./src/bin/pixcalc/imath/command.o \
./src/bin/pixcalc/imath/compile.o \
./src/bin/pixcalc/imath/expression.o \
./src/bin/pixcalc/imath/globals.o \
./src/bin/pixcalc/imath/headers.o \
./src/bin/pixcalc/imath/imath.o \
./src/bin/pixcalc/imath/main.o \
./src/bin/pixcalc/imath/main_compile.o \
./src/bin/pixcalc/imath/main_token.o \
./src/bin/pixcalc/imath/main_xcute.o \
./src/bin/pixcalc/imath/ordinal.o \
./src/bin/pixcalc/imath/token.o \
./src/bin/pixcalc/imath/tokentypes.o 

C_DEPS += \
./src/bin/pixcalc/imath/cmdbuf.d \
./src/bin/pixcalc/imath/cmdcodes.d \
./src/bin/pixcalc/imath/command.d \
./src/bin/pixcalc/imath/compile.d \
./src/bin/pixcalc/imath/expression.d \
./src/bin/pixcalc/imath/globals.d \
./src/bin/pixcalc/imath/headers.d \
./src/bin/pixcalc/imath/imath.d \
./src/bin/pixcalc/imath/main.d \
./src/bin/pixcalc/imath/main_compile.d \
./src/bin/pixcalc/imath/main_token.d \
./src/bin/pixcalc/imath/main_xcute.d \
./src/bin/pixcalc/imath/ordinal.d \
./src/bin/pixcalc/imath/token.d \
./src/bin/pixcalc/imath/tokentypes.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/imath/%.o: ../src/bin/pixcalc/imath/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


