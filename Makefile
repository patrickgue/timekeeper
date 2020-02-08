CFLAGS=-g -std=c90
LDFLAGS=-g -std=c90
SRCS=src/timekeeper.c
OBJS=$(SRCS:.c=.o)
TARGET=bin/timekeeper


all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f bin/* src/*.o
