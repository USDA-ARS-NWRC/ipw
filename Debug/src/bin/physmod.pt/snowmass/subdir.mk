################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.pt/snowmass/main.o \
../src/bin/physmod.pt/snowmass/melt.o \
../src/bin/physmod.pt/snowmass/penman.o \
../src/bin/physmod.pt/snowmass/rainint.o \
../src/bin/physmod.pt/snowmass/snowint.o 

C_SRCS += \
../src/bin/physmod.pt/snowmass/main.c \
../src/bin/physmod.pt/snowmass/melt.c \
../src/bin/physmod.pt/snowmass/penman.c \
../src/bin/physmod.pt/snowmass/rainint.c \
../src/bin/physmod.pt/snowmass/snowint.c 

OBJS += \
./src/bin/physmod.pt/snowmass/main.o \
./src/bin/physmod.pt/snowmass/melt.o \
./src/bin/physmod.pt/snowmass/penman.o \
./src/bin/physmod.pt/snowmass/rainint.o \
./src/bin/physmod.pt/snowmass/snowint.o 

C_DEPS += \
./src/bin/physmod.pt/snowmass/main.d \
./src/bin/physmod.pt/snowmass/melt.d \
./src/bin/physmod.pt/snowmass/penman.d \
./src/bin/physmod.pt/snowmass/rainint.d \
./src/bin/physmod.pt/snowmass/snowint.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.pt/snowmass/%.o: ../src/bin/physmod.pt/snowmass/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


