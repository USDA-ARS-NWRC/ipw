################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.spat/wbal/aet.o \
../src/bin/physmod.spat/wbal/baseflow.o \
../src/bin/physmod.spat/wbal/headers.o \
../src/bin/physmod.spat/wbal/main.o \
../src/bin/physmod.spat/wbal/newlqh.o \
../src/bin/physmod.spat/wbal/output.o \
../src/bin/physmod.spat/wbal/wbal.o 

C_SRCS += \
../src/bin/physmod.spat/wbal/aet.c \
../src/bin/physmod.spat/wbal/baseflow.c \
../src/bin/physmod.spat/wbal/headers.c \
../src/bin/physmod.spat/wbal/main.c \
../src/bin/physmod.spat/wbal/newlqh.c \
../src/bin/physmod.spat/wbal/output.c \
../src/bin/physmod.spat/wbal/wbal.c 

OBJS += \
./src/bin/physmod.spat/wbal/aet.o \
./src/bin/physmod.spat/wbal/baseflow.o \
./src/bin/physmod.spat/wbal/headers.o \
./src/bin/physmod.spat/wbal/main.o \
./src/bin/physmod.spat/wbal/newlqh.o \
./src/bin/physmod.spat/wbal/output.o \
./src/bin/physmod.spat/wbal/wbal.o 

C_DEPS += \
./src/bin/physmod.spat/wbal/aet.d \
./src/bin/physmod.spat/wbal/baseflow.d \
./src/bin/physmod.spat/wbal/headers.d \
./src/bin/physmod.spat/wbal/main.d \
./src/bin/physmod.spat/wbal/newlqh.d \
./src/bin/physmod.spat/wbal/output.d \
./src/bin/physmod.spat/wbal/wbal.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/wbal/%.o: ../src/bin/physmod.spat/wbal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


