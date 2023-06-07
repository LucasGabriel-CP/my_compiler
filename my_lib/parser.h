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
#include"./util/token.h"
#include"./util/SyntaxTree.h"
#include"./util/Node.h"
#include "./util/HashMatrix.h"

class parser : public lexer{
private:
    HashMatrix predictive_table;
    std::unordered_set<std::string, HashMatrix::custom_hash> terminals;
    std::vector<token> tokens;
    SyntaxTree AST;
public:
    parser();
    parser(std::vector<token> const &tokens);
	SyntaxTree work(std::ofstream &);
    void exc_error(std::string at, std::string str, std::pair<int, int> posi, int& id);
    std::string get_real_type(std::string str);
    void add_prod(std::string row, std::string col, std::vector<std::string> res);
    void printerrors(std::ostream& os);
};
