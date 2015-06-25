################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/export/ipw2sun/bitmap.c \
../src/bin/export/ipw2sun/main.c \
../src/bin/export/ipw2sun/sunhdr.c \
../src/bin/export/ipw2sun/sunras.c 

OBJS += \
./src/bin/export/ipw2sun/bitmap.o \
./src/bin/export/ipw2sun/main.o \
./src/bin/export/ipw2sun/sunhdr.o \
./src/bin/export/ipw2sun/sunras.o 

C_DEPS += \
./src/bin/export/ipw2sun/bitmap.d \
./src/bin/export/ipw2sun/main.d \
./src/bin/export/ipw2sun/sunhdr.d \
./src/bin/export/ipw2sun/sunras.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2sun/%.o: ../src/bin/export/ipw2sun/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


