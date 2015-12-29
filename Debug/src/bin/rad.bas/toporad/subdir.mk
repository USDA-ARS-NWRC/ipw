################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/rad.bas/toporad/head_final.o \
../src/bin/rad.bas/toporad/head_init.o \
../src/bin/rad.bas/toporad/main.o \
../src/bin/rad.bas/toporad/radcalc.o \
../src/bin/rad.bas/toporad/toporad.o 

C_SRCS += \
../src/bin/rad.bas/toporad/head_final.c \
../src/bin/rad.bas/toporad/head_init.c \
../src/bin/rad.bas/toporad/main.c \
../src/bin/rad.bas/toporad/radcalc.c \
../src/bin/rad.bas/toporad/toporad.c 

OBJS += \
./src/bin/rad.bas/toporad/head_final.o \
./src/bin/rad.bas/toporad/head_init.o \
./src/bin/rad.bas/toporad/main.o \
./src/bin/rad.bas/toporad/radcalc.o \
./src/bin/rad.bas/toporad/toporad.o 

C_DEPS += \
./src/bin/rad.bas/toporad/head_final.d \
./src/bin/rad.bas/toporad/head_init.d \
./src/bin/rad.bas/toporad/main.d \
./src/bin/rad.bas/toporad/radcalc.d \
./src/bin/rad.bas/toporad/toporad.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/toporad/%.o: ../src/bin/rad.bas/toporad/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


