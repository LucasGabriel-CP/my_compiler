#include "parser.h"

parser::parser(){ }

parser::parser(std::vector<token> const& tokens) {
    symbol_table.resize((int)hash_by_value.size());
    std::vector<std::string> terminals = { "id", "constante", "while", "if", "else", "read", "print"
                                        ")", "(", "{", "}", ",", ";", "int", "float", "string",
                                        "=", "opM", "opL", "$" };
    this->tokens = tokens;
    this->tokens.push_back(token("$", "$", std::pair<int, int>(-1, -1)));
    //Construção da tabela
    predictive_table["<termo>"] = { { "id", {"id"} }, {"constante", {"constante"}} };
    predictive_table["<bloco>"] = { {"id", {"<comando>", "<bloco>"}}, {"while", {"<comando>", "<bloco>"}},
		{"if", {"<comando>", "<bloco>"}},
        {"read", {"<comando>", "<bloco>"}}, {"print", {"<comando>", "<bloco>"}},
        {"int", {"<comando>", "<bloco>"}}, {"float", {"<comando>", "<bloco>"}}, {"string", {"<comando>", "<bloco>"}},
        {"}", {"empty"}}, {"$", {"empty"}}
    };
    predictive_table["<comando>"] = { {"id", {"<exp_atrib>"}}, {"while", {"<while>"}}, {"if", {"<if>"}},
        {"read", {"<read>"}}, {"print", {"<print>"}}, {"int", {"<decl>"}}, {"float", {"<decl>"}}, {"string", {"<decl>"}}
    };
    predictive_table["<while>"] = { {"while", {"while", "(", "<exp>", "{", "<bloco>", "}", "<else>"}}};
    predictive_table["<if>"] = { {"if", {"if", "(", "<exp>", ")", "{", "<bloco>", "}", "<else>"}}};
    predictive_table["<else>"] = { {"while", {"empty"}}, {"if", {"empty"}}, {"read", {"empty"}}, {"print", {"empty"}},
        {"}", {"empty"}}, {"int", {"empty"}}, {"float", {"empty"}}, {"string", {"empty"}}, {"$", {"empty"}},
        {"else", {"else", "{", "<bloco>", "}"}}
    };
    predictive_table["<read>"] = { {"read", {"read", "(", "id", "<read'>", ")", ";"}} };
    predictive_table["<read'>"] = { {")", {"empty"}}, {",", {",", "id", "<read'>"}} };
    predictive_table["<print>"] = { {"print", {"print", "(", "<termo>", "<print'>", ")", ";"}} };
    predictive_table["<print'>"] = { {")", {"empty"}}, {",", {",", "<termo>", "<print'>"}} };
    predictive_table["<decl>"] = { {"int", {"int", "id", "<decl'>", ";"}}, {"float", {"float", "id", "<decl'>", ";"}},
		{"string", {"string", "id", "<decl'>", ";"}}
	};
    predictive_table["<decl'>"] = { {",", {",", "id", "<decl'>"}}, {";", {"empty"}} };
    predictive_table["<exp_atrib>"] = { {"id", {"id", "=", "<exp>", ";"}} };
    predictive_table["<exp>"] = { {"id", {"<termo>", "<exp'>"}}, {"constante", {"<termo>", "<exp'>"}} };
    predictive_table["<exp'>"] = { {";", {"empty"}}, {"opM", {"opM", "<termo>", "<exp'>"}}, {"opL", {"opL", "<termo>"}} };
}

void parser::work(std::ofstream &outFile){
	std::stack<std::string> st;
	auto print_stack = [&](std::stack<std::string> s) {
		for (; !s.empty(); s.pop()) outFile << s.top() << ' ';
		outFile << '\n';
	};
	st.push("$");
	st.push("<bloco>");
	int id = 0;
	while(!st.empty()){
		print_stack(st);
		outFile << tokens[id] << "\n\n";
		std::string at = st.top(); st.pop();
		std::string aux = tokens[id].get_type();
		if (aux == "FNumber" || aux == "INumber" || aux == "Frase") {
			aux = "constante";
		}
		if (at == aux){
			//std::cout << tokens[id] << '\n';
			id++;
			if (at == "$"){
				continue;
			}
			//terminal com terminal
		}
		else if (at[0] != '<'){
			if (at == "empty") continue;
			//erro analise sintatica
			std::cout << "Missing terminal for " << at << '\n'
				<< "Current terminal: " << aux << '\n';
			id++;
		}
		else{
			if (predictive_table[at][aux].empty()){
				std::cout << at << ' ' << aux << '\n';
				std::cout << "deu merda na analise\n";
			}
			else {
				for (int i = (int)predictive_table[at][aux].size() - 1; i >= 0; i--) {
					st.push(predictive_table[at][aux][i]);
				}
			}
		}
	}
}
