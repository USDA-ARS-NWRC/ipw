################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/sath/_sathcheck.c \
../src/lib/libipw/sath/sathdup.c \
../src/lib/libipw/sath/sathmake.c \
../src/lib/libipw/sath/sathread.c \
../src/lib/libipw/sath/sathwrite.c 

OBJS += \
./src/lib/libipw/sath/_sathcheck.o \
./src/lib/libipw/sath/sathdup.o \
./src/lib/libipw/sath/sathmake.o \
./src/lib/libipw/sath/sathread.o \
./src/lib/libipw/sath/sathwrite.o 

C_DEPS += \
./src/lib/libipw/sath/_sathcheck.d \
./src/lib/libipw/sath/sathdup.d \
./src/lib/libipw/sath/sathmake.d \
./src/lib/libipw/sath/sathread.d \
./src/lib/libipw/sath/sathwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/sath/%.o: ../src/lib/libipw/sath/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


