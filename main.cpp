/*
autor: Lucas Gabriel de Godoi Arriel
data inicial: 02/03/2023
*/
//Import das bibliotecas
//#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "SyntaxTree.h"
#include "HashMatrix.h"

//Definir estado random
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

namespace semantic_analisys {
    //funcs[id_funcao] = {Linha, coluna, tipo, agrs, ...}
    std::unordered_map<std::string, std::vector<std::string>, HashMatrix::custom_hash> funcs;
    std::vector<std::tuple<int, int, std::string>> errors;
    HashMatrix symbol_table;
    int cnt_inner = 0;

    std::set<std::string> needs = { "<decl>", "<exp_atrib>", "<return_exp>", "<exp>", "<func_call>", "<arg>"};

    void add_decl(Node* u, std::string scope, std::string tipo) {
        for (Node* v : u->children) {
            if (v->get_tipo() == "id") {
                std::string err = symbol_table.add_id(v->get_valor(), tipo, scope, v->get_pos());
                if (!err.empty()) {
                    auto [x, y] = v->get_pos();
                    errors.push_back({ x, y, err });
                }
            }
            add_decl(v, scope, tipo);
        }
    }

    void add_arg(Node* u, std::string scope) {
        std::string tipo = "none";
        for (Node* v : u->children) {
            if (v->get_tipo() == "empty" || v->get_tipo() != ",") {
                continue;
            }
            if (v->get_tipo() == "<arg'>") {
                add_arg(v, scope);
            }
            else {
                if (v->get_tipo() != "id") {
                    tipo = v->get_tipo();
                }
                else {
                    funcs[scope].push_back(tipo);
                    std::string err = symbol_table.add_id(v->get_valor(), tipo, scope, u->get_pos());
                    if (!err.empty()) {
                        auto [x, y] = v->get_pos();
                        errors.push_back({ x, y, err });
                    }
                }
            }
        }
    }

    void valid_prod(Node *r, std::string scope) {
        std::string prod = r->get_tipo();
        if (prod == "<decl>") {
            std::string tipo = (r->children[0])->get_tipo();
            std::string err = symbol_table.add_id((r->children[1])->get_valor(), tipo, scope, (r->children[1])->get_pos());
            if (!err.empty()) {
                auto [x, y] = (r->children[1])->get_pos();
                errors.push_back({ x, y, err });
            }
            if ((r->children[2])->get_tipo() == "<decl'>") {
                add_decl(r->children[2], scope, tipo);
            }
            return;
        }
        if (prod == "<arg>") {
            add_arg(r, scope);
        }
        if (prod == "<exp>") {

        }
        if (prod == "<func_call>") {

        }
        if (prod == "<exp_atrib>") {
    
        }
        if (prod == "<return_exp>") {
    
        }
    }
    
    void dfs(Node* u, std::string scope) {
        std::string new_scope = scope;
        for (Node* v : u->children) {
            if (v->get_tipo() == "empty") continue;
            if (needs.find(v->get_tipo()) != needs.end()) {
                valid_prod(v, new_scope);
            }
            else {
                if (u->get_tipo() == "<func>"){
                    if (v->get_tipo() == "id") {
                        new_scope = v->get_valor();
                        auto [x, y] = v->get_pos();
                        if (funcs.find(new_scope) != funcs.end()) {
                            errors.push_back({ x, y, "Funcao ja declarada" });
                        }
                        funcs[new_scope].push_back(std::to_string(x));
                        funcs[new_scope].push_back(std::to_string(y));
                        funcs[new_scope].push_back("none");
                    }
                    else if (v->get_tipo() == "<func>") {
                        new_scope = scope;
                    }
                }
                else if (v->get_tipo() == "if") {
                    new_scope = "inner " + std::to_string(cnt_inner++);
                }
                else if (v->get_tipo() == "<else>") {
                    new_scope = scope;
                }
                if (new_scope != scope) {
                    symbol_table.parent[new_scope] = scope;
                }
                dfs(v, new_scope);
            }
        }
    }
    void printerrors(std::ostream& os) {
        if (errors.empty()) {
            os << "Any errors/warnings\n"; return;
        }
        os << "Semantic Errors/Warnings:\n";
        for (auto& [li, col, cara] : errors) {
            os << "Linha: " << li << std::setw(12)
                << "Coluna: " << col << std::setfill('.') << std::setw(7)
                << ": " << cara << '\n';
        }
    }
};

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
    SyntaxTree AST;
    AST = pr.work(outFile);
    outFile << '\n' << std::setfill('-') << std::setw(80) << '\n';
    pr.printerrors(outFile);
    outFile.close();
    std::cout << "Analise sintatica feita!(Tecle ENTER)\n";
    system("pause > null");

    outFile = std::ofstream("output_AST.txt");
    AST.print_tree(outFile);
    outFile.close();

    outFile = std::ofstream("output_semantic.txt");
    semantic_analisys::dfs(AST.get_root(), "NULL");
    semantic_analisys::printerrors(outFile);
    outFile.close();

    return 0;
}
