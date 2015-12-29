################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libmodel/radiation/beta_0.o \
../src/lib/libmodel/radiation/brutsaert.o \
../src/lib/libmodel/radiation/delta_edd.o \
../src/lib/libmodel/radiation/mwgamma.o \
../src/lib/libmodel/radiation/net_therm.o \
../src/lib/libmodel/radiation/twostream.o 

C_SRCS += \
../src/lib/libmodel/radiation/beta_0.c \
../src/lib/libmodel/radiation/brutsaert.c \
../src/lib/libmodel/radiation/delta_edd.c \
../src/lib/libmodel/radiation/mwgamma.c \
../src/lib/libmodel/radiation/net_therm.c \
../src/lib/libmodel/radiation/twostream.c 

OBJS += \
./src/lib/libmodel/radiation/beta_0.o \
./src/lib/libmodel/radiation/brutsaert.o \
./src/lib/libmodel/radiation/delta_edd.o \
./src/lib/libmodel/radiation/mwgamma.o \
./src/lib/libmodel/radiation/net_therm.o \
./src/lib/libmodel/radiation/twostream.o 

C_DEPS += \
./src/lib/libmodel/radiation/beta_0.d \
./src/lib/libmodel/radiation/brutsaert.d \
./src/lib/libmodel/radiation/delta_edd.d \
./src/lib/libmodel/radiation/mwgamma.d \
./src/lib/libmodel/radiation/net_therm.d \
./src/lib/libmodel/radiation/twostream.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libmodel/radiation/%.o: ../src/lib/libmodel/radiation/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


