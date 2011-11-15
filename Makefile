CC = gcc
CCOPTS = -c -g -Wall
LINKOPTS = -g

all: myshell

myshell: main.o 
	$(CC) $(LINKOPTS) -o $@ $^

main.o: main.c
		$(CC) $(CCOPTS) main.c

clean:
	rm -rf *.o main main.exe *~
