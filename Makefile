.PHONY : all
.PHONY : clean
.PHONY : zip

ZIPNAME=xvinog00
TARGET=ifj22
CFLAGS=-std=c99 -Werror -g

all: $(TARGET)

$(TARGET): src/ifj22.c src/error.c src/scanner.c src/str.c src/symstack.c src/symtable.c src/parser.c 
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(TARGET)

# zip:
# 	zip $(ZIPNAME) src/* Makefile rozdeleni rozsireni doc/dokumentace.pdf
