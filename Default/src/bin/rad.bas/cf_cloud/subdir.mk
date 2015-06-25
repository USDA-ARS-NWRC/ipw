################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.bas/cf_cloud/cf_cloud.c \
../src/bin/rad.bas/cf_cloud/cloudcalc.c \
../src/bin/rad.bas/cf_cloud/head_final.c \
../src/bin/rad.bas/cf_cloud/head_init.c \
../src/bin/rad.bas/cf_cloud/main.c 

OBJS += \
./src/bin/rad.bas/cf_cloud/cf_cloud.o \
./src/bin/rad.bas/cf_cloud/cloudcalc.o \
./src/bin/rad.bas/cf_cloud/head_final.o \
./src/bin/rad.bas/cf_cloud/head_init.o \
./src/bin/rad.bas/cf_cloud/main.o 

C_DEPS += \
./src/bin/rad.bas/cf_cloud/cf_cloud.d \
./src/bin/rad.bas/cf_cloud/cloudcalc.d \
./src/bin/rad.bas/cf_cloud/head_final.d \
./src/bin/rad.bas/cf_cloud/head_init.d \
./src/bin/rad.bas/cf_cloud/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/cf_cloud/%.o: ../src/bin/rad.bas/cf_cloud/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


