################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.glb/gtoporad/gtoporad.c \
../src/bin/rad.glb/gtoporad/head_final.c \
../src/bin/rad.glb/gtoporad/head_init.c \
../src/bin/rad.glb/gtoporad/main.c \
../src/bin/rad.glb/gtoporad/radcalc.c 

OBJS += \
./src/bin/rad.glb/gtoporad/gtoporad.o \
./src/bin/rad.glb/gtoporad/head_final.o \
./src/bin/rad.glb/gtoporad/head_init.o \
./src/bin/rad.glb/gtoporad/main.o \
./src/bin/rad.glb/gtoporad/radcalc.o 

C_DEPS += \
./src/bin/rad.glb/gtoporad/gtoporad.d \
./src/bin/rad.glb/gtoporad/head_final.d \
./src/bin/rad.glb/gtoporad/head_init.d \
./src/bin/rad.glb/gtoporad/main.d \
./src/bin/rad.glb/gtoporad/radcalc.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.glb/gtoporad/%.o: ../src/bin/rad.glb/gtoporad/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


