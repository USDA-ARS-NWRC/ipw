################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.pt/sunlight/krontime.c \
../src/bin/rad.pt/sunlight/main.c \
../src/bin/rad.pt/sunlight/sunlight_opts.c \
../src/bin/rad.pt/sunlight/vqk15.c \
../src/bin/rad.pt/sunlight/vqk21.c \
../src/bin/rad.pt/sunlight/vqkagn.c 

OBJS += \
./src/bin/rad.pt/sunlight/krontime.o \
./src/bin/rad.pt/sunlight/main.o \
./src/bin/rad.pt/sunlight/sunlight_opts.o \
./src/bin/rad.pt/sunlight/vqk15.o \
./src/bin/rad.pt/sunlight/vqk21.o \
./src/bin/rad.pt/sunlight/vqkagn.o 

C_DEPS += \
./src/bin/rad.pt/sunlight/krontime.d \
./src/bin/rad.pt/sunlight/main.d \
./src/bin/rad.pt/sunlight/sunlight_opts.d \
./src/bin/rad.pt/sunlight/vqk15.d \
./src/bin/rad.pt/sunlight/vqk21.d \
./src/bin/rad.pt/sunlight/vqkagn.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.pt/sunlight/%.o: ../src/bin/rad.pt/sunlight/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


