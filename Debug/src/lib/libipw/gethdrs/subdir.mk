################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/gethdrs/copyhdrs.o \
../src/lib/libipw/gethdrs/gethdrs.o \
../src/lib/libipw/gethdrs/skiphdrs.o 

C_SRCS += \
../src/lib/libipw/gethdrs/copyhdrs.c \
../src/lib/libipw/gethdrs/gethdrs.c \
../src/lib/libipw/gethdrs/skiphdrs.c 

OBJS += \
./src/lib/libipw/gethdrs/copyhdrs.o \
./src/lib/libipw/gethdrs/gethdrs.o \
./src/lib/libipw/gethdrs/skiphdrs.o 

C_DEPS += \
./src/lib/libipw/gethdrs/copyhdrs.d \
./src/lib/libipw/gethdrs/gethdrs.d \
./src/lib/libipw/gethdrs/skiphdrs.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/gethdrs/%.o: ../src/lib/libipw/gethdrs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


