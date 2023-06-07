#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include "Node.h"
#include "SyntaxTree.h"
#include "HashMatrix.h"

namespace semantic_analisys {
    void valid_prod(Node*, std::string);
    bool check_func_cal(Node*);
    std::string check_exp(Node*, std::string const&);
    std::string get_termo(Node*, std::string const&);


    //funcs[id_funcao] = {Linha, coluna, tipo, agrs, ...}
    std::unordered_map<std::string, std::vector<std::string>, HashMatrix::custom_hash> funcs;
    std::vector<std::tuple<int, int, std::string>> errors;
    HashMatrix symbol_table;
    int cnt_inner = 0;

    std::set<std::string> needs = { "<decl>", "<exp_atrib>", "<return_exp>", "<exp>", "<func_call>", "<arg>" };

    bool check_comp(std::string a, std::string b) {
        if (a != "none" && b != "none" && (a == "string" || b == "string")) {
            return a == b;
        }
        return true;
    }

    void add_decl(Node* u, std::string scope, std::string tipo) {
        for (Node* v : u->children) {
            if (v->get_tipo() == "id") {
                std::string err = symbol_table.add_id(v->get_valor(), tipo, scope, v->get_pos());
                if (!err.empty()) {
                    auto [x, y] = v->get_pos();
                    errors.push_back({ x, y, err });
                }
            }
            add_decl(v, scope, tipo);
        }
    }

    void add_arg(Node* u, std::string scope) {
        std::string tipo = "none";
        for (Node* v : u->children) {
            if (v->get_tipo() == "empty" || v->get_tipo() != ",") {
                continue;
            }
            if (v->get_tipo() == "<arg'>") {
                add_arg(v, scope);
            }
            else {
                if (v->get_tipo() != "id") {
                    tipo = v->get_tipo();
                }
                else {
                    funcs[scope].push_back(tipo);
                    std::string err = symbol_table.add_id(v->get_valor(), tipo, scope, u->get_pos());
                    if (!err.empty()) {
                        auto [x, y] = v->get_pos();
                        errors.push_back({ x, y, err });
                    }
                }
            }
        }
    }

    std::string check_expl(Node* u, std::string const& scope) {
        if ((u->children[0])->get_tipo() == "empty") {
            return "none";
        }
        std::string optype = (u->children[0])->get_tipo(), opval = (u->children[0])->get_valor(),
            rhs = get_termo(u->children[1], scope);
        std::string other_exp = (optype == "opM" ? check_expl(u->children[2], scope) : "none");
        auto [x, y] = u->get_pos();
        if (rhs == "string" && optype == "opM") {
            if (opval != "add") {
                errors.push_back({ x, y, "invalid operant" });
                return "none";
            }
            return rhs;
        }
        if (check_comp(rhs, other_exp)) {
            return rhs;
        }
        errors.push_back({ x, y, "types not compatibles" });
        return "none";
    }

    std::string check_exp(Node* u, std::string const& scope) {
        std::string lhs = get_termo(u->children[0], scope), rhs = check_expl(u->children[1], scope);
        if (check_comp(lhs, rhs)) {
            return lhs;
        }
        auto [x, y] = u->get_pos();
        errors.push_back({ x, y, "types not compatibles" });
        return "none";
    }

    std::string get_termo(Node* r, std::string const& scope) {
        std::string type = (r->children[0])->get_tipo(), val = (r->children[0])->get_valor();
        auto [x, y] = r->get_pos();
        if (type == "INumber") {
            return "int";
        }
        if (type == "FNumber") {
            return "float";
        }
        if (type == "Frase") {
            return "string";
        }
        if (type == "id") {
            if (!symbol_table.check_decl(val, scope)) {
                return "!";
            }
            return symbol_table.table[scope][val][0];
        }
        if (type == "#") {
            if (r->children.size() <= 1) {
                return "none";
            }
            if (!check_func_cal(r->children[1])) {
                return "none";
            }
            return ((r->children[1])->children[1])->get_tipo();
        }
        if (r->children.size() <= 1) {
            return "none";
        }
        return check_exp(r->children[1], scope);
    }

    void check_arg(Node* u, std::string func_name, std::string scope, int t = 3) {
        bool ok = true;
        for (Node* v : u->children) {
            std::string type = v->get_tipo(), val = v->get_valor();
            if (type == ",") {
                continue;
            }
            auto [x, y] = v->get_pos();
            std::string err = get_termo(v, scope);
            if (err == "!") {
                errors.push_back({ x, y, std::to_string(t - 2) + " variavel nao declarada" });
            }
            else if (!check_comp(funcs[func_name][t], err)) {
                errors.push_back({ x, y, std::to_string(t - 2) + " argumento incompativel" });
            }
            valid_prod(u, scope);
        }
    }

    bool check_func_cal(Node* r) {
        auto [x, y] = r->get_pos();
        if (r->children.size() <= 1) {
            return false;
        }
        if (funcs.find((r->children[1])->get_valor()) == funcs.end()) {
            errors.push_back({ x, y, "Funcao nao declarada" });
            return false;
        }
        return true;
    }

    void valid_prod(Node* r, std::string scope) {
        std::string prod = r->get_tipo();
        if (prod == "<decl>") {
            std::string tipo = (r->children[0])->get_tipo();
            if (r->children.size() <= 2) {
                return;
            }
            std::string err = symbol_table.add_id((r->children[1])->get_valor(), tipo, scope, (r->children[1])->get_pos());
            if (!err.empty()) {
                auto [x, y] = (r->children[1])->get_pos();
                errors.push_back({ x, y, err });
            }
            if ((r->children[2])->get_tipo() == "<decl'>") {
                add_decl(r->children[2], scope, tipo);
            }
        }
        else if (prod == "<arg>") {
            add_arg(r, scope);
        }
        else if (prod == "<func_call>") {
            check_func_cal(r);
        }
        else if (prod == "<exp>") {
            std::string err = check_exp(r, scope);
        }
        else if (prod == "<exp_atrib>") {
            auto [x, y] = r->get_pos();
            if (!symbol_table.check_decl((r->children[0])->get_valor(), scope)) {
                errors.push_back({ x, y, "variavel nao declarada" });
            }
            else {
                std::string lhs = symbol_table.table[scope][(r->children[0])->get_valor()][0], rhs = check_exp(r->children[2], scope);
                if (!check_comp(lhs, rhs)) {
                    errors.push_back({ x, y, "tipos incompativeis" });
                }
            }
        }
        else if (prod == "<return_exp>") {
            while (symbol_table.parent[scope] != "NULL" && funcs.find(scope) == funcs.end()) {
                scope = symbol_table.parent[scope];
            }
            std::string ans = get_termo(r->children[1], scope);
            auto [x, y] = r->get_pos();
            if (funcs[scope][2] != "none") {
                if (funcs[scope][2] != ans) {
                    errors.push_back({ x, y, "multiplos tipos de retorno na funcao " + scope });
                }
            }
            else {
                funcs[scope][2] = ans;
            }
        }
    }

    void dfs(Node* u, std::string const& scope) {
        std::string new_scope = scope;
        for (Node* v : u->children) {
            if (v->get_tipo() == "empty") continue;
            if (needs.find(v->get_tipo()) != needs.end()) {
                valid_prod(v, new_scope);
            }
            else {
                if (u->get_tipo() == "<func>") {
                    if (v->get_tipo() == "id") {
                        new_scope = v->get_valor();
                        auto [x, y] = v->get_pos();
                        if (funcs.find(new_scope) != funcs.end()) {
                            errors.push_back({ x, y, "Funcao ja declarada" });
                        }
                        funcs[new_scope].push_back(std::to_string(x));
                        funcs[new_scope].push_back(std::to_string(y));
                        funcs[new_scope].push_back("none");
                    }
                    else if (v->get_tipo() == "<func>") {
                        new_scope = scope;
                    }
                }
                else if (v->get_tipo() == "if" || v->get_tipo() == "else") {
                    new_scope = "inner " + std::to_string(cnt_inner++);
                }
                else if (v->get_tipo() == "<else>") {
                    new_scope = scope;
                }
                if (new_scope != scope) {
                    symbol_table.parent[new_scope] = scope;
                }
                dfs(v, new_scope);
            }
        }
    }

    void printerrors(std::ostream& os) {
        if (errors.empty()) {
            os << "Any errors/warnings\n"; return;
        }
        os << "Semantic Errors/Warnings:\n";
        for (auto& [li, col, cara] : errors) {
            os << "Linha: " << li << std::setw(12)
                << "Coluna: " << col << std::setfill('.') << std::setw(7)
                << ": " << cara << '\n';
        }
    }
};