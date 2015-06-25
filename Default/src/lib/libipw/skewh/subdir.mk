################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/skewh/_skewhcheck.c \
../src/lib/libipw/skewh/skewhmake.c \
../src/lib/libipw/skewh/skewhread.c \
../src/lib/libipw/skewh/skewhwrite.c 

OBJS += \
./src/lib/libipw/skewh/_skewhcheck.o \
./src/lib/libipw/skewh/skewhmake.o \
./src/lib/libipw/skewh/skewhread.o \
./src/lib/libipw/skewh/skewhwrite.o 

C_DEPS += \
./src/lib/libipw/skewh/_skewhcheck.d \
./src/lib/libipw/skewh/skewhmake.d \
./src/lib/libipw/skewh/skewhread.d \
./src/lib/libipw/skewh/skewhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/skewh/%.o: ../src/lib/libipw/skewh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


