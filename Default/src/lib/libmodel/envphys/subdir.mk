################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libmodel/envphys/_bdinit.c \
../src/lib/libmodel/envphys/bevap.c \
../src/lib/libmodel/envphys/bowen.c \
../src/lib/libmodel/envphys/budyer.c \
../src/lib/libmodel/envphys/budyer2.c \
../src/lib/libmodel/envphys/dew_point.c \
../src/lib/libmodel/envphys/efcon.c \
../src/lib/libmodel/envphys/evap.c \
../src/lib/libmodel/envphys/heat_stor.c \
../src/lib/libmodel/envphys/hle1.c \
../src/lib/libmodel/envphys/psychrom.c \
../src/lib/libmodel/envphys/ri_no.c \
../src/lib/libmodel/envphys/sati.c \
../src/lib/libmodel/envphys/satw.c \
../src/lib/libmodel/envphys/ssxfr.c \
../src/lib/libmodel/envphys/wetbulb.c 

OBJS += \
./src/lib/libmodel/envphys/_bdinit.o \
./src/lib/libmodel/envphys/bevap.o \
./src/lib/libmodel/envphys/bowen.o \
./src/lib/libmodel/envphys/budyer.o \
./src/lib/libmodel/envphys/budyer2.o \
./src/lib/libmodel/envphys/dew_point.o \
./src/lib/libmodel/envphys/efcon.o \
./src/lib/libmodel/envphys/evap.o \
./src/lib/libmodel/envphys/heat_stor.o \
./src/lib/libmodel/envphys/hle1.o \
./src/lib/libmodel/envphys/psychrom.o \
./src/lib/libmodel/envphys/ri_no.o \
./src/lib/libmodel/envphys/sati.o \
./src/lib/libmodel/envphys/satw.o \
./src/lib/libmodel/envphys/ssxfr.o \
./src/lib/libmodel/envphys/wetbulb.o 

C_DEPS += \
./src/lib/libmodel/envphys/_bdinit.d \
./src/lib/libmodel/envphys/bevap.d \
./src/lib/libmodel/envphys/bowen.d \
./src/lib/libmodel/envphys/budyer.d \
./src/lib/libmodel/envphys/budyer2.d \
./src/lib/libmodel/envphys/dew_point.d \
./src/lib/libmodel/envphys/efcon.d \
./src/lib/libmodel/envphys/evap.d \
./src/lib/libmodel/envphys/heat_stor.d \
./src/lib/libmodel/envphys/hle1.d \
./src/lib/libmodel/envphys/psychrom.d \
./src/lib/libmodel/envphys/ri_no.d \
./src/lib/libmodel/envphys/sati.d \
./src/lib/libmodel/envphys/satw.d \
./src/lib/libmodel/envphys/ssxfr.d \
./src/lib/libmodel/envphys/wetbulb.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libmodel/envphys/%.o: ../src/lib/libmodel/envphys/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


