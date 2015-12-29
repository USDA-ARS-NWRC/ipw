################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/proj/reproj/bilinear.o \
../src/bin/proj/reproj/cubic.o \
../src/bin/proj/reproj/headers.o \
../src/bin/proj/reproj/main.o \
../src/bin/proj/reproj/nearest.o \
../src/bin/proj/reproj/nothing.o \
../src/bin/proj/reproj/reproj.o 

C_SRCS += \
../src/bin/proj/reproj/bilinear.c \
../src/bin/proj/reproj/cubic.c \
../src/bin/proj/reproj/headers.c \
../src/bin/proj/reproj/main.c \
../src/bin/proj/reproj/nearest.c \
../src/bin/proj/reproj/nothing.c \
../src/bin/proj/reproj/reproj.c 

OBJS += \
./src/bin/proj/reproj/bilinear.o \
./src/bin/proj/reproj/cubic.o \
./src/bin/proj/reproj/headers.o \
./src/bin/proj/reproj/main.o \
./src/bin/proj/reproj/nearest.o \
./src/bin/proj/reproj/nothing.o \
./src/bin/proj/reproj/reproj.o 

C_DEPS += \
./src/bin/proj/reproj/bilinear.d \
./src/bin/proj/reproj/cubic.d \
./src/bin/proj/reproj/headers.d \
./src/bin/proj/reproj/main.d \
./src/bin/proj/reproj/nearest.d \
./src/bin/proj/reproj/nothing.d \
./src/bin/proj/reproj/reproj.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/proj/reproj/%.o: ../src/bin/proj/reproj/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


