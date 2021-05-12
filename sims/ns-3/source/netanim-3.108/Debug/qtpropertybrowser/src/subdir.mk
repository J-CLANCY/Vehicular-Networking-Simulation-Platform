################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../qtpropertybrowser/src/fileedit.cpp \
../qtpropertybrowser/src/fileeditfactory.cpp \
../qtpropertybrowser/src/filepathmanager.cpp \
../qtpropertybrowser/src/qtbuttonpropertybrowser.cpp \
../qtpropertybrowser/src/qteditorfactory.cpp \
../qtpropertybrowser/src/qtgroupboxpropertybrowser.cpp \
../qtpropertybrowser/src/qtpropertybrowser.cpp \
../qtpropertybrowser/src/qtpropertybrowserutils.cpp \
../qtpropertybrowser/src/qtpropertymanager.cpp \
../qtpropertybrowser/src/qttreepropertybrowser.cpp \
../qtpropertybrowser/src/qtvariantproperty.cpp \
../qtpropertybrowser/src/variantfactory.cpp \
../qtpropertybrowser/src/variantmanager.cpp 

OBJS += \
./qtpropertybrowser/src/fileedit.o \
./qtpropertybrowser/src/fileeditfactory.o \
./qtpropertybrowser/src/filepathmanager.o \
./qtpropertybrowser/src/qtbuttonpropertybrowser.o \
./qtpropertybrowser/src/qteditorfactory.o \
./qtpropertybrowser/src/qtgroupboxpropertybrowser.o \
./qtpropertybrowser/src/qtpropertybrowser.o \
./qtpropertybrowser/src/qtpropertybrowserutils.o \
./qtpropertybrowser/src/qtpropertymanager.o \
./qtpropertybrowser/src/qttreepropertybrowser.o \
./qtpropertybrowser/src/qtvariantproperty.o \
./qtpropertybrowser/src/variantfactory.o \
./qtpropertybrowser/src/variantmanager.o 

CPP_DEPS += \
./qtpropertybrowser/src/fileedit.d \
./qtpropertybrowser/src/fileeditfactory.d \
./qtpropertybrowser/src/filepathmanager.d \
./qtpropertybrowser/src/qtbuttonpropertybrowser.d \
./qtpropertybrowser/src/qteditorfactory.d \
./qtpropertybrowser/src/qtgroupboxpropertybrowser.d \
./qtpropertybrowser/src/qtpropertybrowser.d \
./qtpropertybrowser/src/qtpropertybrowserutils.d \
./qtpropertybrowser/src/qtpropertymanager.d \
./qtpropertybrowser/src/qttreepropertybrowser.d \
./qtpropertybrowser/src/qtvariantproperty.d \
./qtpropertybrowser/src/variantfactory.d \
./qtpropertybrowser/src/variantmanager.d 


# Each subdirectory must supply rules for building sources it contributes
qtpropertybrowser/src/%.o: ../qtpropertybrowser/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


