# Compiler settings
CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Iinclude/ -O3
LDFLAGS := -lm -lglad -lglfw -lGL -lX11 -lpthread -lXrandr -lXi

# Source files
SRCS := Learn.c

# Output files
RELEASE_OUTPUT := $ Learn

# Default target
all: release
# Release build
release: $(BUILD_DIR) $(RELEASE_OUTPUT)

$(RELEASE_OUTPUT): $(SRCS)
	@echo "Compiling release build..."
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "Build successful: $(RELEASE_OUTPUT)"

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf RELEASE_OUTPUT
	@echo "Cleaned build files."
