################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/proj/reproj_area/headers.c \
../src/bin/proj/reproj_area/main.c \
../src/bin/proj/reproj_area/reproj_area.c 

OBJS += \
./src/bin/proj/reproj_area/headers.o \
./src/bin/proj/reproj_area/main.o \
./src/bin/proj/reproj_area/reproj_area.o 

C_DEPS += \
./src/bin/proj/reproj_area/headers.d \
./src/bin/proj/reproj_area/main.d \
./src/bin/proj/reproj_area/reproj_area.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/proj/reproj_area/%.o: ../src/bin/proj/reproj_area/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


