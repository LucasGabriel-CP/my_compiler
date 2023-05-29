EXE = compilador
CC = g++ -std=c++20 -w
CPP = g++ -std=c++20 -w -o $(EXE)

all: Compilador clean

Compilador: main.o HashMatrix.o token.o lexer.o Node.o SyntaxTree.o parser.o
	$(CPP) main.o HashMatrix.o token.o lexer.o Node.o SyntaxTree.o parser.o

main.o: main.cpp token.h HashMatrix.h lexer.h Node.h SyntaxTree.h parser.h
	$(CC) -c main.cpp

HashMatrix.o: HashMatrix.h

token.o: token.h

lexer.o: lexer.h token.h HashMatrix.h

Node.o: Node.h

SyntaxTree.o: SyntaxTree.h Node.h

parser.o: parser.h lexer.h token.h HashMatrix.h SyntaxTree.h Node.h

clean:
	del /Q /F *.o