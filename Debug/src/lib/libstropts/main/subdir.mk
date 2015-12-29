################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libstropts/main/_cvtoptarg.o \
../src/lib/libstropts/main/_getargs.o \
../src/lib/libstropts/main/_mainvars.o \
../src/lib/libstropts/main/ipwenter.o \
../src/lib/libstropts/main/opt_check.o \
../src/lib/libstropts/main/usage.o 

C_SRCS += \
../src/lib/libstropts/main/_cvtoptarg.c \
../src/lib/libstropts/main/_getargs.c \
../src/lib/libstropts/main/_mainvars.c \
../src/lib/libstropts/main/ipwenter.c \
../src/lib/libstropts/main/opt_check.c \
../src/lib/libstropts/main/usage.c 

OBJS += \
./src/lib/libstropts/main/_cvtoptarg.o \
./src/lib/libstropts/main/_getargs.o \
./src/lib/libstropts/main/_mainvars.o \
./src/lib/libstropts/main/ipwenter.o \
./src/lib/libstropts/main/opt_check.o \
./src/lib/libstropts/main/usage.o 

C_DEPS += \
./src/lib/libstropts/main/_cvtoptarg.d \
./src/lib/libstropts/main/_getargs.d \
./src/lib/libstropts/main/_mainvars.d \
./src/lib/libstropts/main/ipwenter.d \
./src/lib/libstropts/main/opt_check.d \
./src/lib/libstropts/main/usage.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libstropts/main/%.o: ../src/lib/libstropts/main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


