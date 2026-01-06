CC = gcc
CFLAGS_BASE = -Wall -Wextra -Werror -Iinclude

BUILD ?= release

ifeq ($(BUILD), release)
	CFLAGS := $(CFLAGS_BASE) -O3
else
	CFLAGS := $(CFLAGS_BASE) -g -fsanitize=address
endif

ARENA_DIR = arena
LEXER_DIR = lexer
UTILS_DIR = utils
FRONTEND_DIR = frontend

BUILD_DIR = build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

ARENA_BUILD_DIR := $(BUILD_DIR)/$(ARENA_DIR)
ARENA_SRC := $(shell find $(ARENA_DIR) -name '*.c')
ARENA_OBJ := $(ARENA_SRC:$(ARENA_DIR)/%.c=$(ARENA_BUILD_DIR)/%.o)

$(ARENA_BUILD_DIR)/%.o: $(ARENA_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

LEXER_BUILD_DIR := $(BUILD_DIR)/$(LEXER_DIR)
LEXER_SRC := $(shell find $(LEXER_DIR) -name '*.c')
LEXER_OBJ := $(LEXER_SRC:$(LEXER_DIR)/%.c=$(LEXER_BUILD_DIR)/%.o)

$(LEXER_BUILD_DIR)/%.o: $(LEXER_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

UTILS_BUILD_DIR := $(BUILD_DIR)/$(UTILS_DIR)
UTILS_SRC := $(shell find $(UTILS_DIR) -name '*.c')
UTILS_OBJ := $(UTILS_SRC:$(UTILS_DIR)/%.c=$(UTILS_BUILD_DIR)/%.o)

$(UTILS_BUILD_DIR)/%.o: $(UTILS_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

FRONTEND_BUILD_DIR := $(BUILD_DIR)/$(FRONTEND_DIR)
FRONTEND_SRC := $(shell find $(FRONTEND_DIR) -name '*.c')
FRONTEND_OBJ := $(FRONTEND_SRC:$(FRONTEND_DIR)/%.c=$(FRONTEND_BUILD_DIR)/%.o)

$(FRONTEND_BUILD_DIR)/%.o: $(FRONTEND_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

STARLANG_BIN_NAME := starlang
STARLANG_BIN := $(BIN_DIR)/$(STARLANG_BIN_NAME)
ROOT_BIN_DIR = /usr/bin

$(STARLANG_BIN): $(ARENA_OBJ) $(LEXER_OBJ) $(UTILS_OBJ) $(FRONTEND_OBJ)
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) $^ -lezcli -lm -o $@

.PHONY: all clean

all: $(STARLANG_BIN)

install: $(STARLANG_BIN)
	@sudo cp $(STARLANG_BIN) $(ROOT_BIN_DIR)

uninstall:
	@sudo rm -rf $(ROOT_BIN_DIR)/$(STARLANG_BIN_NAME)

clean:
	@rm -rf $(BUILD_DIR)
