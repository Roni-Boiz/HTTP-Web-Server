################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Client.c \
../Parameters.c \
../Response.c \
../Routes.c \
../Server.c \
../main.c 

OBJS += \
./Client.o \
./Parameters.o \
./Response.o \
./Routes.o \
./Server.o \
./main.o 

C_DEPS += \
./Client.d \
./Parameters.d \
./Response.d \
./Routes.d \
./Server.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


