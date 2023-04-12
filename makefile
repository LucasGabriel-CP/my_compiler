EXE = compilador
CC = g++ -std=c++20 -w
CPP = g++ -std=c++20 -w -o $(EXE)

all: Compilador clean

Compilador: main.o token.o lexer.o
	$(CPP) main.o token.o lexer.o

main.o: main.cpp token.h lexer.h
	$(CC) -c main.cpp

token.o: token.h

lexer.o: lexer.h token.h

clean:
	del /Q /F *.o null
