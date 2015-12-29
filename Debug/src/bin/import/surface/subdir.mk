################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/import/surface/find_neighbors.o \
../src/bin/import/surface/get_points.o \
../src/bin/import/surface/headers.o \
../src/bin/import/surface/interp.o \
../src/bin/import/surface/main.o \
../src/bin/import/surface/surface.o 

C_SRCS += \
../src/bin/import/surface/find_neighbors.c \
../src/bin/import/surface/get_points.c \
../src/bin/import/surface/headers.c \
../src/bin/import/surface/interp.c \
../src/bin/import/surface/main.c \
../src/bin/import/surface/surface.c 

OBJS += \
./src/bin/import/surface/find_neighbors.o \
./src/bin/import/surface/get_points.o \
./src/bin/import/surface/headers.o \
./src/bin/import/surface/interp.o \
./src/bin/import/surface/main.o \
./src/bin/import/surface/surface.o 

C_DEPS += \
./src/bin/import/surface/find_neighbors.d \
./src/bin/import/surface/get_points.d \
./src/bin/import/surface/headers.d \
./src/bin/import/surface/interp.d \
./src/bin/import/surface/main.d \
./src/bin/import/surface/surface.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/surface/%.o: ../src/bin/import/surface/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


