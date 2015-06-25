################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/crh/_crharrays.c \
../src/lib/libipw/crh/_crhcheck.c \
../src/lib/libipw/crh/_crhinit.c \
../src/lib/libipw/crh/_crhvars.c \
../src/lib/libipw/crh/crh.c \
../src/lib/libipw/crh/crhdup.c \
../src/lib/libipw/crh/crhmake.c \
../src/lib/libipw/crh/crhread.c \
../src/lib/libipw/crh/crhsort.c \
../src/lib/libipw/crh/crhwrite.c 

OBJS += \
./src/lib/libipw/crh/_crharrays.o \
./src/lib/libipw/crh/_crhcheck.o \
./src/lib/libipw/crh/_crhinit.o \
./src/lib/libipw/crh/_crhvars.o \
./src/lib/libipw/crh/crh.o \
./src/lib/libipw/crh/crhdup.o \
./src/lib/libipw/crh/crhmake.o \
./src/lib/libipw/crh/crhread.o \
./src/lib/libipw/crh/crhsort.o \
./src/lib/libipw/crh/crhwrite.o 

C_DEPS += \
./src/lib/libipw/crh/_crharrays.d \
./src/lib/libipw/crh/_crhcheck.d \
./src/lib/libipw/crh/_crhinit.d \
./src/lib/libipw/crh/_crhvars.d \
./src/lib/libipw/crh/crh.d \
./src/lib/libipw/crh/crhdup.d \
./src/lib/libipw/crh/crhmake.d \
./src/lib/libipw/crh/crhread.d \
./src/lib/libipw/crh/crhsort.d \
./src/lib/libipw/crh/crhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/crh/%.o: ../src/lib/libipw/crh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


