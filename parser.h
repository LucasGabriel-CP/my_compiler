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

class parser : public lexer{
private:
    std::unordered_map<std::string,
            std::unordered_map<std::string, std::vector<std::string>, lexer::custom_hash>,
        lexer::custom_hash> predictive_table;
    std::vector<std::pair<std::string, std::string>> symbol_table;
    std::vector<token> tokens;
public:
    parser();
    parser(std::vector<token> const &tokens);
	void work(std::ofstream &outFile);
};
