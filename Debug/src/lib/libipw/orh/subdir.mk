################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/orh/_orhcheck.o \
../src/lib/libipw/orh/orhdup.o \
../src/lib/libipw/orh/orhfree.o \
../src/lib/libipw/orh/orhmake.o \
../src/lib/libipw/orh/orhread.o \
../src/lib/libipw/orh/orhwrite.o 

C_SRCS += \
../src/lib/libipw/orh/_orhcheck.c \
../src/lib/libipw/orh/orhdup.c \
../src/lib/libipw/orh/orhfree.c \
../src/lib/libipw/orh/orhmake.c \
../src/lib/libipw/orh/orhread.c \
../src/lib/libipw/orh/orhwrite.c 

OBJS += \
./src/lib/libipw/orh/_orhcheck.o \
./src/lib/libipw/orh/orhdup.o \
./src/lib/libipw/orh/orhfree.o \
./src/lib/libipw/orh/orhmake.o \
./src/lib/libipw/orh/orhread.o \
./src/lib/libipw/orh/orhwrite.o 

C_DEPS += \
./src/lib/libipw/orh/_orhcheck.d \
./src/lib/libipw/orh/orhdup.d \
./src/lib/libipw/orh/orhfree.d \
./src/lib/libipw/orh/orhmake.d \
./src/lib/libipw/orh/orhread.d \
./src/lib/libipw/orh/orhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/orh/%.o: ../src/lib/libipw/orh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


