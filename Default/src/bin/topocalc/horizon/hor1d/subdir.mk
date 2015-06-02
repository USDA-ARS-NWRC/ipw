################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/horizon/hor1d/azmf.c \
../src/bin/topocalc/horizon/hor1d/bihvalid.c \
../src/bin/topocalc/horizon/hor1d/headers.c \
../src/bin/topocalc/horizon/hor1d/hor1b.c \
../src/bin/topocalc/horizon/hor1d/hor1f.c \
../src/bin/topocalc/horizon/hor1d/horizon.c \
../src/bin/topocalc/horizon/hor1d/hormask.c \
../src/bin/topocalc/horizon/hor1d/horval.c \
../src/bin/topocalc/horizon/hor1d/main.c \
../src/bin/topocalc/horizon/hor1d/newlqh.c \
../src/bin/topocalc/horizon/hor1d/parm.c \
../src/bin/topocalc/horizon/hor1d/zenf.c 

OBJS += \
./src/bin/topocalc/horizon/hor1d/azmf.o \
./src/bin/topocalc/horizon/hor1d/bihvalid.o \
./src/bin/topocalc/horizon/hor1d/headers.o \
./src/bin/topocalc/horizon/hor1d/hor1b.o \
./src/bin/topocalc/horizon/hor1d/hor1f.o \
./src/bin/topocalc/horizon/hor1d/horizon.o \
./src/bin/topocalc/horizon/hor1d/hormask.o \
./src/bin/topocalc/horizon/hor1d/horval.o \
./src/bin/topocalc/horizon/hor1d/main.o \
./src/bin/topocalc/horizon/hor1d/newlqh.o \
./src/bin/topocalc/horizon/hor1d/parm.o \
./src/bin/topocalc/horizon/hor1d/zenf.o 

C_DEPS += \
./src/bin/topocalc/horizon/hor1d/azmf.d \
./src/bin/topocalc/horizon/hor1d/bihvalid.d \
./src/bin/topocalc/horizon/hor1d/headers.d \
./src/bin/topocalc/horizon/hor1d/hor1b.d \
./src/bin/topocalc/horizon/hor1d/hor1f.d \
./src/bin/topocalc/horizon/hor1d/horizon.d \
./src/bin/topocalc/horizon/hor1d/hormask.d \
./src/bin/topocalc/horizon/hor1d/horval.d \
./src/bin/topocalc/horizon/hor1d/main.d \
./src/bin/topocalc/horizon/hor1d/newlqh.d \
./src/bin/topocalc/horizon/hor1d/parm.d \
./src/bin/topocalc/horizon/hor1d/zenf.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/horizon/hor1d/%.o: ../src/bin/topocalc/horizon/hor1d/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


