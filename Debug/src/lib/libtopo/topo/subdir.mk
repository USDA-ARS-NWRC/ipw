################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libtopo/topo/_cellstack.o \
../src/lib/libtopo/topo/_neighbor.o \
../src/lib/libtopo/topo/basin.o 

C_SRCS += \
../src/lib/libtopo/topo/_cellstack.c \
../src/lib/libtopo/topo/_neighbor.c \
../src/lib/libtopo/topo/basin.c 

OBJS += \
./src/lib/libtopo/topo/_cellstack.o \
./src/lib/libtopo/topo/_neighbor.o \
./src/lib/libtopo/topo/basin.o 

C_DEPS += \
./src/lib/libtopo/topo/_cellstack.d \
./src/lib/libtopo/topo/_neighbor.d \
./src/lib/libtopo/topo/basin.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libtopo/topo/%.o: ../src/lib/libtopo/topo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


