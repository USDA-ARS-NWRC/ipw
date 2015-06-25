################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/numeric/seq/main.c \
../src/bin/numeric/seq/seqloop.c \
../src/bin/numeric/seq/square.c 

OBJS += \
./src/bin/numeric/seq/main.o \
./src/bin/numeric/seq/seqloop.o \
./src/bin/numeric/seq/square.o 

C_DEPS += \
./src/bin/numeric/seq/main.d \
./src/bin/numeric/seq/seqloop.d \
./src/bin/numeric/seq/square.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/numeric/seq/%.o: ../src/bin/numeric/seq/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


