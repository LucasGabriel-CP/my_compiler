#pragma once
#include<iostream>
#include<vector>

class Node {
private:
    std::string tipo, escopo, valor;
    Node* parent;
public:
    std::vector<Node*> children;
    int id_child = 0;
    Node();
    Node(std::string _tipo, std::string _escopo);
    Node(std::string _tipo, std::string _escopo, Node* p);
    void add_child(Node*);
    std::string get_tipo();
    std::string get_escopo();
    void set_tipo(std::string);
    void set_valor(std::string);
    void set_escopo(std::string);
    Node* get_parent();
    bool is_leaf();
    friend bool operator == (Node & lhs, Node & rhs);
};

