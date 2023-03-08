#include <bits/stdc++.h>
#include "lexer.h"
#include "token.h"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

std::ostream& operator<<(std::ostream& os, token &tk) {
    auto [x, y] = tk.get_pos();
    std::string t = tk.get_type(), tx = tk.get_text();
    return os << "[" << t << ", " << tx << ", " << x << ", " << y << "]";
}

int main(int argc, char* argv[]) {
    std::string str = "input2.cmp";
    //std::cin >> str;
    lexer lx(str);
    
    token tk = lx.next_token();
    std::ofstream outFile("output.txt");
    while (tk.get_type() != "NULL") {
        if (tk.get_type() != "ERROR") {
            outFile << tk << " \n"[tk.get_type() == ";"];
        }
        tk = lx.next_token();
    }

    lx.printerrors(outFile);

    return 0;
}
