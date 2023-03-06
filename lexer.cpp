#include "lexer.h"

lexer::lexer(std::string const& filename) {
    MOperators = {
        {"+", "add"},
        {"-", "sub"},
        {"*", "mul"},
        {"/", "div"},
        {"^", "div"}
    };

    LOperator = {
        {">", "biiger"},
        {"<", "less"},
        {"==", "equal"},
        {">=", "bigger_equal"},
        {"<=", "less_equal"}
    };

    tk_type = {
        {"Identifier", 0},
        {"INumber", 1},
        {"FNumber", 2},
        {"MOperator", 3},
        {"LOperator", 4},
        {"Reserved", 5},
        {"Ponctuation", 6}
    };

    std::ifstream File(filename);
    if (!File.good()) {
        std::cerr << "ERRO: Arquivo nao existe\n";
        abort();
    }

    std::string aux, str;
    bool comented = false;
    //Ler arquivos e imprimir no novo arquivo
    while (std::getline(File, str)) {
        aux = analyse_str(str, comented);
        if (!aux.empty())
            content.push_back(aux);
    }
    content.push_back("\n");    
}

/*
Função para analisar a string
Parametros:
    str -> string para analisar
    comented -> se está dentro de um bloco comentado
*/
std::string lexer::analyse_str(std::string const& str, bool& comented) {
    std::string ans = "";                       //Nova string
    int cnt = 0, size_str = (int)str.size();    //Contador de espaços e tamanho da string
    for (int i = 0; i < size_str; i++) {
        if (i < size_str - 1) {
            //Se a linha tiver comentada, encerra o loop
            if (str[i] == '/' && str[i + 1] == '/') {
                break;
            }
            else if (str[i] == '/' && str[i + 1] == '*') { //Verifica inicio do comentário
                comented = true;
                i++;
            }
            else if (str[i + 1] == '/' && str[i] == '*') { //Verifica o fim do comentário
                comented = false;
                i++;
                continue;
            }
        }
        //Se tiver comentado continua
        if (comented) {
            continue;
        }
        //Conta os espaços
        if (str[i] == ' ') {
            cnt++;
        }
        else {
            cnt = 0;
        }
        //Se aparecer mais espaço do que necessário, continua
        if (ans.empty() && cnt || cnt > 1) {
            continue;
        }
        //Se não, adiciona a nova string
        ans += str[i];
    }
    //Se tiver algum conteúdo, insere a quebra de linha
    if (!ans.empty()) {
        //Se tiver um espaço no fim, remove ele
        if (cnt) {
            ans.pop_back();
        }
        ans += '\n';
    }
    return ans;
}

bool lexer::is_digit(char const& c) {
    return c >= '0' && c <= '9';
}

bool lexer::is_point(char const& c) {
    return c == '.';
}

bool lexer::is_char(char const& c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool lexer::is_operator(char const& c) {
    return c == '>' || c == '<' || c == '=' || c == '!';
}

bool lexer::is_space(char const& c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char lexer::next_char() {
    char ans = content[line][pos++];
    if (pos == (int)content[line].size()) {
        line++;
        pos = 0;
    }
    return ans;
}

token lexer::next_token() {
    try {
        //Retorna NULL se não tiver nada
        if (is_eof()) {
            return token();
        }
        state = 1;
        char cur;
        std::string ans;
        //Enquanto tiver coisa pra ler roda isso
        while (!is_eof()) {
            //Ler prox caracter
            cur = next_char();
            switch (state) {
            //Estado 1 (inicial)
            case 1:
                if (is_char(cur)) {
                    ans += cur;
                    state = 2;
                }
                else if (is_digit(cur)) {
                    ans += cur;
                    state = 4;
                }
                else if (is_operator(cur)) {
                    state = 5; //mudar dps
                }
                else if (!is_space(cur)) {
                    errors.push_back({ line, cur });
                    //throw std::runtime_error("Unrecognized SYMBOL");
                }
                break;
            //Estado 2
            case 2:
                if (is_char(cur) || is_digit(cur)) { //Palavra
                    ans += cur;
                }
                else {
                    state = 3;  //Estado Terminal
                }
                break;
            //Estado 3 (Terminal)
            case 3:
                //Recupera Caracter
                backtrack();
                if (!hash_by_word.count(ans)) { //Adiciona palavra na hash
                    hash_by_word[ans] = ids;
                    hash_by_value[ids] = ans;
                    ids++;
                }
                return token(tk_type["Identifier"], std::to_string(hash_by_word[ans]));
            //Estado 4 (Numero)
            case 4:
                if (is_digit(cur)) {
                    ans += cur;
                }
                else if (is_point(cur)) {   //Numero Real
                    ans += cur;
                    state = 6;
                }
                else if (!is_char(cur)) {
                    state = 5;  //Estado Terminal
                }
                else {
                    errors.push_back({ line, cur }); //Erro lexico
                    state = 1;
                    //throw std::runtime_error("Unrecognized NUMBER");
                }
                break;
            //Estado 5 (Terminal)
            case 5:
                //Recupera Caracter
                backtrack();
                return token(tk_type["INumber"], ans);
            //Estado 6 (Numero com Ponto Flutuante)
            case 6:
                if (is_digit(cur)) {
                    ans += cur;
                }
                else if (!is_char(cur)) {
                    state = 7; //Estado Terminal
                }
                else {
                    errors.push_back({ line, cur }); //Erro lexico
                    state = 1;
                    //throw std::runtime_error("Unrecognized NUMBER");
                }
                break;
            //Estado 7 (Terminal)
            case 7:
                backtrack();
                return token(tk_type["FNumber"], ans);
            }
        }
        return token();
    }
    catch (const std::runtime_error& error) {
        std::cout << error.what() << '\n';
    }
}

void lexer::backtrack() {
    if (pos) {
        pos--;
    }
    else if (line) {
        line--;
        pos = (int)content[line].size() - 1;
    }
    else {
        exit(123);
    }
}

bool lexer::is_eof() {
    return line == (int)content.size();
}

void lexer::printerrors(std::ostream& os) {
    if (errors.empty()) {
        os << "Any errors/warningsz\n"; return;
    }
    for (auto &[li, c] : errors) {
        os << li << std::setfill('.') << std::setw(3) << ": " << c << '\n';
    }
}
