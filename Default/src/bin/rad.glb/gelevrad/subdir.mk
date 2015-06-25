################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.glb/gelevrad/gelevrad.c \
../src/bin/rad.glb/gelevrad/headers.c \
../src/bin/rad.glb/gelevrad/main.c \
../src/bin/rad.glb/gelevrad/newlqh.c \
../src/bin/rad.glb/gelevrad/output.c 

OBJS += \
./src/bin/rad.glb/gelevrad/gelevrad.o \
./src/bin/rad.glb/gelevrad/headers.o \
./src/bin/rad.glb/gelevrad/main.o \
./src/bin/rad.glb/gelevrad/newlqh.o \
./src/bin/rad.glb/gelevrad/output.o 

C_DEPS += \
./src/bin/rad.glb/gelevrad/gelevrad.d \
./src/bin/rad.glb/gelevrad/headers.d \
./src/bin/rad.glb/gelevrad/main.d \
./src/bin/rad.glb/gelevrad/newlqh.d \
./src/bin/rad.glb/gelevrad/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.glb/gelevrad/%.o: ../src/bin/rad.glb/gelevrad/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


