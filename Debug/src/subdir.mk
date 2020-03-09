################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aubatch.c \
../src/batch_job.c \
../src/commandline_parser.c \
../src/process.c \
../src/schedular.c 

OBJS += \
./src/aubatch.o \
./src/batch_job.o \
./src/commandline_parser.o \
./src/process.o \
./src/schedular.o 

C_DEPS += \
./src/aubatch.d \
./src/batch_job.d \
./src/commandline_parser.d \
./src/process.d \
./src/schedular.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


