/*
autor: Lucas Gabriel de Godoi Arriel
data inicial: 02/03/2023
*/
//Import das bibliotecas
#include <bits/stdc++.h>
#include "lexer.h"
#include "token.h"
#include "parser.h"

//Definir estado random
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

int main(int argc, char* argv[]) {
    //Ler nome do arquivo
    std::string str = "input2.cmp";
    /*
    std::cout << "Digite o nome do Arquivo: ";
    std::cin >> str;
    */
    lexer lx(str);
    
    std::vector<token> tokens;
    //Analisar lexemas
    token tk = lx.next_token();
    std::ofstream outFile("output_lexer.txt");
    while (tk.get_type() != "NULL") {
        tokens.push_back(tk);
        if (tk.get_type() != "ERROR") {
            outFile << tk << " \n"[tk.get_type() == ";"];
        }
        tk = lx.next_token();
    }
    
    outFile << '\n' << std::setfill('-') << std::setw(80) << '\n';
    lx.printerrors(outFile);
    outFile.close();
    std::cout << "Tokens gerados!(Tecle ENTER)\n";
    system("pause > null");
    
    outFile = std::ofstream("output_parser.txt");
    parser pr(tokens);
    pr.work(outFile);
    std::cout << "Analise feita!(Tecle ENTER)\n";
    system("pause > null");
    outFile.close();

    return 0;
}
