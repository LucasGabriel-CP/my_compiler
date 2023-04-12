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
#include"token.h"

class parser{
private:
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
    std::unordered_map<std::string,
            std::unordered_map<std::string, std::vector<std::string>, custom_hash>, custom_hash> predictive_table;
    std::unordered_map<std::string, int, custom_hash> hash_by_word;
    std::unordered_map<int, std::string, custom_hash> hash_by_value;
    std::vector<std::pair<std::string, std::string>> symbol_table;
    std::vector<token> tokens;
public:
    parser();
    parser(std::unordered_map<std::string, int, custom_hash> h1, std::unordered_map<int, std::string, custom_hash> h2, std::vector<token> const &tokens);

};

