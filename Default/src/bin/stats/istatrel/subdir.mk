################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/stats/istatrel/cstats.c \
../src/bin/stats/istatrel/headers.c \
../src/bin/stats/istatrel/ipwload.c \
../src/bin/stats/istatrel/istatrel.c \
../src/bin/stats/istatrel/main.c \
../src/bin/stats/istatrel/stat_init.c 

OBJS += \
./src/bin/stats/istatrel/cstats.o \
./src/bin/stats/istatrel/headers.o \
./src/bin/stats/istatrel/ipwload.o \
./src/bin/stats/istatrel/istatrel.o \
./src/bin/stats/istatrel/main.o \
./src/bin/stats/istatrel/stat_init.o 

C_DEPS += \
./src/bin/stats/istatrel/cstats.d \
./src/bin/stats/istatrel/headers.d \
./src/bin/stats/istatrel/ipwload.d \
./src/bin/stats/istatrel/istatrel.d \
./src/bin/stats/istatrel/main.d \
./src/bin/stats/istatrel/stat_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/istatrel/%.o: ../src/bin/stats/istatrel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


