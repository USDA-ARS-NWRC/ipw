################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/convolve/bufcycle.o \
../src/bin/transform/convolve/conv1d.o \
../src/bin/transform/convolve/convolve.o \
../src/bin/transform/convolve/do_kmap.o \
../src/bin/transform/convolve/getkernel.o \
../src/bin/transform/convolve/main.o \
../src/bin/transform/convolve/putkernel.o 

C_SRCS += \
../src/bin/transform/convolve/bufcycle.c \
../src/bin/transform/convolve/conv1d.c \
../src/bin/transform/convolve/convolve.c \
../src/bin/transform/convolve/do_kmap.c \
../src/bin/transform/convolve/getkernel.c \
../src/bin/transform/convolve/main.c \
../src/bin/transform/convolve/putkernel.c 

OBJS += \
./src/bin/transform/convolve/bufcycle.o \
./src/bin/transform/convolve/conv1d.o \
./src/bin/transform/convolve/convolve.o \
./src/bin/transform/convolve/do_kmap.o \
./src/bin/transform/convolve/getkernel.o \
./src/bin/transform/convolve/main.o \
./src/bin/transform/convolve/putkernel.o 

C_DEPS += \
./src/bin/transform/convolve/bufcycle.d \
./src/bin/transform/convolve/conv1d.d \
./src/bin/transform/convolve/convolve.d \
./src/bin/transform/convolve/do_kmap.d \
./src/bin/transform/convolve/getkernel.d \
./src/bin/transform/convolve/main.d \
./src/bin/transform/convolve/putkernel.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/convolve/%.o: ../src/bin/transform/convolve/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


