################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/wedge/head1.o \
../src/bin/transform/wedge/head2.o \
../src/bin/transform/wedge/main.o \
../src/bin/transform/wedge/wedge.o 

C_SRCS += \
../src/bin/transform/wedge/head1.c \
../src/bin/transform/wedge/head2.c \
../src/bin/transform/wedge/main.c \
../src/bin/transform/wedge/wedge.c 

OBJS += \
./src/bin/transform/wedge/head1.o \
./src/bin/transform/wedge/head2.o \
./src/bin/transform/wedge/main.o \
./src/bin/transform/wedge/wedge.o 

C_DEPS += \
./src/bin/transform/wedge/head1.d \
./src/bin/transform/wedge/head2.d \
./src/bin/transform/wedge/main.d \
./src/bin/transform/wedge/wedge.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/wedge/%.o: ../src/bin/transform/wedge/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


