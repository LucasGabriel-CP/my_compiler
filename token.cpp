#include "token.h"

//construtor padrao
token::token() {
    pos = { -1, -1 };
    type = "NULL";
    text = "NULL";
}

/*
Construtor do token
Parametros:
    type_: tipo do token
    text_: padrao do token
    pos:   posicao no arquivo
*/
token::token(std::string const& type_, std::string const& text_, std::pair<int, int> const& pos_) {
    type = type_;
    text = text_;
    pos = pos_;
}

//Getters e setters da classe
void token::set_type(std::string const& type_) {
    type = type_;
}

void token::set_text(std::string const& text_) {
    text = text_;
}

void token::set_pos(std::pair<int, int> const& pos_) {
    pos = pos_;
}

std::string token::get_type() {
    return type;
}

std::string token::get_text() {
    return text;
}

std::pair<int, int> token::get_pos() {
    return pos;
}

//Operador para imprimir o token
std::ostream& operator<<(std::ostream& os, token& tk) {
    auto [x, y] = tk.get_pos();
    std::string t = tk.get_type(), tx = tk.get_text();
    return os << "[" << t << ", " << tx << ", " << x << ", " << y << "]";
}
