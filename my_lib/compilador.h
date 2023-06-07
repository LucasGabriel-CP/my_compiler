#include <iostream>
#include <vector>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"

namespace compilador {
    std::string in_file;
    std::vector<token> tokens;
    SyntaxTree AST;

    void run_lexer(std::ofstream& outFile) {
        lexer lx(in_file);
        //Analisar lexemas
        token tk = lx.next_token();
        
        while (tk.get_type() != "NULL") {
            tokens.push_back(tk);
            if (tk.get_type() != "ERROR") {
                outFile << tk << " \n"[tk.get_type() == ";"];
            }
            tk = lx.next_token();
        }

        outFile << '\n' << std::setfill('-') << std::setw(80) << '\n';
        lx.printerrors(outFile);
    }

    void run_parser(std::ofstream& outFile) {
        parser pr(tokens);
        AST = pr.work(outFile);
        outFile << '\n' << std::setfill('-') << std::setw(80) << '\n';
        pr.printerrors(outFile);
        outFile.close();
    }

    void print_ast(std::ofstream& outFile) {
        AST.print_tree(outFile);
    }

    void run_semantic(std::ofstream& outFile) {
        semantic_analisys::dfs(AST.get_root(), "NULL");
        semantic_analisys::printerrors(outFile);
    }
};