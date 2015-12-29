################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/topocalc/ghorizon/azmf.o \
../src/bin/topocalc/ghorizon/gcp.o \
../src/bin/topocalc/ghorizon/gcp_dist.o \
../src/bin/topocalc/ghorizon/ghorizon.o \
../src/bin/topocalc/ghorizon/headers.o \
../src/bin/topocalc/ghorizon/horizon.o \
../src/bin/topocalc/ghorizon/iobuf.o \
../src/bin/topocalc/ghorizon/main.o \
../src/bin/topocalc/ghorizon/newlqh.o \
../src/bin/topocalc/ghorizon/trigbuf.o \
../src/bin/topocalc/ghorizon/trigtbl.o 

C_SRCS += \
../src/bin/topocalc/ghorizon/azmf.c \
../src/bin/topocalc/ghorizon/gcp.c \
../src/bin/topocalc/ghorizon/gcp_dist.c \
../src/bin/topocalc/ghorizon/ghorizon.c \
../src/bin/topocalc/ghorizon/headers.c \
../src/bin/topocalc/ghorizon/horizon.c \
../src/bin/topocalc/ghorizon/iobuf.c \
../src/bin/topocalc/ghorizon/main.c \
../src/bin/topocalc/ghorizon/newlqh.c \
../src/bin/topocalc/ghorizon/trigbuf.c \
../src/bin/topocalc/ghorizon/trigtbl.c 

OBJS += \
./src/bin/topocalc/ghorizon/azmf.o \
./src/bin/topocalc/ghorizon/gcp.o \
./src/bin/topocalc/ghorizon/gcp_dist.o \
./src/bin/topocalc/ghorizon/ghorizon.o \
./src/bin/topocalc/ghorizon/headers.o \
./src/bin/topocalc/ghorizon/horizon.o \
./src/bin/topocalc/ghorizon/iobuf.o \
./src/bin/topocalc/ghorizon/main.o \
./src/bin/topocalc/ghorizon/newlqh.o \
./src/bin/topocalc/ghorizon/trigbuf.o \
./src/bin/topocalc/ghorizon/trigtbl.o 

C_DEPS += \
./src/bin/topocalc/ghorizon/azmf.d \
./src/bin/topocalc/ghorizon/gcp.d \
./src/bin/topocalc/ghorizon/gcp_dist.d \
./src/bin/topocalc/ghorizon/ghorizon.d \
./src/bin/topocalc/ghorizon/headers.d \
./src/bin/topocalc/ghorizon/horizon.d \
./src/bin/topocalc/ghorizon/iobuf.d \
./src/bin/topocalc/ghorizon/main.d \
./src/bin/topocalc/ghorizon/newlqh.d \
./src/bin/topocalc/ghorizon/trigbuf.d \
./src/bin/topocalc/ghorizon/trigtbl.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/ghorizon/%.o: ../src/bin/topocalc/ghorizon/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


