################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/transform/zoom/fixhdrs.c \
../src/bin/transform/zoom/headers.c \
../src/bin/transform/zoom/main.c \
../src/bin/transform/zoom/parm.c \
../src/bin/transform/zoom/replicate.c \
../src/bin/transform/zoom/subsamp.c \
../src/bin/transform/zoom/zoom.c 

OBJS += \
./src/bin/transform/zoom/fixhdrs.o \
./src/bin/transform/zoom/headers.o \
./src/bin/transform/zoom/main.o \
./src/bin/transform/zoom/parm.o \
./src/bin/transform/zoom/replicate.o \
./src/bin/transform/zoom/subsamp.o \
./src/bin/transform/zoom/zoom.o 

C_DEPS += \
./src/bin/transform/zoom/fixhdrs.d \
./src/bin/transform/zoom/headers.d \
./src/bin/transform/zoom/main.d \
./src/bin/transform/zoom/parm.d \
./src/bin/transform/zoom/replicate.d \
./src/bin/transform/zoom/subsamp.d \
./src/bin/transform/zoom/zoom.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/zoom/%.o: ../src/bin/transform/zoom/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


