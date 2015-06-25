################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libimage/image/_free_img_cb.c \
../src/lib/libimage/image/_new_img_cb.c \
../src/lib/libimage/image/_read_hdrs.c \
../src/lib/libimage/image/_write_hdrs.c \
../src/lib/libimage/image/access.c \
../src/lib/libimage/image/annotation.c \
../src/lib/libimage/image/close_image.c \
../src/lib/libimage/image/get_geoh.c \
../src/lib/libimage/image/has_lqh.c \
../src/lib/libimage/image/history.c \
../src/lib/libimage/image/image_name.c \
../src/lib/libimage/image/lqh_max.c \
../src/lib/libimage/image/lqh_min.c \
../src/lib/libimage/image/nbands.c \
../src/lib/libimage/image/new_fpbuf.c \
../src/lib/libimage/image/new_image.c \
../src/lib/libimage/image/new_pbuf.c \
../src/lib/libimage/image/nlines.c \
../src/lib/libimage/image/not_a_tty.c \
../src/lib/libimage/image/nsamples.c \
../src/lib/libimage/image/open_image.c \
../src/lib/libimage/image/read_fpbuf.c \
../src/lib/libimage/image/read_pbuf.c \
../src/lib/libimage/image/set_annotation.c \
../src/lib/libimage/image/set_geoh.c \
../src/lib/libimage/image/set_history.c \
../src/lib/libimage/image/set_lqh.c \
../src/lib/libimage/image/write_fpbuf.c \
../src/lib/libimage/image/write_pbuf.c 

OBJS += \
./src/lib/libimage/image/_free_img_cb.o \
./src/lib/libimage/image/_new_img_cb.o \
./src/lib/libimage/image/_read_hdrs.o \
./src/lib/libimage/image/_write_hdrs.o \
./src/lib/libimage/image/access.o \
./src/lib/libimage/image/annotation.o \
./src/lib/libimage/image/close_image.o \
./src/lib/libimage/image/get_geoh.o \
./src/lib/libimage/image/has_lqh.o \
./src/lib/libimage/image/history.o \
./src/lib/libimage/image/image_name.o \
./src/lib/libimage/image/lqh_max.o \
./src/lib/libimage/image/lqh_min.o \
./src/lib/libimage/image/nbands.o \
./src/lib/libimage/image/new_fpbuf.o \
./src/lib/libimage/image/new_image.o \
./src/lib/libimage/image/new_pbuf.o \
./src/lib/libimage/image/nlines.o \
./src/lib/libimage/image/not_a_tty.o \
./src/lib/libimage/image/nsamples.o \
./src/lib/libimage/image/open_image.o \
./src/lib/libimage/image/read_fpbuf.o \
./src/lib/libimage/image/read_pbuf.o \
./src/lib/libimage/image/set_annotation.o \
./src/lib/libimage/image/set_geoh.o \
./src/lib/libimage/image/set_history.o \
./src/lib/libimage/image/set_lqh.o \
./src/lib/libimage/image/write_fpbuf.o \
./src/lib/libimage/image/write_pbuf.o 

C_DEPS += \
./src/lib/libimage/image/_free_img_cb.d \
./src/lib/libimage/image/_new_img_cb.d \
./src/lib/libimage/image/_read_hdrs.d \
./src/lib/libimage/image/_write_hdrs.d \
./src/lib/libimage/image/access.d \
./src/lib/libimage/image/annotation.d \
./src/lib/libimage/image/close_image.d \
./src/lib/libimage/image/get_geoh.d \
./src/lib/libimage/image/has_lqh.d \
./src/lib/libimage/image/history.d \
./src/lib/libimage/image/image_name.d \
./src/lib/libimage/image/lqh_max.d \
./src/lib/libimage/image/lqh_min.d \
./src/lib/libimage/image/nbands.d \
./src/lib/libimage/image/new_fpbuf.d \
./src/lib/libimage/image/new_image.d \
./src/lib/libimage/image/new_pbuf.d \
./src/lib/libimage/image/nlines.d \
./src/lib/libimage/image/not_a_tty.d \
./src/lib/libimage/image/nsamples.d \
./src/lib/libimage/image/open_image.d \
./src/lib/libimage/image/read_fpbuf.d \
./src/lib/libimage/image/read_pbuf.d \
./src/lib/libimage/image/set_annotation.d \
./src/lib/libimage/image/set_geoh.d \
./src/lib/libimage/image/set_history.d \
./src/lib/libimage/image/set_lqh.d \
./src/lib/libimage/image/write_fpbuf.d \
./src/lib/libimage/image/write_pbuf.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libimage/image/%.o: ../src/lib/libimage/image/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


