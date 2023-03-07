#pragma once
#include <iostream>
#include<unordered_map>

class token{
private:
public:
    int type;
    std::string text;
    static const int tk_identifier  = 0, tk_inumber = 1, tk_fnumber = 2, tk_operator = 3,
        tk_ponctuation = 4, tk_assign = 5;
    token(int const& type_, std::string const& text_);
    token();
    void set_type(int const& type_);
    void set_text(std::string const& text_);
    int get_type();
    std::string get_text();
    std::ostream& operator<<(std::ostream& os) const;
};
