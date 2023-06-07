/*
autor: Lucas Gabriel de Godoi Arriel
data inicial: 02/03/2023
*/
//Import das bibliotecas
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include "./my_lib/compilador.h"

//Definir estado random
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

int main(int argc, char* argv[]) {
    //Ler nome do arquivo
    std::string str = "input2.cmp";
    /*
    std::cout << "Digite o nome do Arquivo: ";
    std::cin >> str;
    */
    compilador::in_file = str;

    std::ofstream outFile("output_lexer.txt");
    compilador::run_lexer(outFile);
    outFile.close();
    std::cout << "Tokens gerados!(Tecle ENTER)\n";
    system("pause > null");
    
    outFile = std::ofstream("output_parser.txt");
    compilador::run_parser(outFile);
    outFile.close();
    std::cout << "Analise sintatica feita!(Tecle ENTER)\n";
    system("pause > null");

    outFile = std::ofstream("output_AST.txt");
    compilador::print_ast(outFile);
    outFile.close();

    outFile = std::ofstream("output_semantic.txt");
    compilador::run_semantic(outFile);
    outFile.close();

    return 0;
}
