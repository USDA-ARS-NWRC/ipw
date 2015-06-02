################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.glb/gsunlight/gsunlight.c \
../src/bin/rad.glb/gsunlight/headers.c \
../src/bin/rad.glb/gsunlight/main.c \
../src/bin/rad.glb/gsunlight/newlqh.c 

OBJS += \
./src/bin/rad.glb/gsunlight/gsunlight.o \
./src/bin/rad.glb/gsunlight/headers.o \
./src/bin/rad.glb/gsunlight/main.o \
./src/bin/rad.glb/gsunlight/newlqh.o 

C_DEPS += \
./src/bin/rad.glb/gsunlight/gsunlight.d \
./src/bin/rad.glb/gsunlight/headers.d \
./src/bin/rad.glb/gsunlight/main.d \
./src/bin/rad.glb/gsunlight/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.glb/gsunlight/%.o: ../src/bin/rad.glb/gsunlight/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


