################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/geoh/_geohcheck.o \
../src/lib/libipw/geoh/geohdup.o \
../src/lib/libipw/geoh/geohfree.o \
../src/lib/libipw/geoh/geohmake.o \
../src/lib/libipw/geoh/geohread.o \
../src/lib/libipw/geoh/geohspace.o \
../src/lib/libipw/geoh/geohwrite.o 

C_SRCS += \
../src/lib/libipw/geoh/_geohcheck.c \
../src/lib/libipw/geoh/geohdup.c \
../src/lib/libipw/geoh/geohfree.c \
../src/lib/libipw/geoh/geohmake.c \
../src/lib/libipw/geoh/geohread.c \
../src/lib/libipw/geoh/geohspace.c \
../src/lib/libipw/geoh/geohwrite.c 

OBJS += \
./src/lib/libipw/geoh/_geohcheck.o \
./src/lib/libipw/geoh/geohdup.o \
./src/lib/libipw/geoh/geohfree.o \
./src/lib/libipw/geoh/geohmake.o \
./src/lib/libipw/geoh/geohread.o \
./src/lib/libipw/geoh/geohspace.o \
./src/lib/libipw/geoh/geohwrite.o 

C_DEPS += \
./src/lib/libipw/geoh/_geohcheck.d \
./src/lib/libipw/geoh/geohdup.d \
./src/lib/libipw/geoh/geohfree.d \
./src/lib/libipw/geoh/geohmake.d \
./src/lib/libipw/geoh/geohread.d \
./src/lib/libipw/geoh/geohspace.d \
./src/lib/libipw/geoh/geohwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/geoh/%.o: ../src/lib/libipw/geoh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


