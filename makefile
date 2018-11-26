BIN = table.ex hirakata.ex
OBJ = table.o
LIB = silk.o

.PHONY: all
all: $(BIN);

%.ex : %.o $(LIB)
	gcc -o $@ $< $(LIB)

%.o : %.c
	gcc -c -o $@ $< -O3

clean :
	rm $(BIN)