#include "Node.h"

Node::Node() {
    data = token();
    parent = nullptr;
}

Node::Node(token tk): data(tk) {
    parent = nullptr;
}

Node::Node(token tk, Node* p) :
    data(tk), parent(p) {
}

void Node::add_child(Node* c) {
    children.push_back(c);
}

std::string Node::get_tipo() {
    return data.get_type();
}

std::string Node::get_valor() {
    return data.get_text();
}

std::pair<int, int> Node::get_pos() {
    return data.get_pos();
}

void Node::set_tipo(std::string str) {
    data.set_type(str);
}

void Node::set_valor(std::string val) {
    data.set_text(val);
}

Node* Node::get_parent() {
    return parent;
}

bool Node::is_leaf() {
    return children.empty();
}

bool operator==(Node & lhs, Node & rhs) {
    return lhs.get_tipo() == rhs.get_tipo();
}
