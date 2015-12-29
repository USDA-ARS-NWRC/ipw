################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/basic/demux/demux.o \
../src/bin/basic/demux/demuxhdrs.o \
../src/bin/basic/demux/demuximg.o \
../src/bin/basic/demux/main.o 

C_SRCS += \
../src/bin/basic/demux/demux.c \
../src/bin/basic/demux/demuxhdrs.c \
../src/bin/basic/demux/demuximg.c \
../src/bin/basic/demux/main.c 

OBJS += \
./src/bin/basic/demux/demux.o \
./src/bin/basic/demux/demuxhdrs.o \
./src/bin/basic/demux/demuximg.o \
./src/bin/basic/demux/main.o 

C_DEPS += \
./src/bin/basic/demux/demux.d \
./src/bin/basic/demux/demuxhdrs.d \
./src/bin/basic/demux/demuximg.d \
./src/bin/basic/demux/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/basic/demux/%.o: ../src/bin/basic/demux/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


