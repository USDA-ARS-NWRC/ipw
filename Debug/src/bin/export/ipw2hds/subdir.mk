################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/export/ipw2hds/hdspic.o \
../src/bin/export/ipw2hds/headers.o \
../src/bin/export/ipw2hds/main.o \
../src/bin/export/ipw2hds/parm.o 

C_SRCS += \
../src/bin/export/ipw2hds/hdspic.c \
../src/bin/export/ipw2hds/headers.c \
../src/bin/export/ipw2hds/main.c \
../src/bin/export/ipw2hds/parm.c 

OBJS += \
./src/bin/export/ipw2hds/hdspic.o \
./src/bin/export/ipw2hds/headers.o \
./src/bin/export/ipw2hds/main.o \
./src/bin/export/ipw2hds/parm.o 

C_DEPS += \
./src/bin/export/ipw2hds/hdspic.d \
./src/bin/export/ipw2hds/headers.d \
./src/bin/export/ipw2hds/main.d \
./src/bin/export/ipw2hds/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2hds/%.o: ../src/bin/export/ipw2hds/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


