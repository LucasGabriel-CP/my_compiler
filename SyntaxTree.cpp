#include "SyntaxTree.h"

void SyntaxTree::dfs(Node* u, int &cnt, std::ostream& out){
    cnt++;
    for (Node* v: u->children) {
        for (int i = 0; i < cnt; i++) {
            out << "  ";
        }
        out << u->get_tipo() << " - " << v->get_tipo() << '\n';
        dfs(v, cnt, out);
    }
    cnt--;
}

SyntaxTree::SyntaxTree(): root(nullptr) { }

void SyntaxTree::add_node(Node* p) {
    root = p;
}

void SyntaxTree::add_node(Node* p, Node* r) {
    r->add_child(p);
}

void SyntaxTree::print_tree(std::ostream& out){
    Node* aux = root;
    int cnt = 0;
    dfs(aux, cnt, out);
    return;
}

Node* SyntaxTree::get_root() {
    return root;
}

