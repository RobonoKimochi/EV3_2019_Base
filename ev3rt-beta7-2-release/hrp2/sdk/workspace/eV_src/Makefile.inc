mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_CXXOBJS += \
	LineMonitor.o \
	Button.o \
	PidController.o \
	Calibration.o \
	Tracer.o \
	Block.o \
	StateManager.o \
	RotaryEnc.o \
	Odmetry.o \
	Mat.o \
	BlueTooth.o \
	UI.o \
	Logger.o \
	Filter.o \
	Rotator.o \
	Remote.o \
	MeasureDistance.o \
	TailMotor.o \
	RunManager.o \
	Sound.o \
	LEDColor.o \
	COM.o \
	DriveController.o \
	CommandMaker.o \
	ArmMotor.o \
	Parking.o \

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)app $(mkfile_path)unit $(mkfile_path)device

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit \
	-I$(mkfile_path)device \
