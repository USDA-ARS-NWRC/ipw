################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/import/pbm2ipw/main.o \
../src/bin/import/pbm2ipw/parm.o \
../src/bin/import/pbm2ipw/pnm2ipw.o \
../src/bin/import/pbm2ipw/pnm_header.o 

C_SRCS += \
../src/bin/import/pbm2ipw/main.c \
../src/bin/import/pbm2ipw/parm.c \
../src/bin/import/pbm2ipw/pnm2ipw.c \
../src/bin/import/pbm2ipw/pnm_header.c 

OBJS += \
./src/bin/import/pbm2ipw/main.o \
./src/bin/import/pbm2ipw/parm.o \
./src/bin/import/pbm2ipw/pnm2ipw.o \
./src/bin/import/pbm2ipw/pnm_header.o 

C_DEPS += \
./src/bin/import/pbm2ipw/main.d \
./src/bin/import/pbm2ipw/parm.d \
./src/bin/import/pbm2ipw/pnm2ipw.d \
./src/bin/import/pbm2ipw/pnm_header.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/pbm2ipw/%.o: ../src/bin/import/pbm2ipw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


