################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/topocalc/horizon/azmf.o \
../src/bin/topocalc/horizon/bihvalid.o \
../src/bin/topocalc/horizon/headers.o \
../src/bin/topocalc/horizon/hor1b.o \
../src/bin/topocalc/horizon/hor1d.o \
../src/bin/topocalc/horizon/hor1f.o \
../src/bin/topocalc/horizon/horizon.o \
../src/bin/topocalc/horizon/hormask.o \
../src/bin/topocalc/horizon/horval.o \
../src/bin/topocalc/horizon/main.o \
../src/bin/topocalc/horizon/newlqh.o \
../src/bin/topocalc/horizon/parm.o \
../src/bin/topocalc/horizon/zenf.o 

C_SRCS += \
../src/bin/topocalc/horizon/azmf.c \
../src/bin/topocalc/horizon/bihvalid.c \
../src/bin/topocalc/horizon/headers.c \
../src/bin/topocalc/horizon/hor1b.c \
../src/bin/topocalc/horizon/hor1d.c \
../src/bin/topocalc/horizon/hor1f.c \
../src/bin/topocalc/horizon/horizon.c \
../src/bin/topocalc/horizon/hormask.c \
../src/bin/topocalc/horizon/horval.c \
../src/bin/topocalc/horizon/main.c \
../src/bin/topocalc/horizon/newlqh.c \
../src/bin/topocalc/horizon/parm.c \
../src/bin/topocalc/horizon/zenf.c 

OBJS += \
./src/bin/topocalc/horizon/azmf.o \
./src/bin/topocalc/horizon/bihvalid.o \
./src/bin/topocalc/horizon/headers.o \
./src/bin/topocalc/horizon/hor1b.o \
./src/bin/topocalc/horizon/hor1d.o \
./src/bin/topocalc/horizon/hor1f.o \
./src/bin/topocalc/horizon/horizon.o \
./src/bin/topocalc/horizon/hormask.o \
./src/bin/topocalc/horizon/horval.o \
./src/bin/topocalc/horizon/main.o \
./src/bin/topocalc/horizon/newlqh.o \
./src/bin/topocalc/horizon/parm.o \
./src/bin/topocalc/horizon/zenf.o 

C_DEPS += \
./src/bin/topocalc/horizon/azmf.d \
./src/bin/topocalc/horizon/bihvalid.d \
./src/bin/topocalc/horizon/headers.d \
./src/bin/topocalc/horizon/hor1b.d \
./src/bin/topocalc/horizon/hor1d.d \
./src/bin/topocalc/horizon/hor1f.d \
./src/bin/topocalc/horizon/horizon.d \
./src/bin/topocalc/horizon/hormask.d \
./src/bin/topocalc/horizon/horval.d \
./src/bin/topocalc/horizon/main.d \
./src/bin/topocalc/horizon/newlqh.d \
./src/bin/topocalc/horizon/parm.d \
./src/bin/topocalc/horizon/zenf.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/horizon/%.o: ../src/bin/topocalc/horizon/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


