################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/sunh/_sunhcheck.o \
../src/lib/libipw/sunh/sunhmake.o \
../src/lib/libipw/sunh/sunhread.o \
../src/lib/libipw/sunh/sunhwrite.o 

C_SRCS += \
../src/lib/libipw/sunh/_sunhcheck.c \
../src/lib/libipw/sunh/sunhmake.c \
../src/lib/libipw/sunh/sunhread.c \
../src/lib/libipw/sunh/sunhwrite.c 

OBJS += \
./src/lib/libipw/sunh/_sunhcheck.o \
./src/lib/libipw/sunh/sunhmake.o \
./src/lib/libipw/sunh/sunhread.o \
./src/lib/libipw/sunh/sunhwrite.o 

C_DEPS += \
./src/lib/libipw/sunh/_sunhcheck.d \
./src/lib/libipw/sunh/sunhmake.d \
./src/lib/libipw/sunh/sunhread.d \
./src/lib/libipw/sunh/sunhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/sunh/%.o: ../src/lib/libipw/sunh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


