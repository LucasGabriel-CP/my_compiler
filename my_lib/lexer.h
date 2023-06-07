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
#include "token.h"
#include "HashMatrix.h"

class lexer : public HashMatrix{
private:
    //Demais variaveis
    std::vector<bool> terminal_states;
    std::vector<std::string> tk_types;
    std::vector<std::unordered_map<char, int>> state_matrix;
    std::vector<std::string> content;
    int state = 1, pos = 0, line = 0, ids = 0;
protected:
    std::vector<std::tuple<int, int, std::string>> errors;
public:
   
    //Tabelas de simbolos e de palavras reservadas
    std::unordered_map<std::string, std::string, custom_hash> MOperators;
    std::unordered_map<std::string, std::string, custom_hash> LOperators;
    std::unordered_map<std::string, int, custom_hash> tk_type;
    std::unordered_set<std::string, custom_hash> reserved_words;

    //Metodos do analisador lexico
    lexer();
    lexer(std::string const& filename);
    void add_reserved_words();
    void give_adjacence();
    void read_file(std::string const& filename);
    std::string analyse_str(std::string const& str, bool& comented, bool&is_str);
    char next_char();
    token next_token();
    void add_error(std::string str, token &tk);
    bool is_space(char const& c);
    bool is_eof();
    void backtrack(std::string &ans);
    void printerrors(std::ostream& os);
};
