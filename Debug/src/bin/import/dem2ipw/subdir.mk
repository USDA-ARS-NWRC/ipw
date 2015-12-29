################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/import/dem2ipw/atoid.o \
../src/bin/import/dem2ipw/bigatof.o \
../src/bin/import/dem2ipw/demhdr.o \
../src/bin/import/dem2ipw/fillvec.o \
../src/bin/import/dem2ipw/headers.o \
../src/bin/import/dem2ipw/litatof.o \
../src/bin/import/dem2ipw/main.o \
../src/bin/import/dem2ipw/output.o \
../src/bin/import/dem2ipw/parm.o \
../src/bin/import/dem2ipw/prdemhdr.o \
../src/bin/import/dem2ipw/profhdr.o \
../src/bin/import/dem2ipw/profiles.o \
../src/bin/import/dem2ipw/proj.o \
../src/bin/import/dem2ipw/prprofhdr.o \
../src/bin/import/dem2ipw/units.o 

C_SRCS += \
../src/bin/import/dem2ipw/atoid.c \
../src/bin/import/dem2ipw/bigatof.c \
../src/bin/import/dem2ipw/demhdr.c \
../src/bin/import/dem2ipw/fillvec.c \
../src/bin/import/dem2ipw/headers.c \
../src/bin/import/dem2ipw/litatof.c \
../src/bin/import/dem2ipw/main.c \
../src/bin/import/dem2ipw/output.c \
../src/bin/import/dem2ipw/parm.c \
../src/bin/import/dem2ipw/prdemhdr.c \
../src/bin/import/dem2ipw/profhdr.c \
../src/bin/import/dem2ipw/profiles.c \
../src/bin/import/dem2ipw/proj.c \
../src/bin/import/dem2ipw/prprofhdr.c \
../src/bin/import/dem2ipw/units.c 

OBJS += \
./src/bin/import/dem2ipw/atoid.o \
./src/bin/import/dem2ipw/bigatof.o \
./src/bin/import/dem2ipw/demhdr.o \
./src/bin/import/dem2ipw/fillvec.o \
./src/bin/import/dem2ipw/headers.o \
./src/bin/import/dem2ipw/litatof.o \
./src/bin/import/dem2ipw/main.o \
./src/bin/import/dem2ipw/output.o \
./src/bin/import/dem2ipw/parm.o \
./src/bin/import/dem2ipw/prdemhdr.o \
./src/bin/import/dem2ipw/profhdr.o \
./src/bin/import/dem2ipw/profiles.o \
./src/bin/import/dem2ipw/proj.o \
./src/bin/import/dem2ipw/prprofhdr.o \
./src/bin/import/dem2ipw/units.o 

C_DEPS += \
./src/bin/import/dem2ipw/atoid.d \
./src/bin/import/dem2ipw/bigatof.d \
./src/bin/import/dem2ipw/demhdr.d \
./src/bin/import/dem2ipw/fillvec.d \
./src/bin/import/dem2ipw/headers.d \
./src/bin/import/dem2ipw/litatof.d \
./src/bin/import/dem2ipw/main.d \
./src/bin/import/dem2ipw/output.d \
./src/bin/import/dem2ipw/parm.d \
./src/bin/import/dem2ipw/prdemhdr.d \
./src/bin/import/dem2ipw/profhdr.d \
./src/bin/import/dem2ipw/profiles.d \
./src/bin/import/dem2ipw/proj.d \
./src/bin/import/dem2ipw/prprofhdr.d \
./src/bin/import/dem2ipw/units.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/dem2ipw/%.o: ../src/bin/import/dem2ipw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


