SOURCE = test_myalloc.c myalloc.c buddy_alloc.c bitmap.c


CC = gcc
CFLAGS = -lm
TARGET = test_myalloc


$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE)  $(CFLAGS)
