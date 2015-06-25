################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/ialbedo/growth.c \
../src/bin/phys.spat/ialbedo/headers.c \
../src/bin/phys.spat/ialbedo/ialbedo.c \
../src/bin/phys.spat/ialbedo/main.c \
../src/bin/phys.spat/ialbedo/newlqh.c 

OBJS += \
./src/bin/phys.spat/ialbedo/growth.o \
./src/bin/phys.spat/ialbedo/headers.o \
./src/bin/phys.spat/ialbedo/ialbedo.o \
./src/bin/phys.spat/ialbedo/main.o \
./src/bin/phys.spat/ialbedo/newlqh.o 

C_DEPS += \
./src/bin/phys.spat/ialbedo/growth.d \
./src/bin/phys.spat/ialbedo/headers.d \
./src/bin/phys.spat/ialbedo/ialbedo.d \
./src/bin/phys.spat/ialbedo/main.d \
./src/bin/phys.spat/ialbedo/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/ialbedo/%.o: ../src/bin/phys.spat/ialbedo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


