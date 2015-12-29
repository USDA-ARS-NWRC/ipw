################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.spat/mkprecip/calc_snow.o \
../src/bin/physmod.spat/mkprecip/globals.o \
../src/bin/physmod.spat/mkprecip/headers.o \
../src/bin/physmod.spat/mkprecip/main.o \
../src/bin/physmod.spat/mkprecip/mkprecip.o 

C_SRCS += \
../src/bin/physmod.spat/mkprecip/calc_snow.c \
../src/bin/physmod.spat/mkprecip/globals.c \
../src/bin/physmod.spat/mkprecip/headers.c \
../src/bin/physmod.spat/mkprecip/main.c \
../src/bin/physmod.spat/mkprecip/mkprecip.c 

OBJS += \
./src/bin/physmod.spat/mkprecip/calc_snow.o \
./src/bin/physmod.spat/mkprecip/globals.o \
./src/bin/physmod.spat/mkprecip/headers.o \
./src/bin/physmod.spat/mkprecip/main.o \
./src/bin/physmod.spat/mkprecip/mkprecip.o 

C_DEPS += \
./src/bin/physmod.spat/mkprecip/calc_snow.d \
./src/bin/physmod.spat/mkprecip/globals.d \
./src/bin/physmod.spat/mkprecip/headers.d \
./src/bin/physmod.spat/mkprecip/main.d \
./src/bin/physmod.spat/mkprecip/mkprecip.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/mkprecip/%.o: ../src/bin/physmod.spat/mkprecip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


