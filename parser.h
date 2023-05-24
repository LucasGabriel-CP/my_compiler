#pragma once
//Import das bibliotecas necessarias
#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<iomanip>
#include<string>
#include<vector>
#include<fstream>
#include<chrono>
#include<random>
#include<stack>
#include"lexer.h"
#include"token.h"
#include"SyntaxTree.h"
#include"Node.h"

using HashMatrix = std::unordered_map<std::string,
    std::unordered_map<std::string, std::vector<std::string>, lexer::custom_hash>,
    lexer::custom_hash>;

class parser : public lexer{
private:
    HashMatrix predictive_table;
    std::unordered_set<std::string, lexer::custom_hash> terminals;
    std::vector<token> tokens;
    SyntaxTree AST;
public:
    parser();
    parser(std::vector<token> const &tokens);
	SyntaxTree work(std::ofstream &e, HashMatrix&, std::unordered_map<std::string, std::vector<std::string>, lexer::custom_hash>&);
    void exc_error(std::string at, std::string str, std::pair<int, int> posi, int& id);
    void add_prod(std::string row, std::string col, std::vector<std::string> res);
};
