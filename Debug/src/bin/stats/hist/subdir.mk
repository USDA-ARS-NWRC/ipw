################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/stats/hist/hist.o \
../src/bin/stats/hist/histx.o \
../src/bin/stats/hist/main.o 

C_SRCS += \
../src/bin/stats/hist/hist.c \
../src/bin/stats/hist/histx.c \
../src/bin/stats/hist/main.c 

OBJS += \
./src/bin/stats/hist/hist.o \
./src/bin/stats/hist/histx.o \
./src/bin/stats/hist/main.o 

C_DEPS += \
./src/bin/stats/hist/hist.d \
./src/bin/stats/hist/histx.d \
./src/bin/stats/hist/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/hist/%.o: ../src/bin/stats/hist/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


