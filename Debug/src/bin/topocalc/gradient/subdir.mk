################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/topocalc/gradient/bihvalid.o \
../src/bin/topocalc/gradient/caspect.o \
../src/bin/topocalc/gradient/cslope.o \
../src/bin/topocalc/gradient/diffxy.o \
../src/bin/topocalc/gradient/fillends.o \
../src/bin/topocalc/gradient/fillstart.o \
../src/bin/topocalc/gradient/gradient.o \
../src/bin/topocalc/gradient/gradu.o \
../src/bin/topocalc/gradient/headers.o \
../src/bin/topocalc/gradient/main.o \
../src/bin/topocalc/gradient/newgeoh.o \
../src/bin/topocalc/gradient/newlqh.o \
../src/bin/topocalc/gradient/options.o \
../src/bin/topocalc/gradient/shuffle.o 

C_SRCS += \
../src/bin/topocalc/gradient/bihvalid.c \
../src/bin/topocalc/gradient/caspect.c \
../src/bin/topocalc/gradient/cslope.c \
../src/bin/topocalc/gradient/diffxy.c \
../src/bin/topocalc/gradient/fillends.c \
../src/bin/topocalc/gradient/fillstart.c \
../src/bin/topocalc/gradient/gradient.c \
../src/bin/topocalc/gradient/gradu.c \
../src/bin/topocalc/gradient/headers.c \
../src/bin/topocalc/gradient/main.c \
../src/bin/topocalc/gradient/newgeoh.c \
../src/bin/topocalc/gradient/newlqh.c \
../src/bin/topocalc/gradient/options.c \
../src/bin/topocalc/gradient/shuffle.c 

OBJS += \
./src/bin/topocalc/gradient/bihvalid.o \
./src/bin/topocalc/gradient/caspect.o \
./src/bin/topocalc/gradient/cslope.o \
./src/bin/topocalc/gradient/diffxy.o \
./src/bin/topocalc/gradient/fillends.o \
./src/bin/topocalc/gradient/fillstart.o \
./src/bin/topocalc/gradient/gradient.o \
./src/bin/topocalc/gradient/gradu.o \
./src/bin/topocalc/gradient/headers.o \
./src/bin/topocalc/gradient/main.o \
./src/bin/topocalc/gradient/newgeoh.o \
./src/bin/topocalc/gradient/newlqh.o \
./src/bin/topocalc/gradient/options.o \
./src/bin/topocalc/gradient/shuffle.o 

C_DEPS += \
./src/bin/topocalc/gradient/bihvalid.d \
./src/bin/topocalc/gradient/caspect.d \
./src/bin/topocalc/gradient/cslope.d \
./src/bin/topocalc/gradient/diffxy.d \
./src/bin/topocalc/gradient/fillends.d \
./src/bin/topocalc/gradient/fillstart.d \
./src/bin/topocalc/gradient/gradient.d \
./src/bin/topocalc/gradient/gradu.d \
./src/bin/topocalc/gradient/headers.d \
./src/bin/topocalc/gradient/main.d \
./src/bin/topocalc/gradient/newgeoh.d \
./src/bin/topocalc/gradient/newlqh.d \
./src/bin/topocalc/gradient/options.d \
./src/bin/topocalc/gradient/shuffle.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/gradient/%.o: ../src/bin/topocalc/gradient/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


