################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/deltaz/deltaz.c \
../src/bin/topocalc/deltaz/find_neighbors.c \
../src/bin/topocalc/deltaz/get_points.c \
../src/bin/topocalc/deltaz/headers.c \
../src/bin/topocalc/deltaz/interp.c \
../src/bin/topocalc/deltaz/main.c 

OBJS += \
./src/bin/topocalc/deltaz/deltaz.o \
./src/bin/topocalc/deltaz/find_neighbors.o \
./src/bin/topocalc/deltaz/get_points.o \
./src/bin/topocalc/deltaz/headers.o \
./src/bin/topocalc/deltaz/interp.o \
./src/bin/topocalc/deltaz/main.o 

C_DEPS += \
./src/bin/topocalc/deltaz/deltaz.d \
./src/bin/topocalc/deltaz/find_neighbors.d \
./src/bin/topocalc/deltaz/get_points.d \
./src/bin/topocalc/deltaz/headers.d \
./src/bin/topocalc/deltaz/interp.d \
./src/bin/topocalc/deltaz/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/deltaz/%.o: ../src/bin/topocalc/deltaz/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


