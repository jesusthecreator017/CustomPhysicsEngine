CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -O2

# Detect OS
OS := $(shell uname -s 2>/dev/null || echo Windows)

ifeq ($(OS), Darwin) # macOS
    LDFLAGS = -L/usr/local/lib -lraylib -lm -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    TARGET = $(BINDIR)/main
else ifeq ($(OS), Linux) # Linux
    LDFLAGS = -lraylib -lm -lGL -lX11 -lpthread -ldl -I/usr/local/include
    TARGET = $(BINDIR)/main
else # Assume Windows (MinGW)
    CC = x86_64-w64-mingw32-gcc
    LDFLAGS = -lraylib -lm -lopengl32 -lgdi32 -lwinmm
    TARGET = $(BINDIR)/main.exe
endif

SRCDIR = src
HEADDIR = headers
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

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

winrun: all
	$(TARGET)

.PHONY: all clean run winrun
