CC = gcc
# Base CFLAGS for *all* builds
BASE_CFLAGS = -std=c99 -Wall -Wextra -I./src

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# --- Release Config (default) ---
TARGET_RELEASE = mysh
BUILD_DIR_RELEASE = intermediate/release
OBJS_RELEASE = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR_RELEASE)/%.o,$(SRCS))
CFLAGS_RELEASE = $(BASE_CFLAGS) -O2 
LDFLAGS_RELEASE = 

# --- Debug Config ---
TARGET_DEBUG = mysh_debug
BUILD_DIR_DEBUG = intermediate/debug
OBJS_DEBUG = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR_DEBUG)/%.o,$(SRCS))
CFLAGS_DEBUG = $(BASE_CFLAGS) -DDEBUG -g -fsanitize=undefined,address
LDFLAGS_DEBUG = -fsanitize=undefined,address


# Default target
all: $(TARGET_RELEASE)

# Target to build the release version
release: $(TARGET_RELEASE)

# Target to build the debug version
debug: $(TARGET_DEBUG)

# --- Run Targets ---
run: $(TARGET_RELEASE)
	./$(TARGET_RELEASE) $(ARGS)

run-debug: $(TARGET_DEBUG)
	./$(TARGET_DEBUG) $(ARGS)

# Link release executable
$(TARGET_RELEASE): $(OBJS_RELEASE)
	$(CC) $(OBJS_RELEASE) $(LDFLAGS_RELEASE) -o $(TARGET_RELEASE)

# Compile release object
$(BUILD_DIR_RELEASE)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR_RELEASE)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

# Link debug executable
$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CC) $(OBJS_DEBUG) $(LDFLAGS_DEBUG) -o $(TARGET_DEBUG)

# Compile debug object
$(BUILD_DIR_DEBUG)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR_DEBUG)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@


# --- Directory Creation ---
$(BUILD_DIR_RELEASE):
	mkdir -p $(BUILD_DIR_RELEASE)

$(BUILD_DIR_DEBUG):
	mkdir -p $(BUILD_DIR_DEBUG)

# --- Clean Target ---
clean:
	rm -rf intermediate $(TARGET_RELEASE) $(TARGET_DEBUG)

.PHONY: all clean debug release run run-debug


