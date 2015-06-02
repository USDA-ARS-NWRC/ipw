################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/transform/flip/fixgeoh.c \
../src/bin/transform/flip/fixorh.c \
../src/bin/transform/flip/fixwinh.c \
../src/bin/transform/flip/flip.c \
../src/bin/transform/flip/flip1d.c \
../src/bin/transform/flip/flip2d.c \
../src/bin/transform/flip/headers.c \
../src/bin/transform/flip/main.c \
../src/bin/transform/flip/parm.c \
../src/bin/transform/flip/revchar.c \
../src/bin/transform/flip/reverse.c \
../src/bin/transform/flip/revgen.c \
../src/bin/transform/flip/revlong.c \
../src/bin/transform/flip/revshort.c 

OBJS += \
./src/bin/transform/flip/fixgeoh.o \
./src/bin/transform/flip/fixorh.o \
./src/bin/transform/flip/fixwinh.o \
./src/bin/transform/flip/flip.o \
./src/bin/transform/flip/flip1d.o \
./src/bin/transform/flip/flip2d.o \
./src/bin/transform/flip/headers.o \
./src/bin/transform/flip/main.o \
./src/bin/transform/flip/parm.o \
./src/bin/transform/flip/revchar.o \
./src/bin/transform/flip/reverse.o \
./src/bin/transform/flip/revgen.o \
./src/bin/transform/flip/revlong.o \
./src/bin/transform/flip/revshort.o 

C_DEPS += \
./src/bin/transform/flip/fixgeoh.d \
./src/bin/transform/flip/fixorh.d \
./src/bin/transform/flip/fixwinh.d \
./src/bin/transform/flip/flip.d \
./src/bin/transform/flip/flip1d.d \
./src/bin/transform/flip/flip2d.d \
./src/bin/transform/flip/headers.d \
./src/bin/transform/flip/main.d \
./src/bin/transform/flip/parm.d \
./src/bin/transform/flip/revchar.d \
./src/bin/transform/flip/reverse.d \
./src/bin/transform/flip/revgen.d \
./src/bin/transform/flip/revlong.d \
./src/bin/transform/flip/revshort.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/flip/%.o: ../src/bin/transform/flip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


