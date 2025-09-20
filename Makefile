# Makefile for ModToolbox

.PHONY: all build clean run prepare_build_dir

BUILD_DIR := build

all: build

prepare_build_dir:
	@mkdir -p $(BUILD_DIR)

build: prepare_build_dir
	@echo "Configuring and building ModToolbox..."
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

run: build
	@echo "Running ModToolbox..."
	@./$(BUILD_DIR)/modtoolbox

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)