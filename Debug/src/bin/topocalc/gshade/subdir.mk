################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/topocalc/gshade/gshade.o \
../src/bin/topocalc/gshade/headers.o \
../src/bin/topocalc/gshade/main.o \
../src/bin/topocalc/gshade/newlqh.o \
../src/bin/topocalc/gshade/trigtbl.o 

C_SRCS += \
../src/bin/topocalc/gshade/gshade.c \
../src/bin/topocalc/gshade/headers.c \
../src/bin/topocalc/gshade/main.c \
../src/bin/topocalc/gshade/newlqh.c \
../src/bin/topocalc/gshade/trigtbl.c 

OBJS += \
./src/bin/topocalc/gshade/gshade.o \
./src/bin/topocalc/gshade/headers.o \
./src/bin/topocalc/gshade/main.o \
./src/bin/topocalc/gshade/newlqh.o \
./src/bin/topocalc/gshade/trigtbl.o 

C_DEPS += \
./src/bin/topocalc/gshade/gshade.d \
./src/bin/topocalc/gshade/headers.d \
./src/bin/topocalc/gshade/main.d \
./src/bin/topocalc/gshade/newlqh.d \
./src/bin/topocalc/gshade/trigtbl.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/gshade/%.o: ../src/bin/topocalc/gshade/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


