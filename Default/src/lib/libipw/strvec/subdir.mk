################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/strvec/_ok_sv.c \
../src/lib/libipw/strvec/addsv.c \
../src/lib/libipw/strvec/delsv.c \
../src/lib/libipw/strvec/dupsv.c \
../src/lib/libipw/strvec/freesv.c \
../src/lib/libipw/strvec/walksv.c 

OBJS += \
./src/lib/libipw/strvec/_ok_sv.o \
./src/lib/libipw/strvec/addsv.o \
./src/lib/libipw/strvec/delsv.o \
./src/lib/libipw/strvec/dupsv.o \
./src/lib/libipw/strvec/freesv.o \
./src/lib/libipw/strvec/walksv.o 

C_DEPS += \
./src/lib/libipw/strvec/_ok_sv.d \
./src/lib/libipw/strvec/addsv.d \
./src/lib/libipw/strvec/delsv.d \
./src/lib/libipw/strvec/dupsv.d \
./src/lib/libipw/strvec/freesv.d \
./src/lib/libipw/strvec/walksv.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/strvec/%.o: ../src/lib/libipw/strvec/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


