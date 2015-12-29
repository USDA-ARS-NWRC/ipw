################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/export/ipw2xim/headers.o \
../src/bin/export/ipw2xim/ipw2xim.o \
../src/bin/export/ipw2xim/main.o \
../src/bin/export/ipw2xim/parm.o \
../src/bin/export/ipw2xim/ximhdr.o 

C_SRCS += \
../src/bin/export/ipw2xim/headers.c \
../src/bin/export/ipw2xim/ipw2xim.c \
../src/bin/export/ipw2xim/main.c \
../src/bin/export/ipw2xim/parm.c \
../src/bin/export/ipw2xim/ximhdr.c 

OBJS += \
./src/bin/export/ipw2xim/headers.o \
./src/bin/export/ipw2xim/ipw2xim.o \
./src/bin/export/ipw2xim/main.o \
./src/bin/export/ipw2xim/parm.o \
./src/bin/export/ipw2xim/ximhdr.o 

C_DEPS += \
./src/bin/export/ipw2xim/headers.d \
./src/bin/export/ipw2xim/ipw2xim.d \
./src/bin/export/ipw2xim/main.d \
./src/bin/export/ipw2xim/parm.d \
./src/bin/export/ipw2xim/ximhdr.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2xim/%.o: ../src/bin/export/ipw2xim/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


