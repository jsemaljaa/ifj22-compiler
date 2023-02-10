.PHONY : all
.PHONY : clean
.PHONY : zip

ZIPNAME=xvinog00
TARGET=ifj22
CFLAGS=-std=c99 -Werror 

all: $(TARGET)

$(TARGET): *.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(TARGET)
