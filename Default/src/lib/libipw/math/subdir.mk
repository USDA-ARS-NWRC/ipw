################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/math/akcoef.c \
../src/lib/libipw/math/apfit.c \
../src/lib/libipw/math/dtoa.c \
../src/lib/libipw/math/hbit.c \
../src/lib/libipw/math/ipow2.c \
../src/lib/libipw/math/itoa.c \
../src/lib/libipw/math/ltoi.c \
../src/lib/libipw/math/msolve.c \
../src/lib/libipw/math/mvalloc.c \
../src/lib/libipw/math/ndig.c \
../src/lib/libipw/math/rotate.c \
../src/lib/libipw/math/seval.c \
../src/lib/libipw/math/splint.c \
../src/lib/libipw/math/zerobr.c 

OBJS += \
./src/lib/libipw/math/akcoef.o \
./src/lib/libipw/math/apfit.o \
./src/lib/libipw/math/dtoa.o \
./src/lib/libipw/math/hbit.o \
./src/lib/libipw/math/ipow2.o \
./src/lib/libipw/math/itoa.o \
./src/lib/libipw/math/ltoi.o \
./src/lib/libipw/math/msolve.o \
./src/lib/libipw/math/mvalloc.o \
./src/lib/libipw/math/ndig.o \
./src/lib/libipw/math/rotate.o \
./src/lib/libipw/math/seval.o \
./src/lib/libipw/math/splint.o \
./src/lib/libipw/math/zerobr.o 

C_DEPS += \
./src/lib/libipw/math/akcoef.d \
./src/lib/libipw/math/apfit.d \
./src/lib/libipw/math/dtoa.d \
./src/lib/libipw/math/hbit.d \
./src/lib/libipw/math/ipow2.d \
./src/lib/libipw/math/itoa.d \
./src/lib/libipw/math/ltoi.d \
./src/lib/libipw/math/msolve.d \
./src/lib/libipw/math/mvalloc.d \
./src/lib/libipw/math/ndig.d \
./src/lib/libipw/math/rotate.d \
./src/lib/libipw/math/seval.d \
./src/lib/libipw/math/splint.d \
./src/lib/libipw/math/zerobr.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/math/%.o: ../src/lib/libipw/math/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


