################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/ggradient/fillends.c \
../src/bin/topocalc/ggradient/fillstart.c \
../src/bin/topocalc/ggradient/gcp_dist.c \
../src/bin/topocalc/ggradient/ggradient.c \
../src/bin/topocalc/ggradient/gradu.c \
../src/bin/topocalc/ggradient/headers.c \
../src/bin/topocalc/ggradient/main.c \
../src/bin/topocalc/ggradient/newlqh.c \
../src/bin/topocalc/ggradient/shuffle.c 

OBJS += \
./src/bin/topocalc/ggradient/fillends.o \
./src/bin/topocalc/ggradient/fillstart.o \
./src/bin/topocalc/ggradient/gcp_dist.o \
./src/bin/topocalc/ggradient/ggradient.o \
./src/bin/topocalc/ggradient/gradu.o \
./src/bin/topocalc/ggradient/headers.o \
./src/bin/topocalc/ggradient/main.o \
./src/bin/topocalc/ggradient/newlqh.o \
./src/bin/topocalc/ggradient/shuffle.o 

C_DEPS += \
./src/bin/topocalc/ggradient/fillends.d \
./src/bin/topocalc/ggradient/fillstart.d \
./src/bin/topocalc/ggradient/gcp_dist.d \
./src/bin/topocalc/ggradient/ggradient.d \
./src/bin/topocalc/ggradient/gradu.d \
./src/bin/topocalc/ggradient/headers.d \
./src/bin/topocalc/ggradient/main.d \
./src/bin/topocalc/ggradient/newlqh.d \
./src/bin/topocalc/ggradient/shuffle.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/ggradient/%.o: ../src/bin/topocalc/ggradient/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


