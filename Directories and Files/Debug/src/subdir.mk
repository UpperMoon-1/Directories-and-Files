################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/file_copy.c \
../src/file_directory.c 

C_DEPS += \
./src/file_copy.d \
./src/file_directory.d 

OBJS += \
./src/file_copy.o \
./src/file_directory.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/file_copy.d ./src/file_copy.o ./src/file_directory.d ./src/file_directory.o

.PHONY: clean-src

