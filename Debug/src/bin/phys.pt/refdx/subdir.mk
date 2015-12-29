################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/phys.pt/refdx/akcoef.o \
../src/bin/phys.pt/refdx/main.o \
../src/bin/phys.pt/refdx/modprod.o \
../src/bin/phys.pt/refdx/refh2o.o \
../src/bin/phys.pt/refdx/refice.o \
../src/bin/phys.pt/refdx/spval1.o \
../src/bin/phys.pt/refdx/vdcps.o \
../src/bin/phys.pt/refdx/vtdbl.o \
../src/bin/phys.pt/refdx/wavechoice.o 

C_SRCS += \
../src/bin/phys.pt/refdx/akcoef.c \
../src/bin/phys.pt/refdx/main.c \
../src/bin/phys.pt/refdx/modprod.c \
../src/bin/phys.pt/refdx/refh2o.c \
../src/bin/phys.pt/refdx/refice.c \
../src/bin/phys.pt/refdx/spval1.c \
../src/bin/phys.pt/refdx/vdcps.c \
../src/bin/phys.pt/refdx/vtdbl.c \
../src/bin/phys.pt/refdx/wavechoice.c 

OBJS += \
./src/bin/phys.pt/refdx/akcoef.o \
./src/bin/phys.pt/refdx/main.o \
./src/bin/phys.pt/refdx/modprod.o \
./src/bin/phys.pt/refdx/refh2o.o \
./src/bin/phys.pt/refdx/refice.o \
./src/bin/phys.pt/refdx/spval1.o \
./src/bin/phys.pt/refdx/vdcps.o \
./src/bin/phys.pt/refdx/vtdbl.o \
./src/bin/phys.pt/refdx/wavechoice.o 

C_DEPS += \
./src/bin/phys.pt/refdx/akcoef.d \
./src/bin/phys.pt/refdx/main.d \
./src/bin/phys.pt/refdx/modprod.d \
./src/bin/phys.pt/refdx/refh2o.d \
./src/bin/phys.pt/refdx/refice.d \
./src/bin/phys.pt/refdx/spval1.d \
./src/bin/phys.pt/refdx/vdcps.d \
./src/bin/phys.pt/refdx/vtdbl.d \
./src/bin/phys.pt/refdx/wavechoice.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.pt/refdx/%.o: ../src/bin/phys.pt/refdx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


