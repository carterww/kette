CC=gcc
C_STANDARD=c99
CFLAGS=-Wall -Wextra -std=$(C_STANDARD) -pedantic -g

PROJECT_ABS_PATH=$(shell pwd)
TEST_SRC_PATH=$(PROJECT_ABS_PATH)/test

UNITY_PROJECT_NAME=unity
UNITY_DIR=$(PROJECT_ABS_PATH)/external/Unity
UNITY_BUILD_DIR=$(UNITY_DIR)/build

test: unity test-dlink test-slink
	@echo "Running tests..."

unity: $(UNITY_BUILD_DIR)
	@echo "Building Unity..."
	@cmake -B$(UNITY_BUILD_DIR) -S$(UNITY_DIR)
	@make -C $(UNITY_BUILD_DIR)
	@echo "Unity build complete: $(UNITY_BUILD_DIR)/lib$(UNITY_PROJECT_NAME).a"


$(UNITY_BUILD_DIR):
	@mkdir -p $(UNITY_BUILD_DIR)

clean-all: clean-unity clean

clean:
	@rm -rf $(PROJECT_ABS_PATH)/build
	@echo "Cleaned project build directory: $(PROJECT_ABS_PATH)/build"

clean-unity:
	@rm -rf $(UNITY_BUILD_DIR)
	@echo "Cleaned Unity build directory: $(UNITY_BUILD_DIR)"
