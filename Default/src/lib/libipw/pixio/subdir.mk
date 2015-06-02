################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/pixio/_piocbinit.c \
../src/lib/libipw/pixio/_pixioinit.c \
../src/lib/libipw/pixio/_pixiovars.c \
../src/lib/libipw/pixio/pvread.c \
../src/lib/libipw/pixio/pvwrite.c \
../src/lib/libipw/pixio/pxclose.c 

OBJS += \
./src/lib/libipw/pixio/_piocbinit.o \
./src/lib/libipw/pixio/_pixioinit.o \
./src/lib/libipw/pixio/_pixiovars.o \
./src/lib/libipw/pixio/pvread.o \
./src/lib/libipw/pixio/pvwrite.o \
./src/lib/libipw/pixio/pxclose.o 

C_DEPS += \
./src/lib/libipw/pixio/_piocbinit.d \
./src/lib/libipw/pixio/_pixioinit.d \
./src/lib/libipw/pixio/_pixiovars.d \
./src/lib/libipw/pixio/pvread.d \
./src/lib/libipw/pixio/pvwrite.d \
./src/lib/libipw/pixio/pxclose.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/pixio/%.o: ../src/lib/libipw/pixio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


