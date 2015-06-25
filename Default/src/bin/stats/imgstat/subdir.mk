################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/stats/imgstat/class_statf.c \
../src/bin/stats/imgstat/headers.c \
../src/bin/stats/imgstat/main.c \
../src/bin/stats/imgstat/output.c \
../src/bin/stats/imgstat/statf.c 

OBJS += \
./src/bin/stats/imgstat/class_statf.o \
./src/bin/stats/imgstat/headers.o \
./src/bin/stats/imgstat/main.o \
./src/bin/stats/imgstat/output.o \
./src/bin/stats/imgstat/statf.o 

C_DEPS += \
./src/bin/stats/imgstat/class_statf.d \
./src/bin/stats/imgstat/headers.d \
./src/bin/stats/imgstat/main.d \
./src/bin/stats/imgstat/output.d \
./src/bin/stats/imgstat/statf.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/imgstat/%.o: ../src/bin/stats/imgstat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


