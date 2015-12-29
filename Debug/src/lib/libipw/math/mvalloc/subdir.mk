################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/math/mvalloc/mvalloc.c \
../src/lib/libipw/math/mvalloc/test1.c \
../src/lib/libipw/math/mvalloc/test2.c \
../src/lib/libipw/math/mvalloc/test3.c \
../src/lib/libipw/math/mvalloc/test4.c \
../src/lib/libipw/math/mvalloc/test5.c 

OBJS += \
./src/lib/libipw/math/mvalloc/mvalloc.o \
./src/lib/libipw/math/mvalloc/test1.o \
./src/lib/libipw/math/mvalloc/test2.o \
./src/lib/libipw/math/mvalloc/test3.o \
./src/lib/libipw/math/mvalloc/test4.o \
./src/lib/libipw/math/mvalloc/test5.o 

C_DEPS += \
./src/lib/libipw/math/mvalloc/mvalloc.d \
./src/lib/libipw/math/mvalloc/test1.d \
./src/lib/libipw/math/mvalloc/test2.d \
./src/lib/libipw/math/mvalloc/test3.d \
./src/lib/libipw/math/mvalloc/test4.d \
./src/lib/libipw/math/mvalloc/test5.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/math/mvalloc/%.o: ../src/lib/libipw/math/mvalloc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


