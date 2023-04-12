#include "parser.h"

parser::parser(){ }

parser::parser(std::unordered_map<std::string, int, custom_hash> h1,
    std::unordered_map<int, std::string, custom_hash> h2, std::vector<token> const& tokens) {
    hash_by_word = h1; hash_by_value = h2;
    symbol_table.resize((int)h1.size());
    std::vector<std::string> terminals = { "id", "constante", "while", "if", "else", "read", "print"
                                        ")", "(", "{", "}", ",", ";", "int", "float", "string",
                                        "=", "opM", "opL", "$" };
    this->tokens = tokens;

    //Construção da tabela
    predictive_table["<termo>"] = { { "id", {"id"} }, {"constante", {"constante"}} };
    predictive_table["<bloco>"] = { {"id", {"<comando>", "<bloco>"}}, {"while", {"<comando>", "<bloco>"}}, {"if", {"<comando>", "<bloco>"}},
        {"read", {"<comando>", "<bloco>"}}, {"print", {"<comando>", "<bloco>"}},
        {"int", {"<comando>", "<bloco>"}}, {"float", {"<comando>", "<bloco>"}}, {"string", {"<comando>", "<bloco>"}},
        {"}", {"empty"}}, {"$", {"empty"}}
    };
    predictive_table["<comando>"] = { {"id", {"<exp_atrib>"}}, {"while", {"<while>"}}, {"if", {"<if>"}},
        {"read", {"<read>"}}, {"int", {"<decl>"}}, {"float", {"<decl>"}}, {"string", {"<decl>"}}
    };
    predictive_table["<while>"] = { {"while", {"while", "(", "<exp>", "{", "<bloco>", "}", "<else>"}}};
    predictive_table["<if>"] = { {"if", {"if", "(", "<exp>", "{", "<bloco>", "}"}}};
    predictive_table["<else>"] = { {"while", {"empty"}}, {"if", {"empty"}}, {"read", {"empty"}}, {"print", {"empty"}},
        {"}", {"empty"}}, {"int", {"empty"}}, {"float", {"empty"}}, {"string", {"empty"}}, {"$", {"empty"}},
        {"else", {"{", "<bloco>", "}"}}
    };
    predictive_table["<read>"] = { {"read", {"read", "(", "id", "<read'>", ")", ";"}} };
    predictive_table["<read'>"] = { {")", {"empty"}}, {",", {",", "id", "<read'>"}} };
    predictive_table["<print>"] = { {"print", {"print", "(", "<termo>", "<print'>", ")", ";"}} };
    predictive_table["<print'>"] = { {")", {"empty"}}, {",", {",", "<termo>", "<print'>"}} };
    predictive_table["decl"] = { {"int", {"int", "id", "<decl'>", ";"}}, {"float", {"float", "id", "<decl'>", ";"}}, {"string", {"string", "id", "<decl'>", ";"}} };
    predictive_table["decl'"] = { {",", {",", "id", "<decl'>"}}, {";", {"empty"}} };
    predictive_table["exp_atrib"] = { {"id", {"id", "=", "<exp>", ";"}} };
    predictive_table["exp"] = { {"id", {"<termo>", "<exp'>"}}, {"constante", {"<termo>", "<exp'>"}} };
    predictive_table["exp'"] = { {";", {"empty"}}, {"opM", {"opM", "<termo>", "<exp'>"}}, {"opL", {"opL", "<termo>"}} };
}

