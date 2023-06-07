#pragma once
#include<iostream>
#include<string>
#include<map>
#include<unordered_map>
#include<chrono>
#include<random>

class HashMatrix {
public:
    //Funcao hash
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
        std::unordered_map<std::string, std::vector<std::string>, custom_hash>,
        custom_hash> table;
    std::map<std::string, std::string> parent;
    std::string add_id(std::string id_name, std::string decl_type, std::string scope, std::pair<int, int> posi);
    bool check_decl(std::string id_name, std::string scope, std::string &par);
    HashMatrix();
    ~HashMatrix();
};

