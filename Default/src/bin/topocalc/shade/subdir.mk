################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/shade/azmf.c \
../src/bin/topocalc/shade/buffers.c \
../src/bin/topocalc/shade/cosines.c \
../src/bin/topocalc/shade/data.c \
../src/bin/topocalc/shade/headers.c \
../src/bin/topocalc/shade/init_shade.c \
../src/bin/topocalc/shade/invzenf.c \
../src/bin/topocalc/shade/main.c \
../src/bin/topocalc/shade/newlqh.c \
../src/bin/topocalc/shade/newsunh.c \
../src/bin/topocalc/shade/shadetbl.c \
../src/bin/topocalc/shade/trigtbl.c \
../src/bin/topocalc/shade/zenf.c 

OBJS += \
./src/bin/topocalc/shade/azmf.o \
./src/bin/topocalc/shade/buffers.o \
./src/bin/topocalc/shade/cosines.o \
./src/bin/topocalc/shade/data.o \
./src/bin/topocalc/shade/headers.o \
./src/bin/topocalc/shade/init_shade.o \
./src/bin/topocalc/shade/invzenf.o \
./src/bin/topocalc/shade/main.o \
./src/bin/topocalc/shade/newlqh.o \
./src/bin/topocalc/shade/newsunh.o \
./src/bin/topocalc/shade/shadetbl.o \
./src/bin/topocalc/shade/trigtbl.o \
./src/bin/topocalc/shade/zenf.o 

C_DEPS += \
./src/bin/topocalc/shade/azmf.d \
./src/bin/topocalc/shade/buffers.d \
./src/bin/topocalc/shade/cosines.d \
./src/bin/topocalc/shade/data.d \
./src/bin/topocalc/shade/headers.d \
./src/bin/topocalc/shade/init_shade.d \
./src/bin/topocalc/shade/invzenf.d \
./src/bin/topocalc/shade/main.d \
./src/bin/topocalc/shade/newlqh.d \
./src/bin/topocalc/shade/newsunh.d \
./src/bin/topocalc/shade/shadetbl.d \
./src/bin/topocalc/shade/trigtbl.d \
./src/bin/topocalc/shade/zenf.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/shade/%.o: ../src/bin/topocalc/shade/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


