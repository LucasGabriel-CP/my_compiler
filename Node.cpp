#include "Node.h"

Node::Node() {
    tipo = escopo = valor = "none";
    parent = nullptr;
}

Node::Node(std::string _tipo, std::string _escopo):
    tipo(_tipo), escopo(_escopo) {
    parent = nullptr;
    valor = "none";
}

Node::Node(std::string _tipo, std::string _escopo, Node* p):
    tipo(_tipo), escopo(_escopo), parent(p) {
    valor = "none";
}

void Node::add_child(Node* c) {
    children.push_back(c);
}

std::string Node::get_tipo() {
    return tipo;
}

std::string Node::get_escopo() {
    return escopo;
}

void Node::set_tipo(std::string str) {
    tipo = str;
}

void Node::set_valor(std::string val) {
    valor = val;
}

void Node::set_escopo(std::string str) {
    escopo = str;
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
