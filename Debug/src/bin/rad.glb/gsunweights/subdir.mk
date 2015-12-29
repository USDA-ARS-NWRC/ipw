################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/rad.glb/gsunweights/gsunweights.o \
../src/bin/rad.glb/gsunweights/headers.o \
../src/bin/rad.glb/gsunweights/main.o \
../src/bin/rad.glb/gsunweights/out_image.o \
../src/bin/rad.glb/gsunweights/output.o 

C_SRCS += \
../src/bin/rad.glb/gsunweights/gsunweights.c \
../src/bin/rad.glb/gsunweights/headers.c \
../src/bin/rad.glb/gsunweights/main.c \
../src/bin/rad.glb/gsunweights/out_image.c \
../src/bin/rad.glb/gsunweights/output.c 

OBJS += \
./src/bin/rad.glb/gsunweights/gsunweights.o \
./src/bin/rad.glb/gsunweights/headers.o \
./src/bin/rad.glb/gsunweights/main.o \
./src/bin/rad.glb/gsunweights/out_image.o \
./src/bin/rad.glb/gsunweights/output.o 

C_DEPS += \
./src/bin/rad.glb/gsunweights/gsunweights.d \
./src/bin/rad.glb/gsunweights/headers.d \
./src/bin/rad.glb/gsunweights/main.d \
./src/bin/rad.glb/gsunweights/out_image.d \
./src/bin/rad.glb/gsunweights/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.glb/gsunweights/%.o: ../src/bin/rad.glb/gsunweights/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


