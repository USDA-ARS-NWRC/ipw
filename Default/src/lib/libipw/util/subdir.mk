################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/util/_quicksort.c \
../src/lib/libipw/util/allocnd.c \
../src/lib/libipw/util/ecalloc.c \
../src/lib/libipw/util/imgcopy.c \
../src/lib/libipw/util/no_tty.c \
../src/lib/libipw/util/rmlead.c \
../src/lib/libipw/util/rmtrail.c 

OBJS += \
./src/lib/libipw/util/_quicksort.o \
./src/lib/libipw/util/allocnd.o \
./src/lib/libipw/util/ecalloc.o \
./src/lib/libipw/util/imgcopy.o \
./src/lib/libipw/util/no_tty.o \
./src/lib/libipw/util/rmlead.o \
./src/lib/libipw/util/rmtrail.o 

C_DEPS += \
./src/lib/libipw/util/_quicksort.d \
./src/lib/libipw/util/allocnd.d \
./src/lib/libipw/util/ecalloc.d \
./src/lib/libipw/util/imgcopy.d \
./src/lib/libipw/util/no_tty.d \
./src/lib/libipw/util/rmlead.d \
./src/lib/libipw/util/rmtrail.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/util/%.o: ../src/lib/libipw/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


