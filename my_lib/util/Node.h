#pragma once
#include<iostream>
#include<vector>
#include"token.h"

class Node {
private:
    token data;
    Node* parent;
public:
    std::vector<Node*> children;
    int id_child = 0;
    Node();
    Node(token tk);
    Node(token tk, Node* p);
    void add_child(Node*);
    std::string get_tipo();
    std::string get_valor();
    std::pair<int, int> get_pos();
    void set_tipo(std::string);
    void set_valor(std::string);
    Node* get_parent();
    bool is_leaf();
    friend bool operator == (Node & lhs, Node & rhs);
};

