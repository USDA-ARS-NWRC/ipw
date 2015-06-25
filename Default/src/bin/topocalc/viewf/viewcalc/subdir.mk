################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/viewf/viewcalc/buffers.c \
../src/bin/topocalc/viewf/viewcalc/headers.c \
../src/bin/topocalc/viewf/viewcalc/main.c \
../src/bin/topocalc/viewf/viewcalc/newlqh.c \
../src/bin/topocalc/viewf/viewcalc/parm.c \
../src/bin/topocalc/viewf/viewcalc/trigtbl.c \
../src/bin/topocalc/viewf/viewcalc/viewf.c 

OBJS += \
./src/bin/topocalc/viewf/viewcalc/buffers.o \
./src/bin/topocalc/viewf/viewcalc/headers.o \
./src/bin/topocalc/viewf/viewcalc/main.o \
./src/bin/topocalc/viewf/viewcalc/newlqh.o \
./src/bin/topocalc/viewf/viewcalc/parm.o \
./src/bin/topocalc/viewf/viewcalc/trigtbl.o \
./src/bin/topocalc/viewf/viewcalc/viewf.o 

C_DEPS += \
./src/bin/topocalc/viewf/viewcalc/buffers.d \
./src/bin/topocalc/viewf/viewcalc/headers.d \
./src/bin/topocalc/viewf/viewcalc/main.d \
./src/bin/topocalc/viewf/viewcalc/newlqh.d \
./src/bin/topocalc/viewf/viewcalc/parm.d \
./src/bin/topocalc/viewf/viewcalc/trigtbl.d \
./src/bin/topocalc/viewf/viewcalc/viewf.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/viewf/viewcalc/%.o: ../src/bin/topocalc/viewf/viewcalc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


