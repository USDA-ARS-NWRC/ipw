################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/physmod.pt/snobal/check_range.c \
../src/bin/physmod.pt/snobal/get_in_rec.c \
../src/bin/physmod.pt/snobal/get_mh_rec.c \
../src/bin/physmod.pt/snobal/get_pr_rec.c \
../src/bin/physmod.pt/snobal/get_sn_rec.c \
../src/bin/physmod.pt/snobal/getargs.c \
../src/bin/physmod.pt/snobal/getf.c \
../src/bin/physmod.pt/snobal/getok.c \
../src/bin/physmod.pt/snobal/initialize.c \
../src/bin/physmod.pt/snobal/main.c \
../src/bin/physmod.pt/snobal/openfiles.c \
../src/bin/physmod.pt/snobal/output.c \
../src/bin/physmod.pt/snobal/sn_err.c \
../src/bin/physmod.pt/snobal/vars.c 

OBJS += \
./src/bin/physmod.pt/snobal/check_range.o \
./src/bin/physmod.pt/snobal/get_in_rec.o \
./src/bin/physmod.pt/snobal/get_mh_rec.o \
./src/bin/physmod.pt/snobal/get_pr_rec.o \
./src/bin/physmod.pt/snobal/get_sn_rec.o \
./src/bin/physmod.pt/snobal/getargs.o \
./src/bin/physmod.pt/snobal/getf.o \
./src/bin/physmod.pt/snobal/getok.o \
./src/bin/physmod.pt/snobal/initialize.o \
./src/bin/physmod.pt/snobal/main.o \
./src/bin/physmod.pt/snobal/openfiles.o \
./src/bin/physmod.pt/snobal/output.o \
./src/bin/physmod.pt/snobal/sn_err.o \
./src/bin/physmod.pt/snobal/vars.o 

C_DEPS += \
./src/bin/physmod.pt/snobal/check_range.d \
./src/bin/physmod.pt/snobal/get_in_rec.d \
./src/bin/physmod.pt/snobal/get_mh_rec.d \
./src/bin/physmod.pt/snobal/get_pr_rec.d \
./src/bin/physmod.pt/snobal/get_sn_rec.d \
./src/bin/physmod.pt/snobal/getargs.d \
./src/bin/physmod.pt/snobal/getf.d \
./src/bin/physmod.pt/snobal/getok.d \
./src/bin/physmod.pt/snobal/initialize.d \
./src/bin/physmod.pt/snobal/main.d \
./src/bin/physmod.pt/snobal/openfiles.d \
./src/bin/physmod.pt/snobal/output.d \
./src/bin/physmod.pt/snobal/sn_err.d \
./src/bin/physmod.pt/snobal/vars.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.pt/snobal/%.o: ../src/bin/physmod.pt/snobal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


