################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/main/_cmdline.o \
../src/lib/libipw/main/_cvtoptarg.o \
../src/lib/libipw/main/_getargs.o \
../src/lib/libipw/main/_mainvars.o \
../src/lib/libipw/main/ipwenter.o \
../src/lib/libipw/main/ipwexit.o \
../src/lib/libipw/main/opt_check.o \
../src/lib/libipw/main/usage.o 

C_SRCS += \
../src/lib/libipw/main/_cmdline.c \
../src/lib/libipw/main/_cvtoptarg.c \
../src/lib/libipw/main/_getargs.c \
../src/lib/libipw/main/_mainvars.c \
../src/lib/libipw/main/ipwenter.c \
../src/lib/libipw/main/ipwexit.c \
../src/lib/libipw/main/opt_check.c \
../src/lib/libipw/main/usage.c 

OBJS += \
./src/lib/libipw/main/_cmdline.o \
./src/lib/libipw/main/_cvtoptarg.o \
./src/lib/libipw/main/_getargs.o \
./src/lib/libipw/main/_mainvars.o \
./src/lib/libipw/main/ipwenter.o \
./src/lib/libipw/main/ipwexit.o \
./src/lib/libipw/main/opt_check.o \
./src/lib/libipw/main/usage.o 

C_DEPS += \
./src/lib/libipw/main/_cmdline.d \
./src/lib/libipw/main/_cvtoptarg.d \
./src/lib/libipw/main/_getargs.d \
./src/lib/libipw/main/_mainvars.d \
./src/lib/libipw/main/ipwenter.d \
./src/lib/libipw/main/ipwexit.d \
./src/lib/libipw/main/opt_check.d \
./src/lib/libipw/main/usage.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/main/%.o: ../src/lib/libipw/main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


