# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I./include -D__UT_NO_FPRINTF -D__UT_AES_USING_LCRYPTO
LDFLAGS = -lcrypto

# Directories to search for source files
# Works like a subproject
SRCDIRS = aes padding queue unit etc

# Recursive wildcard function (GNU Make)
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Find all .c sources recursively
SRCS := $(call rwildcard,$(SRCDIRS)/,*.c)

# Objects go to obj/, preserving directory structure
OBJDIR = obj
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

# Target executable
TARGET = main

.PHONY: all clean

all: $(TARGET)

# Link final executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile .c -> .o with directory creation
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I./aes -I./padding -I./queue -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
