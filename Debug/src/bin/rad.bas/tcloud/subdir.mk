################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/rad.bas/tcloud/headers.o \
../src/bin/rad.bas/tcloud/main.o \
../src/bin/rad.bas/tcloud/output.o \
../src/bin/rad.bas/tcloud/parm.o \
../src/bin/rad.bas/tcloud/tcloud.o 

C_SRCS += \
../src/bin/rad.bas/tcloud/headers.c \
../src/bin/rad.bas/tcloud/main.c \
../src/bin/rad.bas/tcloud/output.c \
../src/bin/rad.bas/tcloud/parm.c \
../src/bin/rad.bas/tcloud/tcloud.c 

OBJS += \
./src/bin/rad.bas/tcloud/headers.o \
./src/bin/rad.bas/tcloud/main.o \
./src/bin/rad.bas/tcloud/output.o \
./src/bin/rad.bas/tcloud/parm.o \
./src/bin/rad.bas/tcloud/tcloud.o 

C_DEPS += \
./src/bin/rad.bas/tcloud/headers.d \
./src/bin/rad.bas/tcloud/main.d \
./src/bin/rad.bas/tcloud/output.d \
./src/bin/rad.bas/tcloud/parm.d \
./src/bin/rad.bas/tcloud/tcloud.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/tcloud/%.o: ../src/bin/rad.bas/tcloud/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


