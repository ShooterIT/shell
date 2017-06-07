# shell

CC = gcc
CFLAGS = -Wall -g
TARGET = shell
OBJS = $(TARGET).o

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf *.o $(TARGET)
