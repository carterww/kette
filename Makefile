export CC=gcc
export C_STANDARD=c11
export CFLAGS=-Wall -Wextra -std=$(C_STANDARD) -pedantic -g

export PROJECT_ABS_PATH=$(shell pwd)
export TEST_SRC_PATH=$(PROJECT_ABS_PATH)/tests

export UNITY_PROJECT_NAME=unity
export UNITY_DIR=$(PROJECT_ABS_PATH)/external/Unity
export UNITY_BUILD_DIR=$(UNITY_DIR)/build

test: unity test-dlink test-slink

test-dlink: unity
	@make -C $(TEST_SRC_PATH) test-dlink

test-slink: unity
	@make -C $(TEST_SRC_PATH) test-slink

unity: $(UNITY_BUILD_DIR)
	@echo "Building Unity..."
	@cmake -B$(UNITY_BUILD_DIR) -S$(UNITY_DIR)
	@make -C $(UNITY_BUILD_DIR)
	@echo "Unity build complete: $(UNITY_BUILD_DIR)/lib$(UNITY_PROJECT_NAME).a"


$(UNITY_BUILD_DIR):
	@mkdir -p $(UNITY_BUILD_DIR)

clean-all: clean-unity clean

clean:
	@rm $(TEST_SRC_PATH)/dlink $(TEST_SRC_PATH)/slink
	@echo "Cleaned test executables: $(TEST_SRC_PATH)/dlink $(TEST_SRC_PATH)/slink"

clean-unity:
	@rm -rf $(UNITY_BUILD_DIR)
	@echo "Cleaned Unity build directory: $(UNITY_BUILD_DIR)"
