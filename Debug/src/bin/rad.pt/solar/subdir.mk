################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/rad.pt/solar/data.o \
../src/bin/rad.pt/solar/main.o \
../src/bin/rad.pt/solar/solar.o \
../src/bin/rad.pt/solar/solint.o \
../src/bin/rad.pt/solar/solval.o \
../src/bin/rad.pt/solar/spval1.o \
../src/bin/rad.pt/solar/updbl.o 

C_SRCS += \
../src/bin/rad.pt/solar/data.c \
../src/bin/rad.pt/solar/main.c \
../src/bin/rad.pt/solar/solar.c \
../src/bin/rad.pt/solar/solint.c \
../src/bin/rad.pt/solar/solval.c \
../src/bin/rad.pt/solar/spval1.c \
../src/bin/rad.pt/solar/updbl.c 

OBJS += \
./src/bin/rad.pt/solar/data.o \
./src/bin/rad.pt/solar/main.o \
./src/bin/rad.pt/solar/solar.o \
./src/bin/rad.pt/solar/solint.o \
./src/bin/rad.pt/solar/solval.o \
./src/bin/rad.pt/solar/spval1.o \
./src/bin/rad.pt/solar/updbl.o 

C_DEPS += \
./src/bin/rad.pt/solar/data.d \
./src/bin/rad.pt/solar/main.d \
./src/bin/rad.pt/solar/solar.d \
./src/bin/rad.pt/solar/solint.d \
./src/bin/rad.pt/solar/solval.d \
./src/bin/rad.pt/solar/spval1.d \
./src/bin/rad.pt/solar/updbl.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.pt/solar/%.o: ../src/bin/rad.pt/solar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


