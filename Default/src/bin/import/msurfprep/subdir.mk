################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/import/msurfprep/find_neibs.c \
../src/bin/import/msurfprep/geodist.c \
../src/bin/import/msurfprep/headers.c \
../src/bin/import/msurfprep/main.c \
../src/bin/import/msurfprep/read_points.c \
../src/bin/import/msurfprep/surfaceprep.c 

OBJS += \
./src/bin/import/msurfprep/find_neibs.o \
./src/bin/import/msurfprep/geodist.o \
./src/bin/import/msurfprep/headers.o \
./src/bin/import/msurfprep/main.o \
./src/bin/import/msurfprep/read_points.o \
./src/bin/import/msurfprep/surfaceprep.o 

C_DEPS += \
./src/bin/import/msurfprep/find_neibs.d \
./src/bin/import/msurfprep/geodist.d \
./src/bin/import/msurfprep/headers.d \
./src/bin/import/msurfprep/main.d \
./src/bin/import/msurfprep/read_points.d \
./src/bin/import/msurfprep/surfaceprep.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/msurfprep/%.o: ../src/bin/import/msurfprep/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


