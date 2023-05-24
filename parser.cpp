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
    terminals = { "id", "constante", "while", "if", "else", "read", "print"
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

void parser::exc_error(std::string at, std::string aux, std::pair<int, int> posi, int &id) {
	auto [line, col] = posi;
	if (at == ")") {
		errors.push_back({ line, col, "Missing (" });
	}
	else if (at == "}") {
		errors.push_back({ line, col, "Missing {" });
	}
	else if (at == "<exp'>" && (aux == "constante" || aux == "id")) {
		errors.push_back({ line, col, "Missing operand" });
	}
	else if (at == "def") {
		errors.push_back({ line, col, "Code outside a function" });
	}
	else {
		if (at == "$") {
			errors.push_back({ line, col, "Found EOF not expected" });
		}
		else {
			errors.push_back({ line, col, "ERRO!" });
		}
		id++;
	}
}

/*
Analisador sintatico (futuramente com o semantico)
Parametros:
	Arquivo de saida do analisador sintatico
*/
SyntaxTree parser::work(std::ofstream &outFile, HashMatrix& symbol_table,
						std::unordered_map<std::string, std::vector<std::string>, lexer::custom_hash>& funcs) {
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
	
	Node* p = new Node("$", "global");
	AST.add_node(p);
	Node* tree_node = AST.get_root();
	p = new Node("<func>", "global", tree_node);
	tree_node->add_child(p);

	//auto add_node = [&]() {

	//};
	
	int id = 0;
	bool inside_decl = false, function_decl = false, function_type = false;
	std::string decl_type = "none", func_name = "none";
	while(!st.empty()) {
		print_stack(st);
		outFile << tokens[id] << "\n\n";
		//Pegar o topo da pilha e simbolo para analisar
		std::string at = st.top(); st.pop();
		std::string aux = tokens[id].get_type();
		if (aux == "FNumber" || aux == "INumber" || aux == "Frase") {
			aux = "constante";
		}
		
		if (at == aux) { //terminal com terminal igual
			if (aux == "constante") {
				if (tokens[id].get_type() == "FNumber") {
					aux = "float";
				}
				else if (tokens[id].get_type() == "INumber") {
					aux = "int";
				}
				else {
					aux = "string";
				}
			}
			/*else if (aux == "id") {
				aux = tokens[id].get_text();
			}
			if (aux == "opM") {
				aux = tokens[id].get_text();
			}
			(tree_node->children[tree_node->id_child])->set_valor(aux);
			(tree_node->children[tree_node->id_child++])->set_escopo("global");

			if (tree_node->id_child == (int)tree_node->children.size())
				tree_node = tree_node->get_parent();*/
			if (inside_decl && aux != ",") {
				if (aux == ";") {
					inside_decl = false;
				}
				else {
					if (aux != "id") {
						decl_type = tokens[id].get_text();
					}
					else {
						std::string id_name = tokens[id].get_text(),
								posi = "line " + std::to_string(tokens[id].get_pos().second) + " col " + std::to_string(tokens[id].get_pos().first);
						if (symbol_table["global"].find(id_name) != symbol_table["global"].end()) {
							std::cout << "Variavel ja declarada: " << symbol_table["global"][id_name][1] << '\n';
						}
						else {
							symbol_table["global"][id_name] = { decl_type, posi };
						}
					}
				}
			}
			else if (aux == "def") {
				function_decl = true;
			}
			else if (function_decl) {
				std::string id_name = tokens[id].get_text(),
					posi = "line " + std::to_string(tokens[id].get_pos().second) + " col " + std::to_string(tokens[id].get_pos().first);
				if (funcs.find(id_name) != funcs.end()) {
					std::cout << "Funcao ja declarada: " << symbol_table["global"][id_name][1] << '\n';
				}
				else {
					funcs[id_name] = { "none", posi};
				}
				func_name = id_name;
				function_decl = false;
			}
			else if (aux == "return") {
				function_type = true;
			}
			else if (function_type) {
				if (aux == "id") {
					funcs[func_name][0] = symbol_table["global"][tokens[id].get_text()][0];
				}
				else {
					funcs[func_name][0] = aux;
				}
				function_type = false;
			}
			else if (aux == "id" && symbol_table["global"].find(tokens[id].get_text()) == symbol_table["global"].end()) {
				std::cout << "Variavel nao declarada!!\n";
			}
			id++;
		}
		else if (at[0] != '<') { //terminal com coisa diferente
			if (at == "empty") continue;
			//erro analise sintatica
			exc_error(at, aux, tokens[id].get_pos(), id);
		}
		else{ //Nao terminal com outra coisa
			if (predictive_table[at][aux].empty()){ //Se tiver numa celula vazia deu ruim
				exc_error(at, aux, tokens[id].get_pos(), id);
			}
			else { //Remover nao terminal da pilha e adicionar a producao formada na pilha
				//tree_node = tree_node->children[tree_node->id_child++];
				if (at == "<decl>") {
					inside_decl = true;
				}
				int sz = (int)predictive_table[at][aux].size() - 1;
				for (int i = sz; i >= 0; i--) {
					st.push(predictive_table[at][aux][i]);
					//Node* cria = new Node(predictive_table[at][aux][sz - i], "global", tree_node);
					//tree_node->add_child(cria);
				}
			}
		}
	}

	return AST;
}
