################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/datetime/add_to_dt.o \
../src/lib/libipw/datetime/dt2fmtstr.o \
../src/lib/libipw/datetime/dt2str.o \
../src/lib/libipw/datetime/dt_diff.o \
../src/lib/libipw/datetime/dt_in_order.o \
../src/lib/libipw/datetime/gmt2local.o \
../src/lib/libipw/datetime/leapyear.o \
../src/lib/libipw/datetime/local2gmt.o \
../src/lib/libipw/datetime/make_dt.o \
../src/lib/libipw/datetime/now_dt.o \
../src/lib/libipw/datetime/numdays.o \
../src/lib/libipw/datetime/sec2hms.o \
../src/lib/libipw/datetime/str2dt.o \
../src/lib/libipw/datetime/waterday.o \
../src/lib/libipw/datetime/wday2mday.o \
../src/lib/libipw/datetime/weekday.o \
../src/lib/libipw/datetime/yday2mday.o \
../src/lib/libipw/datetime/yearday.o \
../src/lib/libipw/datetime/zone2str.o 

C_SRCS += \
../src/lib/libipw/datetime/add_to_dt.c \
../src/lib/libipw/datetime/dt2fmtstr.c \
../src/lib/libipw/datetime/dt2str.c \
../src/lib/libipw/datetime/dt_diff.c \
../src/lib/libipw/datetime/dt_in_order.c \
../src/lib/libipw/datetime/gmt2local.c \
../src/lib/libipw/datetime/leapyear.c \
../src/lib/libipw/datetime/local2gmt.c \
../src/lib/libipw/datetime/make_dt.c \
../src/lib/libipw/datetime/now_dt.c \
../src/lib/libipw/datetime/numdays.c \
../src/lib/libipw/datetime/sec2hms.c \
../src/lib/libipw/datetime/str2dt.c \
../src/lib/libipw/datetime/waterday.c \
../src/lib/libipw/datetime/wday2mday.c \
../src/lib/libipw/datetime/weekday.c \
../src/lib/libipw/datetime/yday2mday.c \
../src/lib/libipw/datetime/yearday.c \
../src/lib/libipw/datetime/zone2str.c 

OBJS += \
./src/lib/libipw/datetime/add_to_dt.o \
./src/lib/libipw/datetime/dt2fmtstr.o \
./src/lib/libipw/datetime/dt2str.o \
./src/lib/libipw/datetime/dt_diff.o \
./src/lib/libipw/datetime/dt_in_order.o \
./src/lib/libipw/datetime/gmt2local.o \
./src/lib/libipw/datetime/leapyear.o \
./src/lib/libipw/datetime/local2gmt.o \
./src/lib/libipw/datetime/make_dt.o \
./src/lib/libipw/datetime/now_dt.o \
./src/lib/libipw/datetime/numdays.o \
./src/lib/libipw/datetime/sec2hms.o \
./src/lib/libipw/datetime/str2dt.o \
./src/lib/libipw/datetime/waterday.o \
./src/lib/libipw/datetime/wday2mday.o \
./src/lib/libipw/datetime/weekday.o \
./src/lib/libipw/datetime/yday2mday.o \
./src/lib/libipw/datetime/yearday.o \
./src/lib/libipw/datetime/zone2str.o 

C_DEPS += \
./src/lib/libipw/datetime/add_to_dt.d \
./src/lib/libipw/datetime/dt2fmtstr.d \
./src/lib/libipw/datetime/dt2str.d \
./src/lib/libipw/datetime/dt_diff.d \
./src/lib/libipw/datetime/dt_in_order.d \
./src/lib/libipw/datetime/gmt2local.d \
./src/lib/libipw/datetime/leapyear.d \
./src/lib/libipw/datetime/local2gmt.d \
./src/lib/libipw/datetime/make_dt.d \
./src/lib/libipw/datetime/now_dt.d \
./src/lib/libipw/datetime/numdays.d \
./src/lib/libipw/datetime/sec2hms.d \
./src/lib/libipw/datetime/str2dt.d \
./src/lib/libipw/datetime/waterday.d \
./src/lib/libipw/datetime/wday2mday.d \
./src/lib/libipw/datetime/weekday.d \
./src/lib/libipw/datetime/yday2mday.d \
./src/lib/libipw/datetime/yearday.d \
./src/lib/libipw/datetime/zone2str.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/datetime/%.o: ../src/lib/libipw/datetime/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


