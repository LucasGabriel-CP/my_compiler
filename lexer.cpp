#include "lexer.h"

/*
Construtor
Parametros:
    filename -> Nome do arquivo com o programa fonte
*/
lexer::lexer(){ }
lexer::lexer(std::string const& filename) {
    MOperators = {
        {"+", "add"},
        {"-", "sub"},
        {"*", "mul"},
        {"/", "div"},
        {"%", "mod"},
        {"^", "pow"}
    };

    LOperators = {
        {">", "bigger"},
        {"<", "less"},
        {"==", "equal"},
        {">=", "bigger_equal"},
        {"<=", "less_equal"}
    };

    add_reserved_words();
    give_adjacence();
    read_file(filename);
}

//Funcao para adicionar palavras reservadas
void lexer::add_reserved_words() {
    reserved_words.insert("int");
    reserved_words.insert("float");
    reserved_words.insert("string");
    reserved_words.insert("if");
    reserved_words.insert("else");
    reserved_words.insert("while");
    reserved_words.insert("def");
    reserved_words.insert("return");
    reserved_words.insert("read");
    reserved_words.insert("print");
}

//Funcao para adicionar transicoes
void lexer::give_adjacence() {
    state_matrix.assign(256, std::unordered_map<char, int>());
    terminal_states.assign(256, false);
    terminal_states[3]  = terminal_states[5]  = true;
    terminal_states[7]  = terminal_states[9]  = true;
    terminal_states[10] = terminal_states[12] = true;
    terminal_states[15] = terminal_states[18] = true;
    terminal_states[19] = terminal_states[20] = true;

    state_matrix[1][' ']  = state_matrix[1]['\n'] = 1;
    state_matrix[1]['\t'] = state_matrix[1]['\r'] = 1;
    state_matrix[1]['('] = state_matrix[1][')'] = 19;
    state_matrix[1]['{'] = state_matrix[1]['}'] = 19;
    state_matrix[1][','] = state_matrix[1][';'] = 20;
    state_matrix[1]['$'] = 0;
    state_matrix[2]['$'] = 3;
    state_matrix[4]['$'] = 5;
    state_matrix[6]['$'] = 7;
    state_matrix[8]['$'] = 9;
    state_matrix[11]['$'] = 12;
    state_matrix[13]['$'] = 15;
    state_matrix[14]['$'] = 15;
    state_matrix[16]['$'] = 16;
    state_matrix[17]['$'] = 18;

    for (char i = 'a'; i <= 'z'; i++) {
        state_matrix[1][i] = state_matrix[1][(int)i - 32] = 2;
        state_matrix[2][i] = state_matrix[2][(int)i - 32] = 2;
    }

    for (char i = '0'; i <= '9'; i++) {
        state_matrix[1][i] = state_matrix[4][i] = 4;
        for (char j = 'a'; j <= 'z'; j++) {
            state_matrix[4][j] = state_matrix[4][(int)j - 32] = 0;
            state_matrix[6][j] = state_matrix[6][(int)j - 32] = 0;
        }
        state_matrix[6][i] = 6;
    }
    state_matrix[4]['.'] = 6;
    
    state_matrix[1]['-'] = state_matrix[1]['+'] = 8;
    state_matrix[1]['/'] = state_matrix[1]['*'] = 8;
    state_matrix[1]['%'] = state_matrix[1]['^'] = 8;

    //state_matrix[1].push_back(9); plus assign
    state_matrix[1]['='] = 11;
    state_matrix[1]['<'] = state_matrix[1]['>'] = state_matrix[1]['!'] = 14;
    state_matrix[11]['='] = state_matrix[14]['='] = 13;

    state_matrix[1][(char)39] = 16;
    state_matrix[16][(char)39] = 17;
}

/*
Funcao para ler o programa fonte
Parametros:
    filename -> Nome do arquivo com o programa fonte
*/
void lexer::read_file(std::string const& filename) {
    //Abrir arquivo
    std::ifstream File(filename);
    if (!File.good()) {
        std::cerr << "ERRO: Arquivo nao existe\n";
        abort();
    }

    
    std::string aux, str;
    bool comented = false, is_str = false;
    //Ler arquivos e imprimir no novo arquivo
    while (std::getline(File, str)) {
        //Analisa a string
        aux = analyse_str(str, comented, is_str);
        if (!aux.empty())   //Se tiver algo adiciona
            content.push_back(aux);
    }
    //Adicionar quebra de linha caso nao tenha
    if (content.empty() || content.back().back() != '\n')
        content.push_back("\n");
}

/*
Função para analisar a string
Parametros:
    str      -> string para analisar
    comented -> se está dentro de um bloco comentado
    is_str   -> se está dentro de uma frase
*/
std::string lexer::analyse_str(std::string const& str, bool& comented, bool&is_str) {
    std::string ans = "";                       //Nova string
    int cnt = 0, size_str = (int)str.size();    //Contador de espaços e tamanho da string
    for (int i = 0; i < size_str; i++) {
        if (is_str) {
            ans += str[i];
            is_str = bool(str[i] != (char)39);
            continue;
        }
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
        if (str[i] == (char)39) is_str = true;
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

void lexer::add_error(std::string str, token &tk) {
    errors.push_back({ line, pos, str });
    tk.set_type("ERROR");
}

//Funcao para pegar o proximo caracter do arquivo
char lexer::next_char() {
    char ans = content[line][pos++];
    if (pos == (int)content[line].size()) {
        line++;
        pos = 0;
    }
    return ans;
}

//Funcao para pegar o proximo token
token lexer::next_token() {
    state = 1;
    char cur;
    std::string ans = "";
    token tk;
    bool ok = true;
    //Analisar caracteres ate chegar no eof ou em um estado final
    while (!is_eof() && ok) {
        cur = next_char();                      //Pega o prox caracter
        if (state_matrix[state].count(cur)) {   //Se tiver transicao vai pra ela
            state = state_matrix[state][cur];
        }
        else {
            state = state_matrix[state]['$'];   //Se nao tiver vai pra erro (16 nao tem erro)
        }
        ok &= (bool)((bool)state && !terminal_states[state]);
        //Adiciona a string resultante se estiver em um estado maior que 1 ou
        //se nao for espaco no primeiro estado
        if (state > 1 || state == 1 && !is_space(cur) || !ok)
            ans += cur;
    }
    
    if (terminal_states[state]) { //Se for estado terminal deu bom
        switch (state) {
        //Estado 3 (Identificador)
        case 3:
            backtrack(ans); //Recupera Caracter
            if (reserved_words.count(ans)) {
                tk = token(ans, ans, { line, pos });
            }
            else {
                if (!hash_by_word.count(ans)) { //Adiciona palavra na hash
                    hash_by_word[ans] = (int)hash_by_word.size();
                }
                tk = token("id", std::to_string(hash_by_word[ans]), { line, pos });
            }
            break;
        //Estado 5(Numero inteiro)
        case 5:
            backtrack(ans);
            if ((int)ans.size() > 9) {
                add_error(ans, tk);
            }
            else {
                tk = token("INumber", ans, { line, pos });
            }
            break;
        //Estado 7 (Numero com Ponto Flutuante)
        case 7:
            backtrack(ans);
            if ((int)ans.size() > 9) {
                add_error(ans, tk);
            }
            else {
                tk = token("FNumber", ans, { line, pos });
            }
            break;
        //Estado 9 (Operador matematico)
        case 9:
            backtrack(ans);
            tk = token("opM", MOperators[ans], { line, pos });
            break;
        //Estado 12 (Exprecao de atribuicao)
        case 12:
            backtrack(ans);
            tk = token("=", ans, { line, pos });
            break;
        //Estado 15 (Operador de relacao)
        case 15:
            backtrack(ans);
            tk = token("opL", LOperators[ans], { line, pos });
            break;
        //Estado 18 (Frase)
        case 18:
            backtrack(ans);
            tk = token("Frase", ans, { line, pos });
            break;
        //Estado 19 (Chaves/Parenteses)
        case 19:
            tk = token(ans, ans, { line, pos });
            break;
        //Estado 20 (Delimitadores)
        case 20:
            tk = token(ans, ans, { line, pos });
            break;
        }
    }
    else if (!ans.empty()){ //Senao deu ruim
        add_error(ans, tk);
    }
    if (is_eof()) tk.set_type("NULL");

    return tk;
}

/*
Funcao para verificar se e espaco
Parametros:
    c -> caracter a ser analisado
*/
bool lexer::is_space(char const& c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

//Funcao para verificar se chegou no fim do arquivo
bool lexer::is_eof() {
    return line == (int)content.size();
}

/*
Funcao para retroceder caracter
Parametros:
    ans -> lexema com caracter alheio
*/
void lexer::backtrack(std::string &ans) {
    if (!ans.empty()) {
        ans.pop_back();
    }
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

/*
Funcao para printar erros
Parametros:
    os -> arquivo de saida
*/
void lexer::printerrors(std::ostream& os) {
    if (errors.empty()) {
        os << "Any errors/warnings\n"; return;
    }
    os << "Errors/Warnings:\n";
    for (auto &[li, col, cara] : errors) {
        os << "Linha: " << li << std::setw(12)
           << "Coluna: " << col << std::setfill('.') << std::setw(7)
           << ": " << cara << '\n';
    }
}
