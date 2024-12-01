CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I./include

SRCS = src/main.c \
       src/commands.c \
       src/utils.c

OBJS = $(SRCS:.c=.o)
TARGET = myshell

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 