################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/stats/ipwinterp/headers.c \
../src/bin/stats/ipwinterp/interp.c \
../src/bin/stats/ipwinterp/main.c \
../src/bin/stats/ipwinterp/read_par.c \
../src/bin/stats/ipwinterp/stat_anal.c 

OBJS += \
./src/bin/stats/ipwinterp/headers.o \
./src/bin/stats/ipwinterp/interp.o \
./src/bin/stats/ipwinterp/main.o \
./src/bin/stats/ipwinterp/read_par.o \
./src/bin/stats/ipwinterp/stat_anal.o 

C_DEPS += \
./src/bin/stats/ipwinterp/headers.d \
./src/bin/stats/ipwinterp/interp.d \
./src/bin/stats/ipwinterp/main.d \
./src/bin/stats/ipwinterp/read_par.d \
./src/bin/stats/ipwinterp/stat_anal.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/ipwinterp/%.o: ../src/bin/stats/ipwinterp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


