#include "token.h"

token::token() {
    pos = { -1, -1 };
    type = "NULL";
    text = "NULL";
}

token::token(std::string const& type_, std::string const& text_, std::pair<int, int> const& pos_) {
    type = type_;
    text = text_;
    pos = pos_;
}

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

std::ostream& token::operator<<(std::ostream& os) const {
    return os << "type: [" << type << "] text: [" << text << "]\n";
}
