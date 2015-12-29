################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/climate/hcnx/hcnx.o \
../src/bin/climate/hcnx/main.o \
../src/bin/climate/hcnx/output_dat.o \
../src/bin/climate/hcnx/xtract_data.o 

C_SRCS += \
../src/bin/climate/hcnx/hcnx.c \
../src/bin/climate/hcnx/main.c \
../src/bin/climate/hcnx/output_dat.c \
../src/bin/climate/hcnx/xtract_data.c 

OBJS += \
./src/bin/climate/hcnx/hcnx.o \
./src/bin/climate/hcnx/main.o \
./src/bin/climate/hcnx/output_dat.o \
./src/bin/climate/hcnx/xtract_data.o 

C_DEPS += \
./src/bin/climate/hcnx/hcnx.d \
./src/bin/climate/hcnx/main.d \
./src/bin/climate/hcnx/output_dat.d \
./src/bin/climate/hcnx/xtract_data.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/climate/hcnx/%.o: ../src/bin/climate/hcnx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


