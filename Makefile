CFLAGS=-g -std=c89
LDFLAGS=-g -std=c89
SRCS=src/timekeeper.c
OBJS=$(SRCS:.c=.o)
TARGET=bin/timekeeper


all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

install:$(TARGET)
	cp $(TARGET) /usr/local/bin/tk

clean:
	rm -f bin/* src/*.o
