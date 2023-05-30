#pragma once
#include<iostream>
#include "Node.h"

class SyntaxTree {
private:
    Node* root;
    void dfs(Node*, int&, std::ostream&);
public:
    SyntaxTree();
    void add_node(Node*);
    void add_node(Node*, Node*);
    void print_tree(std::ostream& out);
    Node* get_root();
};

