# =========================================================
# Makefile for Glam Studio – Makeup & Dress-Up Game
# =========================================================

CC      = gcc
TARGET  = makeup_game
SRCS    = main.c makeup.c dressup.c showcase.c ui.c
HDRS    = assets.h makeup.h dressup.h showcase.h ui.h
CFLAGS  = -Wall -Wextra -O2
LIBS    = -lGL -lGLU -lglut -lm

# macOS (Homebrew freeglut or XQuartz)
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LIBS = -framework OpenGL -framework GLUT -lm
    CFLAGS += -Wno-deprecated
endif

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run