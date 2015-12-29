################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/hdrio/_hdrioinit.o \
../src/lib/libipw/hdrio/_hdriovars.o \
../src/lib/libipw/hdrio/_hrrec.o \
../src/lib/libipw/hdrio/_hwrec.o \
../src/lib/libipw/hdrio/hcopy.o \
../src/lib/libipw/hdrio/hdralloc.o \
../src/lib/libipw/hdrio/hgetrec.o \
../src/lib/libipw/hdrio/hpass.o \
../src/lib/libipw/hdrio/hputrec.o \
../src/lib/libipw/hdrio/hrband.o \
../src/lib/libipw/hdrio/hrname.o \
../src/lib/libipw/hdrio/hrskip.o \
../src/lib/libipw/hdrio/hrvers.o \
../src/lib/libipw/hdrio/hwprmb.o 

C_SRCS += \
../src/lib/libipw/hdrio/_hdrioinit.c \
../src/lib/libipw/hdrio/_hdriovars.c \
../src/lib/libipw/hdrio/_hrrec.c \
../src/lib/libipw/hdrio/_hwrec.c \
../src/lib/libipw/hdrio/hcopy.c \
../src/lib/libipw/hdrio/hdralloc.c \
../src/lib/libipw/hdrio/hgetrec.c \
../src/lib/libipw/hdrio/hpass.c \
../src/lib/libipw/hdrio/hputrec.c \
../src/lib/libipw/hdrio/hrband.c \
../src/lib/libipw/hdrio/hrname.c \
../src/lib/libipw/hdrio/hrskip.c \
../src/lib/libipw/hdrio/hrvers.c \
../src/lib/libipw/hdrio/hwprmb.c 

OBJS += \
./src/lib/libipw/hdrio/_hdrioinit.o \
./src/lib/libipw/hdrio/_hdriovars.o \
./src/lib/libipw/hdrio/_hrrec.o \
./src/lib/libipw/hdrio/_hwrec.o \
./src/lib/libipw/hdrio/hcopy.o \
./src/lib/libipw/hdrio/hdralloc.o \
./src/lib/libipw/hdrio/hgetrec.o \
./src/lib/libipw/hdrio/hpass.o \
./src/lib/libipw/hdrio/hputrec.o \
./src/lib/libipw/hdrio/hrband.o \
./src/lib/libipw/hdrio/hrname.o \
./src/lib/libipw/hdrio/hrskip.o \
./src/lib/libipw/hdrio/hrvers.o \
./src/lib/libipw/hdrio/hwprmb.o 

C_DEPS += \
./src/lib/libipw/hdrio/_hdrioinit.d \
./src/lib/libipw/hdrio/_hdriovars.d \
./src/lib/libipw/hdrio/_hrrec.d \
./src/lib/libipw/hdrio/_hwrec.d \
./src/lib/libipw/hdrio/hcopy.d \
./src/lib/libipw/hdrio/hdralloc.d \
./src/lib/libipw/hdrio/hgetrec.d \
./src/lib/libipw/hdrio/hpass.d \
./src/lib/libipw/hdrio/hputrec.d \
./src/lib/libipw/hdrio/hrband.d \
./src/lib/libipw/hdrio/hrname.d \
./src/lib/libipw/hdrio/hrskip.d \
./src/lib/libipw/hdrio/hrvers.d \
./src/lib/libipw/hdrio/hwprmb.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/hdrio/%.o: ../src/lib/libipw/hdrio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


