################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/transform/dither/dither.o \
../src/bin/transform/dither/dithimg.o \
../src/bin/transform/dither/dm_val.o \
../src/bin/transform/dither/main.o \
../src/bin/transform/dither/mk_dm.o 

C_SRCS += \
../src/bin/transform/dither/dither.c \
../src/bin/transform/dither/dithimg.c \
../src/bin/transform/dither/dm_val.c \
../src/bin/transform/dither/main.c \
../src/bin/transform/dither/mk_dm.c 

OBJS += \
./src/bin/transform/dither/dither.o \
./src/bin/transform/dither/dithimg.o \
./src/bin/transform/dither/dm_val.o \
./src/bin/transform/dither/main.o \
./src/bin/transform/dither/mk_dm.o 

C_DEPS += \
./src/bin/transform/dither/dither.d \
./src/bin/transform/dither/dithimg.d \
./src/bin/transform/dither/dm_val.d \
./src/bin/transform/dither/main.d \
./src/bin/transform/dither/mk_dm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/dither/%.o: ../src/bin/transform/dither/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


