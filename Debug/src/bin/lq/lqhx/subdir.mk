################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/lq/lqhx/headers.o \
../src/bin/lq/lqhx/lqhx.o \
../src/bin/lq/lqhx/main.o \
../src/bin/lq/lqhx/parm.o 

C_SRCS += \
../src/bin/lq/lqhx/headers.c \
../src/bin/lq/lqhx/lqhx.c \
../src/bin/lq/lqhx/main.c \
../src/bin/lq/lqhx/parm.c 

OBJS += \
./src/bin/lq/lqhx/headers.o \
./src/bin/lq/lqhx/lqhx.o \
./src/bin/lq/lqhx/main.o \
./src/bin/lq/lqhx/parm.o 

C_DEPS += \
./src/bin/lq/lqhx/headers.d \
./src/bin/lq/lqhx/lqhx.d \
./src/bin/lq/lqhx/main.d \
./src/bin/lq/lqhx/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/lq/lqhx/%.o: ../src/bin/lq/lqhx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


