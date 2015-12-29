################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/phys.spat/idewptp/headers.o \
../src/bin/phys.spat/idewptp/idewptp.o \
../src/bin/phys.spat/idewptp/main.o \
../src/bin/phys.spat/idewptp/newlqh.o 

C_SRCS += \
../src/bin/phys.spat/idewptp/headers.c \
../src/bin/phys.spat/idewptp/idewptp.c \
../src/bin/phys.spat/idewptp/main.c \
../src/bin/phys.spat/idewptp/newlqh.c 

OBJS += \
./src/bin/phys.spat/idewptp/headers.o \
./src/bin/phys.spat/idewptp/idewptp.o \
./src/bin/phys.spat/idewptp/main.o \
./src/bin/phys.spat/idewptp/newlqh.o 

C_DEPS += \
./src/bin/phys.spat/idewptp/headers.d \
./src/bin/phys.spat/idewptp/idewptp.d \
./src/bin/phys.spat/idewptp/main.d \
./src/bin/phys.spat/idewptp/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/idewptp/%.o: ../src/bin/phys.spat/idewptp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


