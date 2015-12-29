################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libmodel/solar/_sunpos.o \
../src/lib/libmodel/solar/ephemeris.o \
../src/lib/libmodel/solar/sunangle.o \
../src/lib/libmodel/solar/sunpath.o \
../src/lib/libmodel/solar/sunrise.o \
../src/lib/libmodel/solar/sunset.o 

C_SRCS += \
../src/lib/libmodel/solar/_sunpos.c \
../src/lib/libmodel/solar/ephemeris.c \
../src/lib/libmodel/solar/sunangle.c \
../src/lib/libmodel/solar/sunpath.c \
../src/lib/libmodel/solar/sunrise.c \
../src/lib/libmodel/solar/sunset.c 

OBJS += \
./src/lib/libmodel/solar/_sunpos.o \
./src/lib/libmodel/solar/ephemeris.o \
./src/lib/libmodel/solar/sunangle.o \
./src/lib/libmodel/solar/sunpath.o \
./src/lib/libmodel/solar/sunrise.o \
./src/lib/libmodel/solar/sunset.o 

C_DEPS += \
./src/lib/libmodel/solar/_sunpos.d \
./src/lib/libmodel/solar/ephemeris.d \
./src/lib/libmodel/solar/sunangle.d \
./src/lib/libmodel/solar/sunpath.d \
./src/lib/libmodel/solar/sunrise.d \
./src/lib/libmodel/solar/sunset.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libmodel/solar/%.o: ../src/lib/libmodel/solar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


