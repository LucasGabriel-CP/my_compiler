#pragma once
#include<iostream>
#include<unordered_map>
#include<iomanip>
#include<string>
#include<vector>
#include<fstream>
#include<chrono>
#include "token.h"

class lexer{
private:

    //Hashes necessarias
    struct custom_hash {
        static uint64_t splitmix64(uint64_t x) {
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
            static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }

        size_t operator()(std::string x) const {
            static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(std::hash<std::string>()(x) + FIXED_RANDOM);
        }
    };
    std::unordered_map<std::string, std::string, custom_hash> MOperators;
    std::unordered_map<std::string, std::string, custom_hash> LOperators;
    std::unordered_map<std::string, int, custom_hash> tk_type;
    std::unordered_map<std::string, int, custom_hash> hash_by_word;
    std::unordered_map<int, std::string, custom_hash> hash_by_value;


    std::vector<std::string> content;
    std::vector<std::pair<int, char>> errors;
    int state = 1, pos = 0, line = 0, ids = 0;
public:
    lexer(std::string const& filename);
    std::string analyse_str(std::string const& str, bool& comented);
    bool is_digit(char const& c);
    bool is_point(char const& c);
    bool is_char(char const& c);
    bool is_operator(char const& c);
    bool is_space(char const& c);
    bool is_eof();
    char next_char();
    token next_token();
    void backtrack();
    void printerrors(std::ostream& os);
};

