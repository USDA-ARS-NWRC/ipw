################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/export/ipw2ppm/headers.o \
../src/bin/export/ipw2ppm/initcolors.o \
../src/bin/export/ipw2ppm/ipw3ppm.o \
../src/bin/export/ipw2ppm/ipwload.o \
../src/bin/export/ipw2ppm/main.o \
../src/bin/export/ipw2ppm/pgmwrite.o \
../src/bin/export/ipw2ppm/ppmwrite.o \
../src/bin/export/ipw2ppm/readcolormap.o \
../src/bin/export/ipw2ppm/rgblookup.o 

C_SRCS += \
../src/bin/export/ipw2ppm/headers.c \
../src/bin/export/ipw2ppm/initcolors.c \
../src/bin/export/ipw2ppm/ipw3ppm.c \
../src/bin/export/ipw2ppm/ipwload.c \
../src/bin/export/ipw2ppm/main.c \
../src/bin/export/ipw2ppm/pgmwrite.c \
../src/bin/export/ipw2ppm/ppmwrite.c \
../src/bin/export/ipw2ppm/readcolormap.c \
../src/bin/export/ipw2ppm/rgblookup.c 

OBJS += \
./src/bin/export/ipw2ppm/headers.o \
./src/bin/export/ipw2ppm/initcolors.o \
./src/bin/export/ipw2ppm/ipw3ppm.o \
./src/bin/export/ipw2ppm/ipwload.o \
./src/bin/export/ipw2ppm/main.o \
./src/bin/export/ipw2ppm/pgmwrite.o \
./src/bin/export/ipw2ppm/ppmwrite.o \
./src/bin/export/ipw2ppm/readcolormap.o \
./src/bin/export/ipw2ppm/rgblookup.o 

C_DEPS += \
./src/bin/export/ipw2ppm/headers.d \
./src/bin/export/ipw2ppm/initcolors.d \
./src/bin/export/ipw2ppm/ipw3ppm.d \
./src/bin/export/ipw2ppm/ipwload.d \
./src/bin/export/ipw2ppm/main.d \
./src/bin/export/ipw2ppm/pgmwrite.d \
./src/bin/export/ipw2ppm/ppmwrite.d \
./src/bin/export/ipw2ppm/readcolormap.d \
./src/bin/export/ipw2ppm/rgblookup.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2ppm/%.o: ../src/bin/export/ipw2ppm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


