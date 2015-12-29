################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/stats/mstats/accum.o \
../src/bin/stats/mstats/headers.o \
../src/bin/stats/mstats/init.o \
../src/bin/stats/mstats/main.o \
../src/bin/stats/mstats/mcov.o \
../src/bin/stats/mstats/mstats.o \
../src/bin/stats/mstats/parm.o 

C_SRCS += \
../src/bin/stats/mstats/accum.c \
../src/bin/stats/mstats/headers.c \
../src/bin/stats/mstats/init.c \
../src/bin/stats/mstats/main.c \
../src/bin/stats/mstats/mcov.c \
../src/bin/stats/mstats/mstats.c \
../src/bin/stats/mstats/parm.c 

OBJS += \
./src/bin/stats/mstats/accum.o \
./src/bin/stats/mstats/headers.o \
./src/bin/stats/mstats/init.o \
./src/bin/stats/mstats/main.o \
./src/bin/stats/mstats/mcov.o \
./src/bin/stats/mstats/mstats.o \
./src/bin/stats/mstats/parm.o 

C_DEPS += \
./src/bin/stats/mstats/accum.d \
./src/bin/stats/mstats/headers.d \
./src/bin/stats/mstats/init.d \
./src/bin/stats/mstats/main.d \
./src/bin/stats/mstats/mcov.d \
./src/bin/stats/mstats/mstats.d \
./src/bin/stats/mstats/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/mstats/%.o: ../src/bin/stats/mstats/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


