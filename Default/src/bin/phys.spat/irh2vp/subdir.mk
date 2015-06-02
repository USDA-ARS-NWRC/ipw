################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/irh2vp/headers.c \
../src/bin/phys.spat/irh2vp/irh2vp.c \
../src/bin/phys.spat/irh2vp/main.c \
../src/bin/phys.spat/irh2vp/newlqh.c \
../src/bin/phys.spat/irh2vp/output.c 

OBJS += \
./src/bin/phys.spat/irh2vp/headers.o \
./src/bin/phys.spat/irh2vp/irh2vp.o \
./src/bin/phys.spat/irh2vp/main.o \
./src/bin/phys.spat/irh2vp/newlqh.o \
./src/bin/phys.spat/irh2vp/output.o 

C_DEPS += \
./src/bin/phys.spat/irh2vp/headers.d \
./src/bin/phys.spat/irh2vp/irh2vp.d \
./src/bin/phys.spat/irh2vp/main.d \
./src/bin/phys.spat/irh2vp/newlqh.d \
./src/bin/phys.spat/irh2vp/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/irh2vp/%.o: ../src/bin/phys.spat/irh2vp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


