################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/lqh/_lqharrays.c \
../src/lib/libipw/lqh/_lqhcheck.c \
../src/lib/libipw/lqh/_lqhfree.c \
../src/lib/libipw/lqh/_lqhinit.c \
../src/lib/libipw/lqh/_lqhinterp.c \
../src/lib/libipw/lqh/_lqhvars.c \
../src/lib/libipw/lqh/lqh.c \
../src/lib/libipw/lqh/lqhdup.c \
../src/lib/libipw/lqh/lqhmake.c \
../src/lib/libipw/lqh/lqhread.c \
../src/lib/libipw/lqh/lqhwrite.c 

OBJS += \
./src/lib/libipw/lqh/_lqharrays.o \
./src/lib/libipw/lqh/_lqhcheck.o \
./src/lib/libipw/lqh/_lqhfree.o \
./src/lib/libipw/lqh/_lqhinit.o \
./src/lib/libipw/lqh/_lqhinterp.o \
./src/lib/libipw/lqh/_lqhvars.o \
./src/lib/libipw/lqh/lqh.o \
./src/lib/libipw/lqh/lqhdup.o \
./src/lib/libipw/lqh/lqhmake.o \
./src/lib/libipw/lqh/lqhread.o \
./src/lib/libipw/lqh/lqhwrite.o 

C_DEPS += \
./src/lib/libipw/lqh/_lqharrays.d \
./src/lib/libipw/lqh/_lqhcheck.d \
./src/lib/libipw/lqh/_lqhfree.d \
./src/lib/libipw/lqh/_lqhinit.d \
./src/lib/libipw/lqh/_lqhinterp.d \
./src/lib/libipw/lqh/_lqhvars.d \
./src/lib/libipw/lqh/lqh.d \
./src/lib/libipw/lqh/lqhdup.d \
./src/lib/libipw/lqh/lqhmake.d \
./src/lib/libipw/lqh/lqhread.d \
./src/lib/libipw/lqh/lqhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/lqh/%.o: ../src/lib/libipw/lqh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


