################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/isatvp/headers.c \
../src/bin/phys.spat/isatvp/isatvp.c \
../src/bin/phys.spat/isatvp/main.c \
../src/bin/phys.spat/isatvp/newlqh.c \
../src/bin/phys.spat/isatvp/output.c 

OBJS += \
./src/bin/phys.spat/isatvp/headers.o \
./src/bin/phys.spat/isatvp/isatvp.o \
./src/bin/phys.spat/isatvp/main.o \
./src/bin/phys.spat/isatvp/newlqh.o \
./src/bin/phys.spat/isatvp/output.o 

C_DEPS += \
./src/bin/phys.spat/isatvp/headers.d \
./src/bin/phys.spat/isatvp/isatvp.d \
./src/bin/phys.spat/isatvp/main.d \
./src/bin/phys.spat/isatvp/newlqh.d \
./src/bin/phys.spat/isatvp/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/isatvp/%.o: ../src/bin/phys.spat/isatvp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


