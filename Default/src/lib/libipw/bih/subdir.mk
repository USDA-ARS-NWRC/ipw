################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libipw/bih/_bihcheck.c \
../src/lib/libipw/bih/_bihfree.c \
../src/lib/libipw/bih/_bihinit.c \
../src/lib/libipw/bih/_bihvars.c \
../src/lib/libipw/bih/bih.c \
../src/lib/libipw/bih/bihdup.c \
../src/lib/libipw/bih/bihmake.c \
../src/lib/libipw/bih/bihread.c \
../src/lib/libipw/bih/bihwrite.c \
../src/lib/libipw/bih/hbyteorder.c \
../src/lib/libipw/bih/hnbands.c \
../src/lib/libipw/bih/hnbits.c \
../src/lib/libipw/bih/hnbytes.c \
../src/lib/libipw/bih/hnlines.c \
../src/lib/libipw/bih/hnsamps.c \
../src/lib/libipw/bih/hostorder.c \
../src/lib/libipw/bih/imgsize.c \
../src/lib/libipw/bih/no_history.c \
../src/lib/libipw/bih/sampsize.c 

OBJS += \
./src/lib/libipw/bih/_bihcheck.o \
./src/lib/libipw/bih/_bihfree.o \
./src/lib/libipw/bih/_bihinit.o \
./src/lib/libipw/bih/_bihvars.o \
./src/lib/libipw/bih/bih.o \
./src/lib/libipw/bih/bihdup.o \
./src/lib/libipw/bih/bihmake.o \
./src/lib/libipw/bih/bihread.o \
./src/lib/libipw/bih/bihwrite.o \
./src/lib/libipw/bih/hbyteorder.o \
./src/lib/libipw/bih/hnbands.o \
./src/lib/libipw/bih/hnbits.o \
./src/lib/libipw/bih/hnbytes.o \
./src/lib/libipw/bih/hnlines.o \
./src/lib/libipw/bih/hnsamps.o \
./src/lib/libipw/bih/hostorder.o \
./src/lib/libipw/bih/imgsize.o \
./src/lib/libipw/bih/no_history.o \
./src/lib/libipw/bih/sampsize.o 

C_DEPS += \
./src/lib/libipw/bih/_bihcheck.d \
./src/lib/libipw/bih/_bihfree.d \
./src/lib/libipw/bih/_bihinit.d \
./src/lib/libipw/bih/_bihvars.d \
./src/lib/libipw/bih/bih.d \
./src/lib/libipw/bih/bihdup.d \
./src/lib/libipw/bih/bihmake.d \
./src/lib/libipw/bih/bihread.d \
./src/lib/libipw/bih/bihwrite.d \
./src/lib/libipw/bih/hbyteorder.d \
./src/lib/libipw/bih/hnbands.d \
./src/lib/libipw/bih/hnbits.d \
./src/lib/libipw/bih/hnbytes.d \
./src/lib/libipw/bih/hnlines.d \
./src/lib/libipw/bih/hnsamps.d \
./src/lib/libipw/bih/hostorder.d \
./src/lib/libipw/bih/imgsize.d \
./src/lib/libipw/bih/no_history.d \
./src/lib/libipw/bih/sampsize.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/bih/%.o: ../src/lib/libipw/bih/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


