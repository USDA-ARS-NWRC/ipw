################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/winh/_winhcheck.o \
../src/lib/libipw/winh/winhdup.o \
../src/lib/libipw/winh/winhfree.o \
../src/lib/libipw/winh/winhmake.o \
../src/lib/libipw/winh/winhread.o \
../src/lib/libipw/winh/winhwrite.o 

C_SRCS += \
../src/lib/libipw/winh/_winhcheck.c \
../src/lib/libipw/winh/winhdup.c \
../src/lib/libipw/winh/winhfree.c \
../src/lib/libipw/winh/winhmake.c \
../src/lib/libipw/winh/winhread.c \
../src/lib/libipw/winh/winhwrite.c 

OBJS += \
./src/lib/libipw/winh/_winhcheck.o \
./src/lib/libipw/winh/winhdup.o \
./src/lib/libipw/winh/winhfree.o \
./src/lib/libipw/winh/winhmake.o \
./src/lib/libipw/winh/winhread.o \
./src/lib/libipw/winh/winhwrite.o 

C_DEPS += \
./src/lib/libipw/winh/_winhcheck.d \
./src/lib/libipw/winh/winhdup.d \
./src/lib/libipw/winh/winhfree.d \
./src/lib/libipw/winh/winhmake.d \
./src/lib/libipw/winh/winhread.d \
./src/lib/libipw/winh/winhwrite.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/winh/%.o: ../src/lib/libipw/winh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


