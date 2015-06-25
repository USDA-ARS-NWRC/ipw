################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/basic/mux/main.c \
../src/bin/basic/mux/mux.c \
../src/bin/basic/mux/muxhdrs.c \
../src/bin/basic/mux/muximg.c 

OBJS += \
./src/bin/basic/mux/main.o \
./src/bin/basic/mux/mux.o \
./src/bin/basic/mux/muxhdrs.o \
./src/bin/basic/mux/muximg.o 

C_DEPS += \
./src/bin/basic/mux/main.d \
./src/bin/basic/mux/mux.d \
./src/bin/basic/mux/muxhdrs.d \
./src/bin/basic/mux/muximg.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/basic/mux/%.o: ../src/bin/basic/mux/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


