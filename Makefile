CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -O2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin) # macOS
	LDFLAGS = -L/usr/local/lib -lraylib -lm -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else # Linux/Windows (using mingw for Windows cross-compilation)
	LDFLAGS = -lraylib -lm -lGL -lX11 -lpthread -ldl
endif

SRCDIR = src
HEADDIR = headers
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/main

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(HEADDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: all
	./$(TARGET)

.PHONY: all clean run
