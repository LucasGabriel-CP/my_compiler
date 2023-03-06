#include <bits/stdc++.h>
#include "lexer.h"
#include "token.h"

std::ostream& operator<<(std::ostream& os, token const &tk) {
    std::string t;
    switch (tk.type) {
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
    return os << "[" << t << ", " << tk.text << "]\n";
}


int main(int argc, char** argv) {
    std::string str; std::cin >> str;
    lexer lx(str);
    
    token tk = lx.next_token();
    std::ofstream outFile("output.txt");
    
    while (tk.get_text() != "NULL") {
        outFile << tk << '\n';
        tk = lx.next_token();
    }

    lx.printerrors(outFile);

    return 0;
}
