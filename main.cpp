/*
autor: Lucas Gabriel de Godoi Arriel
data: 02/03/2023
*/
//Import das bibliotecas
#include <bits/stdc++.h>
#include "lexer.h"
#include "token.h"

//Definir estado random
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

//Operador para imprimir o token
std::ostream& operator<<(std::ostream& os, token &tk) {
    auto [x, y] = tk.get_pos();
    std::string t = tk.get_type(), tx = tk.get_text();
    return os << "[" << t << ", " << tx << ", " << x << ", " << y << "]";
}

int main(int argc, char* argv[]) {
    //Ler nome do arquivo
    std::string str = "input2.cmp";
    /*
    std::cout << "Digite o nome do Arquivo: ";
    std::cin >> str;
    */
    lexer lx(str);
    
    //Analisar lexemas
    token tk = lx.next_token();
    std::ofstream outFile("output.txt");
    while (tk.get_type() != "NULL") {
        if (tk.get_type() != "ERROR") {
            outFile << tk << " \n"[tk.get_type() == ";"];
        }
        tk = lx.next_token();
    }
    
    //Printar erros
    outFile << std::setfill('-') << std::setw(80) << '\n';
    lx.printerrors(outFile);
    std::cout << "Arquivo analisado!(Tecle ENTER)\n";
    system("pause > null");

    return 0;
}
