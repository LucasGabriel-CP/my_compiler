EXE = compilador
CC = g++ -std=c++20 -w
CPP = g++ -std=c++20 -w -o $(EXE)

all: Compilador clean

Compilador: main.o token.o lexer.o Node.o SyntaxTree.o parser.o
	$(CPP) main.o token.o lexer.o Node.o SyntaxTree.o parser.o

main.o: main.cpp token.h lexer.h Node.h SyntaxTree.h parser.h
	$(CC) -c main.cpp

token.o: token.h

lexer.o: lexer.h token.h

Node.o: Node.h

SyntaxTree.o: SyntaxTree.h Node.h

parser.o: parser.h lexer.h token.h SyntaxTree.h Node.h

clean:
	del /Q /F *.o