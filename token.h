#pragma once
#include <iostream>
#include<unordered_map>

class token{
private:
    std::pair<int, int> pos;
    std::string type;
    std::string text;
public:
    token();
    token(std::string const& type_, std::string const& text_, std::pair<int, int> const& pos_);
    void set_type(std::string const& type_);
    void set_text(std::string const& text_);
    void set_pos(std::pair<int, int> const& pos_);
    std::string get_type();
    std::string get_text();
    std::pair<int, int> get_pos();
    std::ostream& operator<<(std::ostream& os) const;
};
