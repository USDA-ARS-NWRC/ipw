################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/rad.bas/th_adj/adjust.o \
../src/bin/rad.bas/th_adj/head_final.o \
../src/bin/rad.bas/th_adj/head_init.o \
../src/bin/rad.bas/th_adj/main.o \
../src/bin/rad.bas/th_adj/th_adj.o 

C_SRCS += \
../src/bin/rad.bas/th_adj/adjust.c \
../src/bin/rad.bas/th_adj/head_final.c \
../src/bin/rad.bas/th_adj/head_init.c \
../src/bin/rad.bas/th_adj/main.c \
../src/bin/rad.bas/th_adj/th_adj.c 

OBJS += \
./src/bin/rad.bas/th_adj/adjust.o \
./src/bin/rad.bas/th_adj/head_final.o \
./src/bin/rad.bas/th_adj/head_init.o \
./src/bin/rad.bas/th_adj/main.o \
./src/bin/rad.bas/th_adj/th_adj.o 

C_DEPS += \
./src/bin/rad.bas/th_adj/adjust.d \
./src/bin/rad.bas/th_adj/head_final.d \
./src/bin/rad.bas/th_adj/head_init.d \
./src/bin/rad.bas/th_adj/main.d \
./src/bin/rad.bas/th_adj/th_adj.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/th_adj/%.o: ../src/bin/rad.bas/th_adj/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


