################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libsnobal/snobalp/_adj_layers.c \
../src/lib/libsnobal/snobalp/_adj_snow.c \
../src/lib/libsnobal/snobalp/_advec.c \
../src/lib/libsnobal/snobalp/_below_thold.c \
../src/lib/libsnobal/snobalp/_calc_layers.c \
../src/lib/libsnobal/snobalp/_cold_content.c \
../src/lib/libsnobal/snobalp/_divide_tstep.c \
../src/lib/libsnobal/snobalp/_do_tstep.c \
../src/lib/libsnobal/snobalp/_e_bal.c \
../src/lib/libsnobal/snobalp/_evap_cond.c \
../src/lib/libsnobal/snobalp/_h2o_compact.c \
../src/lib/libsnobal/snobalp/_h_le.c \
../src/lib/libsnobal/snobalp/_layer_mass.c \
../src/lib/libsnobal/snobalp/_mass_bal.c \
../src/lib/libsnobal/snobalp/_net_rad.c \
../src/lib/libsnobal/snobalp/_new_density.c \
../src/lib/libsnobal/snobalp/_precip.c \
../src/lib/libsnobal/snobalp/_runoff.c \
../src/lib/libsnobal/snobalp/_snowmelt.c \
../src/lib/libsnobal/snobalp/_time_compact.c \
../src/lib/libsnobal/snobalp/_vars.c \
../src/lib/libsnobal/snobalp/do_data_tstep.c \
../src/lib/libsnobal/snobalp/init_snow.c \
../src/lib/libsnobal/snobalp/vars.c 

OBJS += \
./src/lib/libsnobal/snobalp/_adj_layers.o \
./src/lib/libsnobal/snobalp/_adj_snow.o \
./src/lib/libsnobal/snobalp/_advec.o \
./src/lib/libsnobal/snobalp/_below_thold.o \
./src/lib/libsnobal/snobalp/_calc_layers.o \
./src/lib/libsnobal/snobalp/_cold_content.o \
./src/lib/libsnobal/snobalp/_divide_tstep.o \
./src/lib/libsnobal/snobalp/_do_tstep.o \
./src/lib/libsnobal/snobalp/_e_bal.o \
./src/lib/libsnobal/snobalp/_evap_cond.o \
./src/lib/libsnobal/snobalp/_h2o_compact.o \
./src/lib/libsnobal/snobalp/_h_le.o \
./src/lib/libsnobal/snobalp/_layer_mass.o \
./src/lib/libsnobal/snobalp/_mass_bal.o \
./src/lib/libsnobal/snobalp/_net_rad.o \
./src/lib/libsnobal/snobalp/_new_density.o \
./src/lib/libsnobal/snobalp/_precip.o \
./src/lib/libsnobal/snobalp/_runoff.o \
./src/lib/libsnobal/snobalp/_snowmelt.o \
./src/lib/libsnobal/snobalp/_time_compact.o \
./src/lib/libsnobal/snobalp/_vars.o \
./src/lib/libsnobal/snobalp/do_data_tstep.o \
./src/lib/libsnobal/snobalp/init_snow.o \
./src/lib/libsnobal/snobalp/vars.o 

C_DEPS += \
./src/lib/libsnobal/snobalp/_adj_layers.d \
./src/lib/libsnobal/snobalp/_adj_snow.d \
./src/lib/libsnobal/snobalp/_advec.d \
./src/lib/libsnobal/snobalp/_below_thold.d \
./src/lib/libsnobal/snobalp/_calc_layers.d \
./src/lib/libsnobal/snobalp/_cold_content.d \
./src/lib/libsnobal/snobalp/_divide_tstep.d \
./src/lib/libsnobal/snobalp/_do_tstep.d \
./src/lib/libsnobal/snobalp/_e_bal.d \
./src/lib/libsnobal/snobalp/_evap_cond.d \
./src/lib/libsnobal/snobalp/_h2o_compact.d \
./src/lib/libsnobal/snobalp/_h_le.d \
./src/lib/libsnobal/snobalp/_layer_mass.d \
./src/lib/libsnobal/snobalp/_mass_bal.d \
./src/lib/libsnobal/snobalp/_net_rad.d \
./src/lib/libsnobal/snobalp/_new_density.d \
./src/lib/libsnobal/snobalp/_precip.d \
./src/lib/libsnobal/snobalp/_runoff.d \
./src/lib/libsnobal/snobalp/_snowmelt.d \
./src/lib/libsnobal/snobalp/_time_compact.d \
./src/lib/libsnobal/snobalp/_vars.d \
./src/lib/libsnobal/snobalp/do_data_tstep.d \
./src/lib/libsnobal/snobalp/init_snow.d \
./src/lib/libsnobal/snobalp/vars.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libsnobal/snobalp/%.o: ../src/lib/libsnobal/snobalp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


