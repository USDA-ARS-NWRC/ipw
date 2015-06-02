################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/error/_bug.c \
../src/lib/libipw/error/_error.c \
../src/lib/libipw/error/_errvars.c \
../src/lib/libipw/error/error.c \
../src/lib/libipw/error/syserr.c \
../src/lib/libipw/error/uferr.c \
../src/lib/libipw/error/usrerr.c \
../src/lib/libipw/error/warn.c 

OBJS += \
./src/lib/libipw/error/_bug.o \
./src/lib/libipw/error/_error.o \
./src/lib/libipw/error/_errvars.o \
./src/lib/libipw/error/error.o \
./src/lib/libipw/error/syserr.o \
./src/lib/libipw/error/uferr.o \
./src/lib/libipw/error/usrerr.o \
./src/lib/libipw/error/warn.o 

C_DEPS += \
./src/lib/libipw/error/_bug.d \
./src/lib/libipw/error/_error.d \
./src/lib/libipw/error/_errvars.d \
./src/lib/libipw/error/error.d \
./src/lib/libipw/error/syserr.d \
./src/lib/libipw/error/uferr.d \
./src/lib/libipw/error/usrerr.d \
./src/lib/libipw/error/warn.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/error/%.o: ../src/lib/libipw/error/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


