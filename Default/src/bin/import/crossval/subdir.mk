################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/import/crossval/crossval.c \
../src/bin/import/crossval/find_neighbors.c \
../src/bin/import/crossval/get_points.c \
../src/bin/import/crossval/headers.c \
../src/bin/import/crossval/interp.c \
../src/bin/import/crossval/main.c 

OBJS += \
./src/bin/import/crossval/crossval.o \
./src/bin/import/crossval/find_neighbors.o \
./src/bin/import/crossval/get_points.o \
./src/bin/import/crossval/headers.o \
./src/bin/import/crossval/interp.o \
./src/bin/import/crossval/main.o 

C_DEPS += \
./src/bin/import/crossval/crossval.d \
./src/bin/import/crossval/find_neighbors.d \
./src/bin/import/crossval/get_points.d \
./src/bin/import/crossval/headers.d \
./src/bin/import/crossval/interp.d \
./src/bin/import/crossval/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/crossval/%.o: ../src/bin/import/crossval/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


