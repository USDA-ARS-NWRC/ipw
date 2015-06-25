################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libsnobal/snobal/_adj_layers.c \
../src/lib/libsnobal/snobal/_adj_snow.c \
../src/lib/libsnobal/snobal/_advec.c \
../src/lib/libsnobal/snobal/_below_thold.c \
../src/lib/libsnobal/snobal/_calc_layers.c \
../src/lib/libsnobal/snobal/_cold_content.c \
../src/lib/libsnobal/snobal/_divide_tstep.c \
../src/lib/libsnobal/snobal/_do_tstep.c \
../src/lib/libsnobal/snobal/_e_bal.c \
../src/lib/libsnobal/snobal/_evap_cond.c \
../src/lib/libsnobal/snobal/_h2o_compact.c \
../src/lib/libsnobal/snobal/_h_le.c \
../src/lib/libsnobal/snobal/_layer_mass.c \
../src/lib/libsnobal/snobal/_mass_bal.c \
../src/lib/libsnobal/snobal/_net_rad.c \
../src/lib/libsnobal/snobal/_new_density.c \
../src/lib/libsnobal/snobal/_precip.c \
../src/lib/libsnobal/snobal/_runoff.c \
../src/lib/libsnobal/snobal/_snowmelt.c \
../src/lib/libsnobal/snobal/_time_compact.c \
../src/lib/libsnobal/snobal/_vars.c \
../src/lib/libsnobal/snobal/do_data_tstep.c \
../src/lib/libsnobal/snobal/init_snow.c \
../src/lib/libsnobal/snobal/vars.c 

OBJS += \
./src/lib/libsnobal/snobal/_adj_layers.o \
./src/lib/libsnobal/snobal/_adj_snow.o \
./src/lib/libsnobal/snobal/_advec.o \
./src/lib/libsnobal/snobal/_below_thold.o \
./src/lib/libsnobal/snobal/_calc_layers.o \
./src/lib/libsnobal/snobal/_cold_content.o \
./src/lib/libsnobal/snobal/_divide_tstep.o \
./src/lib/libsnobal/snobal/_do_tstep.o \
./src/lib/libsnobal/snobal/_e_bal.o \
./src/lib/libsnobal/snobal/_evap_cond.o \
./src/lib/libsnobal/snobal/_h2o_compact.o \
./src/lib/libsnobal/snobal/_h_le.o \
./src/lib/libsnobal/snobal/_layer_mass.o \
./src/lib/libsnobal/snobal/_mass_bal.o \
./src/lib/libsnobal/snobal/_net_rad.o \
./src/lib/libsnobal/snobal/_new_density.o \
./src/lib/libsnobal/snobal/_precip.o \
./src/lib/libsnobal/snobal/_runoff.o \
./src/lib/libsnobal/snobal/_snowmelt.o \
./src/lib/libsnobal/snobal/_time_compact.o \
./src/lib/libsnobal/snobal/_vars.o \
./src/lib/libsnobal/snobal/do_data_tstep.o \
./src/lib/libsnobal/snobal/init_snow.o \
./src/lib/libsnobal/snobal/vars.o 

C_DEPS += \
./src/lib/libsnobal/snobal/_adj_layers.d \
./src/lib/libsnobal/snobal/_adj_snow.d \
./src/lib/libsnobal/snobal/_advec.d \
./src/lib/libsnobal/snobal/_below_thold.d \
./src/lib/libsnobal/snobal/_calc_layers.d \
./src/lib/libsnobal/snobal/_cold_content.d \
./src/lib/libsnobal/snobal/_divide_tstep.d \
./src/lib/libsnobal/snobal/_do_tstep.d \
./src/lib/libsnobal/snobal/_e_bal.d \
./src/lib/libsnobal/snobal/_evap_cond.d \
./src/lib/libsnobal/snobal/_h2o_compact.d \
./src/lib/libsnobal/snobal/_h_le.d \
./src/lib/libsnobal/snobal/_layer_mass.d \
./src/lib/libsnobal/snobal/_mass_bal.d \
./src/lib/libsnobal/snobal/_net_rad.d \
./src/lib/libsnobal/snobal/_new_density.d \
./src/lib/libsnobal/snobal/_precip.d \
./src/lib/libsnobal/snobal/_runoff.d \
./src/lib/libsnobal/snobal/_snowmelt.d \
./src/lib/libsnobal/snobal/_time_compact.d \
./src/lib/libsnobal/snobal/_vars.d \
./src/lib/libsnobal/snobal/do_data_tstep.d \
./src/lib/libsnobal/snobal/init_snow.d \
./src/lib/libsnobal/snobal/vars.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libsnobal/snobal/%.o: ../src/lib/libsnobal/snobal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


