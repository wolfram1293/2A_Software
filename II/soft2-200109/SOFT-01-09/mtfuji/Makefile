CC = gcc
CFLAGS = -w -g
LDLIBS = -lm
OBJS = main.o func1.o
TARGET = mtfuji

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

.PHONY: tmpclean clean
tmpclean:
	rm -f *~
clean: tmpclean
	rm -f $(OBJS) $(TARGET) 
