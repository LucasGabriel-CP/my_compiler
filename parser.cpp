#include "parser.h"

parser::parser(){ }

void parser::add_prod(std::string row, std::string col, std::vector<std::string> res) {
	predictive_table[row][col].insert(predictive_table[row][col].end(), res.begin(), res.end());
}

/*
Construtor do parser
Parametros:
	tokens: tokens analisados no analisador lexico
*/
parser::parser(std::vector<token> const& tokens) {
	//Uso futuro
    symbol_table.resize((int)lexer::hash_by_word.size());
    std::vector<std::string> terminals = { "id", "constante", "while", "if", "else", "read", "print"
                                        ")", "(", "{", "}", ",", ";", "int", "float", "string",
                                        "=", "opM", "opL", "def", "return", "$"};
    
	//Atribuir os tokens e o simbolo $ no fim
	this->tokens = tokens;
    this->tokens.push_back(token("$", "$", std::pair<int, int>(-1, -1)));
    
	//Construção da tabela

	predictive_table["<termo>"] = {
		{ "id", {"id"} }, {"constante", {"constante"}}, {"(", {"(", "<exp>", ")"}}
	};
	predictive_table["<func>"] = {
		{ "def", {"def", "id", "(", "<arg>", ")", "{", "<bloco>", "}"}}, {"$", {"empty"}}
	};
	predictive_table["<arg>"] = {
		{"int", {"int", "id"}}, {"float", {"float", "id"}},
		{"string", {"string", "id"}}, {")", {"empty"}}
	};

    predictive_table["<bloco>"] = {
		{"id", {"<comando>", "<bloco>"}}, {"while", {"<comando>", "<bloco>"}}, {"if", {"<comando>", "<bloco>"}},
		{"read", {"<comando>", "<bloco>"}}, {"print", {"<comando>", "<bloco>"}}, {"int", {"<comando>", "<bloco>"}},
		{"float", {"<comando>", "<bloco>"}}, {"string", {"<comando>", "<bloco>"}}, {"}", {"empty"}}, {"$", {"empty"}},
		{"return", {"<comando>", "<bloco>"}}
    };
	predictive_table["<comando>"] = {
		{"id", {"<exp_atrib>"}}, {"while", {"<while>"}}, {"if", {"<if>"}}, {"read", {"<read>"}},
		{"print", {"<print>"}}, {"int", {"<decl>"}}, {"float", {"<decl>"}}, {"string", {"<decl>"}},
		{"return", {"<return_exp>"} }
	};
    predictive_table["<while>"] = {
		{"while", {"while", "(", "<exp>", ")", "{", "<bloco>", "}"}}
	};
    predictive_table["<if>"] = {
		{"if", {"if", "(", "<exp>", ")", "{", "<bloco>", "}", "<else>"}}
	};
    predictive_table["<else>"] = {
		{"while", {"empty"}}, {"if", {"empty"}}, {"read", {"empty"}}, {"print", {"empty"}}, {"}", {"empty"}},
		{"int", {"empty"}}, {"float", {"empty"}}, {"string", {"empty"}}, {"$", {"empty"}},
        {"else", {"else", "{", "<bloco>", "}"}}
    };
    predictive_table["<read>"] = {
		{"read", {"read", "(", "id", "<read'>", ")", ";"}}
	};
    predictive_table["<read'>"] = {
		{")", {"empty"}}, {",", {",", "id", "<read'>"}}
	};
    predictive_table["<print>"] = {
		{"print", {"print", "(", "<termo>", "<print'>", ")", ";"}}
	};
    predictive_table["<print'>"] = {
		{")", {"empty"}}, {",", {",", "<termo>", "<print'>"}}
	};
    predictive_table["<decl>"] = {
		{"int", {"int", "id", "<decl'>", ";"}}, {"float", {"float", "id", "<decl'>", ";"}},
		{"string", {"string", "id", "<decl'>", ";"}}
	};
    predictive_table["<decl'>"] = {
		{",", {",", "id", "<decl'>"}}, {";", {"empty"}}
	};
    predictive_table["<exp_atrib>"] = {
		{"id", {"id", "=", "<exp>", ";"}}
	};
    predictive_table["<exp>"] = {
		{"id", {"<termo>", "<exp'>"}}, {"constante", {"<termo>", "<exp'>"}}, {"(", {"<termo>", "<exp'>"}}
	};
    predictive_table["<exp'>"] = {
		{";", {"empty"}}, {"opM", {"opM", "<termo>", "<exp'>"}},
		{"opL", {"opL", "<termo>"}}, {")", {"empty"}}
	};
	predictive_table["<return_exp>"] = {
		{"return", {"return", "<termo>", ";"}}
	};
}

void parser::exc_error(std::string at, std::string aux, int &id) {
	if (at == ")") {
		std::cout << "Missing )\n";
	}
	else if (at == "}") {
		std::cout << "Missing }\n";
	}
	else if (at == "<exp'>" && (aux == "constante" || aux == "id")) {
		std::cout << "Missing operand\n";
	}
	else if (at == "def") {
		std::cout << "Code outside a function\n";
	}
	else {
		if (at == "$") {
			std::cout << "Found EOF not expected\n";
		}
		else {
			std::cout << "ERRO!\n";
		}
		id++;
	}
}

/*
Analisador sintatico (futuramente com o semantico)
Parametros:
	Arquivo de saida do analisador sintatico
*/
void parser::work(std::ofstream &outFile){
	//Funcao lambda pra printar a pilha do analisador
	auto print_stack = [&](std::stack<std::string> s) {
		for (; !s.empty(); s.pop()) outFile << s.top() << ' ';
		outFile << '\n';
	};
	//Pilha do analisador
	std::stack<std::string> st;
	//Adicionar o simbolo $ e o simbolo inicial
	st.push("$");
	st.push("<func>");
	int id = 0;
	while(!st.empty()){
		print_stack(st);
		outFile << tokens[id] << "\n\n";
		//Pegar o topo da pilha e simbolo para analisar
		std::string at = st.top(); st.pop();
		std::string aux = tokens[id].get_type();
		if (aux == "FNumber" || aux == "INumber" || aux == "Frase") {
			aux = "constante";
		}
		
		if (at == aux){ //terminal com terminal igual
			id++;
		}
		else if (at[0] != '<'){ //terminal com coisa diferente
			if (at == "empty") continue;
			//erro analise sintatica
			exc_error(at, aux, id);
		}
		else{ //Nao terminal com outra coisa
			if (predictive_table[at][aux].empty()){ //Se tiver numa celula vazia deu ruim
				exc_error(at, aux, id);
			}
			else { //Remover nao terminal da pilha e adicionar a producao formada na pilha
				for (int i = (int)predictive_table[at][aux].size() - 1; i >= 0; i--) {
					st.push(predictive_table[at][aux][i]);
				}
			}
		}
	}
}
