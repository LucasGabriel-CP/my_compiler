#include "SyntaxTree.h"

SyntaxTree::SyntaxTree(): root(nullptr) { }

void SyntaxTree::add_node(Node* p) {
    root = p;
}

void SyntaxTree::add_node(Node* p, Node* r) {
    r->add_child(p);
}

Node* SyntaxTree::get_root() {
    return root;
}

