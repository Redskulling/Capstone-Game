TARGET_WEB ?= 0
COMPILER ?= g++

ifeq ($(TARGET_WEB),1)
	COMPILER ?= emcc
endif

BUILD_DIR_BASE := build

OPT_FLAGS := -O2

CPP_VERSION := -std=c++17

RAYLIB_FLAGS := -I -L -lraylib -lGL

SRC_DIRS := src math

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(SRC_DIRS)))

$(BUILD_DIR)/%.o: $(BUILD_DIR)%.cpp