SRCS=src/timekeep.c
OBJS=$(SRCS:.c=.o)
TARGET=bin/timekeep


all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o:%.c
	$(CC) -c $^ -o $@

clean:
	rm -f bin/* src/*.o
