################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/horh/_horhcheck.c \
../src/lib/libipw/horh/horhmake.c \
../src/lib/libipw/horh/horhread.c \
../src/lib/libipw/horh/horhwrite.c 

OBJS += \
./src/lib/libipw/horh/_horhcheck.o \
./src/lib/libipw/horh/horhmake.o \
./src/lib/libipw/horh/horhread.o \
./src/lib/libipw/horh/horhwrite.o 

C_DEPS += \
./src/lib/libipw/horh/_horhcheck.d \
./src/lib/libipw/horh/horhmake.d \
./src/lib/libipw/horh/horhread.d \
./src/lib/libipw/horh/horhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/horh/%.o: ../src/lib/libipw/horh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


