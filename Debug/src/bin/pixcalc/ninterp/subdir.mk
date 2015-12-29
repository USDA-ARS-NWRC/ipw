################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/ninterp/headers.o \
../src/bin/pixcalc/ninterp/interp_image.o \
../src/bin/pixcalc/ninterp/load_table.o \
../src/bin/pixcalc/ninterp/main.o \
../src/bin/pixcalc/ninterp/n_interp.o \
../src/bin/pixcalc/ninterp/vars.o 

C_SRCS += \
../src/bin/pixcalc/ninterp/headers.c \
../src/bin/pixcalc/ninterp/interp_image.c \
../src/bin/pixcalc/ninterp/load_table.c \
../src/bin/pixcalc/ninterp/main.c \
../src/bin/pixcalc/ninterp/n_interp.c \
../src/bin/pixcalc/ninterp/vars.c 

OBJS += \
./src/bin/pixcalc/ninterp/headers.o \
./src/bin/pixcalc/ninterp/interp_image.o \
./src/bin/pixcalc/ninterp/load_table.o \
./src/bin/pixcalc/ninterp/main.o \
./src/bin/pixcalc/ninterp/n_interp.o \
./src/bin/pixcalc/ninterp/vars.o 

C_DEPS += \
./src/bin/pixcalc/ninterp/headers.d \
./src/bin/pixcalc/ninterp/interp_image.d \
./src/bin/pixcalc/ninterp/load_table.d \
./src/bin/pixcalc/ninterp/main.d \
./src/bin/pixcalc/ninterp/n_interp.d \
./src/bin/pixcalc/ninterp/vars.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/ninterp/%.o: ../src/bin/pixcalc/ninterp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


