CC=g++
CFLAGS=-I.

main: main.o fiveThread.o tenThread.o
	$(CC) -o main  main.o fiveThread.o tenThread.o