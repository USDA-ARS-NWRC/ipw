################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/fpio/_fpiocbinit.c \
../src/lib/libipw/fpio/_fpioinit.c \
../src/lib/libipw/fpio/_fpiovars.c \
../src/lib/libipw/fpio/fpclose.c \
../src/lib/libipw/fpio/fpfmax.c \
../src/lib/libipw/fpio/fpfmin.c \
../src/lib/libipw/fpio/fphdrs.c \
../src/lib/libipw/fpio/fpmap.c \
../src/lib/libipw/fpio/fpmaplen.c \
../src/lib/libipw/fpio/fpvread.c \
../src/lib/libipw/fpio/fpvwrite.c \
../src/lib/libipw/fpio/mnxfp.c 

OBJS += \
./src/lib/libipw/fpio/_fpiocbinit.o \
./src/lib/libipw/fpio/_fpioinit.o \
./src/lib/libipw/fpio/_fpiovars.o \
./src/lib/libipw/fpio/fpclose.o \
./src/lib/libipw/fpio/fpfmax.o \
./src/lib/libipw/fpio/fpfmin.o \
./src/lib/libipw/fpio/fphdrs.o \
./src/lib/libipw/fpio/fpmap.o \
./src/lib/libipw/fpio/fpmaplen.o \
./src/lib/libipw/fpio/fpvread.o \
./src/lib/libipw/fpio/fpvwrite.o \
./src/lib/libipw/fpio/mnxfp.o 

C_DEPS += \
./src/lib/libipw/fpio/_fpiocbinit.d \
./src/lib/libipw/fpio/_fpioinit.d \
./src/lib/libipw/fpio/_fpiovars.d \
./src/lib/libipw/fpio/fpclose.d \
./src/lib/libipw/fpio/fpfmax.d \
./src/lib/libipw/fpio/fpfmin.d \
./src/lib/libipw/fpio/fphdrs.d \
./src/lib/libipw/fpio/fpmap.d \
./src/lib/libipw/fpio/fpmaplen.d \
./src/lib/libipw/fpio/fpvread.d \
./src/lib/libipw/fpio/fpvwrite.d \
./src/lib/libipw/fpio/mnxfp.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/fpio/%.o: ../src/lib/libipw/fpio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


