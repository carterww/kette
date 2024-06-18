test-dlink:
	@echo "Running doubly linked list tests..."
	@$(CC) $(CFLAGS) -I$(PROJECT_ABS_PATH) -I$(UNITY_DIR)/src -I$(TEST_SRC_PATH) $(TEST_SRC_PATH)/dlink.c $(UNITY_BUILD_DIR)/lib$(UNITY_PROJECT_NAME).a -o $(TEST_SRC_PATH)/dlink
	@$(TEST_SRC_PATH)/dlink

test-slink:
	@echo "Running singly linked list tests..."
	@$(CC) $(CFLAGS) -I$(PROJECT_ABS_PATH) -I$(UNITY_DIR)/src -I$(TEST_SRC_PATH) $(TEST_SRC_PATH)/slink.c $(UNITY_BUILD_DIR)/lib$(UNITY_PROJECT_NAME).a -o $(TEST_SRC_PATH)/slink
	@$(TEST_SRC_PATH)/slink
