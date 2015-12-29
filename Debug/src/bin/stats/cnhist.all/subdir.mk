################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/stats/cnhist.all/accum.o \
../src/bin/stats/cnhist.all/check_headers.o \
../src/bin/stats/cnhist.all/cnhist.o \
../src/bin/stats/cnhist.all/main.o \
../src/bin/stats/cnhist.all/read_headers.o 

C_SRCS += \
../src/bin/stats/cnhist.all/accum.c \
../src/bin/stats/cnhist.all/check_headers.c \
../src/bin/stats/cnhist.all/cnhist.c \
../src/bin/stats/cnhist.all/main.c \
../src/bin/stats/cnhist.all/read_headers.c 

OBJS += \
./src/bin/stats/cnhist.all/accum.o \
./src/bin/stats/cnhist.all/check_headers.o \
./src/bin/stats/cnhist.all/cnhist.o \
./src/bin/stats/cnhist.all/main.o \
./src/bin/stats/cnhist.all/read_headers.o 

C_DEPS += \
./src/bin/stats/cnhist.all/accum.d \
./src/bin/stats/cnhist.all/check_headers.d \
./src/bin/stats/cnhist.all/cnhist.d \
./src/bin/stats/cnhist.all/main.d \
./src/bin/stats/cnhist.all/read_headers.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/cnhist.all/%.o: ../src/bin/stats/cnhist.all/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


