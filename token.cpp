#include "token.h"


token::token() {
    type = -1;
    text = "NULL";
}

token::token(int const& type_, std::string const& text_) {
    type = type_;
    text = text_;
}

void token::set_type(int const& type_) {
    type = type_;
}

void token::set_text(std::string const& text_) {
    text = text_;
}

int token::get_type() {
    return type;
}

std::string token::get_text() {
    return text;
}

std::ostream& token::operator<<(std::ostream& os) const {
    std::string t;
    switch (type) {
    case 0:
        t = "Identifier"; break;
    case 1:
        t = "INumber"; break;
    case 2:
        t = "FNumber"; break;
    case 3:
        t = "LOperator"; break;
    case 4:
        t = "Ponctuation"; break;
    case 5:
        t = "MOperator"; break;
    case 6:
        t = "Reserved Word"; break;
    }
    return os << "type: [" << type << "] text: [" << text << "]\n";
}
