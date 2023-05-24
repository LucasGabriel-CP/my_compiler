#pragma once
#include<iostream>
#include "Node.h"

class SyntaxTree {
private:
    Node* root;
public:
    SyntaxTree();
    void add_node(Node*);
    void add_node(Node*, Node*);
    Node* get_root();
};

