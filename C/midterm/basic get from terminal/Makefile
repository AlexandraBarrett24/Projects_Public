CC=gcc
CFLAGS=-I./include
DEPS=main.h name.h address.h major.h
OBJ=main.o name.o address.o major.o
EXE=w2.out

all: $(EXE)
$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CLFAGS)
%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)
clean:
	rm *.o $(EXE)
run:
	./$(EXE)
