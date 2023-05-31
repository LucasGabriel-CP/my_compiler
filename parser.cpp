#include "parser.h"

parser::parser(){ }

void parser::add_prod(std::string row, std::string col, std::vector<std::string> res) {
	//predictive_table[row][col].insert(predictive_table[row][col].end(), res.begin(), res.end());
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
    
	//----------------------------------------------------Inicio da Construcao da tabela----------------------------------------------------
	predictive_table.table["<termo>"] = {
		{ "id", {"id"} }, {"constante", {"constante"}},
		{"(", {"(", "<exp>", ")"}}, {"#", {"<func_cal>"}}
	};
	predictive_table.table["<func'>"] = {
		{ "def", {"<func>"}}
	};
	predictive_table.table["<func>"] = {
		{ "def", {"def", "id", "(", "<arg>", ")", "{", "<bloco>", "}", "<func>"}}, {"$", {"empty"}}
	};
	predictive_table.table["<arg>"] = {
		{"int", {"int", "id"}}, {"float", {"float", "id"}},
		{"string", {"string", "id"}}, {")", {"empty"}}
	};

	predictive_table.table["<bloco>"] = {
		{"id", {"<comando>", "<bloco>"}}, {"while", {"<comando>", "<bloco>"}}, {"if", {"<comando>", "<bloco>"}},
		{"read", {"<comando>", "<bloco>"}}, {"print", {"<comando>", "<bloco>"}}, {"int", {"<comando>", "<bloco>"}},
		{"float", {"<comando>", "<bloco>"}}, {"string", {"<comando>", "<bloco>"}}, {"}", {"empty"}}, {"$", {"empty"}},
		{"return", {"<comando>", "<bloco>"}}, {"#", {"<comando>", "<bloco>"}}
	};
	predictive_table.table["<comando>"] = {
		{"id", {"<exp_atrib>"}}, {"while", {"<while>"}}, {"if", {"<if>"}}, {"read", {"<read>"}},
		{"print", {"<print>"}}, {"int", {"<decl>"}}, {"float", {"<decl>"}}, {"string", {"<decl>"}},
		{"return", {"<return_exp>"}}, {"#", {"<exec_func>"}}
	};
    predictive_table.table["<while>"] = {
		{"while", {"while", "(", "<exp>", ")", "{", "<bloco>", "}"}}
	};
    predictive_table.table["<if>"] = {
		{"if", {"if", "(", "<exp>", ")", "{", "<bloco>", "}", "<else>"}}
	};
	predictive_table.table["<else>"] = {
		{"while", {"empty"}}, {"if", {"empty"}}, {"read", {"empty"}}, {"print", {"empty"}}, {"}", {"empty"}},
		{"int", {"empty"}}, {"float", {"empty"}}, {"string", {"empty"}}, {"$", {"empty"}},
		{"else", {"else", "{", "<bloco>", "}"}}, {"return", {"empty"}}, {"#", {"empty"}}
	};
    predictive_table.table["<read>"] = {
		{"read", {"read", "(", "id", "<read'>", ")", ";"}}
	};
    predictive_table.table["<read'>"] = {
		{")", {"empty"}}, {",", {",", "id", "<read'>"}}
	};
    predictive_table.table["<print>"] = {
		{"print", {"print", "(", "<termo>", "<print'>", ")", ";"}}
	};
    predictive_table.table["<print'>"] = {
		{")", {"empty"}}, {",", {",", "<termo>", "<print'>"}}
	};
    predictive_table.table["<decl>"] = {
		{"int", {"int", "id", "<decl'>", ";"}}, {"float", {"float", "id", "<decl'>", ";"}},
		{"string", {"string", "id", "<decl'>", ";"}}
	};
    predictive_table.table["<decl'>"] = {
		{",", {",", "id", "<decl'>"}}, {";", {"empty"}}
	};
    predictive_table.table["<exp_atrib>"] = {
		{"id", {"id", "=", "<exp>", ";"}}
	};
    predictive_table.table["<exp>"] = {
		{"id", {"<termo>", "<exp'>"}}, {"constante", {"<termo>", "<exp'>"}},
		{"(", {"<termo>", "<exp'>"}}, {"#", {"<termo>", "<exp'>"}}
	};
    predictive_table.table["<exp'>"] = {
		{";", {"empty"}}, {"opM", {"opM", "<termo>", "<exp'>"}},
		{"opL", {"opL", "<termo>"}}, {")", {"empty"}}
	};
	predictive_table.table["<return_exp>"] = {
		{"return", {"return", "<termo>", ";"}}
	};
	predictive_table.table["<exec_func>"] = {
		{"#", {"<func_call>", ";"}}
	};
	predictive_table.table["<func_cal>"] = {
		{"#", {"#", "id", "(", "<call_args>", ")"}}
	};
	predictive_table.table["<call_args>"] = {
		{"id", {"<termo>", "<call_args'>"}}, {"constante", {"<termo>", "<call_args'>"}},
		{"(", {"<termo>", "<call_args'>"}}, {")", {"empty"}}
	};
	predictive_table.table["<call_args'>"] = {
		{",", {"<termo>", "<call_args'>"}}, {")", {"empty"}}
	};

	//-------------------------------------------------------Fim da Construcao da tabela-------------------------------------------------------

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
	else if (at == "id") {
		errors.push_back({ line, col, "Missing identificator" });
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

std::string parser::get_real_type(std::string str) {
	if (str == "FNumber") {
		return "float";
	}
	if (str == "INumber") {
		return "int";
	}
	return "string";
}

/*
Analisador sintatico (futuramente com o semantico)
Parametros:
	Arquivo de saida do analisador sintatico
*/
SyntaxTree parser::work(std::ofstream &outFile) {
	//Funcao lambda pra printar a pilha do analisador
	auto print_stack = [&](std::stack<std::string> s) {
		for (; !s.empty(); s.pop()) outFile << s.top() << ' ';
		outFile << '\n';
	};
	
	//Pilha do analisador
	std::stack<std::string> st;
	//Adicionar o simbolo $ e o simbolo inicial
	st.push("$");
	st.push("<func'>");
	
	//Criando a Arvore Sintatica
	Node* p = new Node(token());
	AST.add_node(p);
	Node* tree_node = AST.get_root();

	int id = 0;
	/*bool inside_decl, function_id, function_type, function_decl;
	auto reset_levers = [&]() {
		inside_decl = function_id = function_type = function_decl = function_args = false;
	};
	reset_levers();
	std::string decl_type = "none", func_name = "none", func_to_call;*/

	//Inicio da analise sintatica
	while(!st.empty()) {
		//Printa a pilha e a entrada
		print_stack(st);
		outFile << tokens[id] << "\n\n";
		
		//Pegar o topo da pilha e simbolo para analisar
		std::string at = st.top(); st.pop();
		std::string aux = tokens[id].get_type();
		if (aux == "FNumber" || aux == "INumber" || aux == "Frase") {
			aux = "constante";
		}
		
		if (at == aux) { //terminal com terminal igual
			/*if (inside_decl && aux != ",") {
				if (aux == ";") {
					inside_decl = false;
				}
				else {
					if (aux != "id") {
						decl_type = tokens[id].get_text();
					}
					else {
						std::string err = symbol_table.add_id(tokens[id].get_text(), "global", func_name, tokens[id].get_pos());
						if (!err.empty()) {
							auto [x, y] = tokens[id].get_pos();
							errors.push_back({ x, y, "Variavel ja declarada!!" });
						}
					}
				}
			}
			else if (aux == "def") {
				function_id = true;
			}
			else if (function_id) {
				std::string id_name = tokens[id].get_text(),
					posi = "line " + std::to_string(tokens[id].get_pos().second) + " col " + std::to_string(tokens[id].get_pos().first);
				if (funcs.find(id_name) != funcs.end()) {
					std::cout << "Funcao ja declarada: " << funcs[id_name][1] << '\n';
				}
				else {
					funcs[id_name] = { "none", posi};
				}
				func_name = id_name;
				function_id = false;
			}
			else if (aux == "return") {
				function_type = true;
			}
			else if (function_type) {
				if (aux == "id") {
					std::string tp = symbol_table.table["global"][tokens[id].get_text()][0];
					if (tp != "none") {
						if (funcs[func_name][0] != "none" && tp != funcs[func_name][0]) {
							auto [x, y] = tokens[id].get_pos();
							errors.push_back({ x, y, "multiple returning types" });
						}
						funcs[func_name][0] = tp;
					}
				}
				else {
					std::string tp = get_real_type(tokens[id].get_type());
					if (funcs[func_name][0] != "none" && tp != funcs[func_name][0]) {
						auto [x, y] = tokens[id].get_pos();
						errors.push_back({ x, y, "multiple returning types" });
					}
					funcs[func_name][0] = get_real_type(tokens[id].get_type());
				}
				function_type = false;
			}
			else if (function_decl) {
				if (at == "id") {
					std::string err = symbol_table.add_id(tokens[id].get_text(), decl_type, func_name, tokens[id].get_pos());
					funcs[func_name].push_back(decl_type);
					if (!err.empty()) {
						auto [x, y] = tokens[id].get_pos();
						errors.push_back({ x, y, err });
					}
				}
				else if (at == ")") {
					function_decl = false;
				}
				else {
					decl_type = at;
				}
			}
			else if (aux == "id") {
				if (!symbol_table.check_decl(tokens[id].get_text(), "global") && funcs.find(tokens[id].get_text()) == funcs.end()) {
					auto [x, y] = tokens[id].get_pos();
					errors.push_back({ x, y, "Variavel nao declarada!!" });
				}
			}*/
		
			if (tree_node->id_child < (int)tree_node->children.size()) {
				(tree_node->children[tree_node->id_child])->set_valor(tokens[id].get_text());
				(tree_node->children[tree_node->id_child])->set_tipo(tokens[id].get_type());
			}

			// Recursao para "regenerar" ate o no da producao atual na arvore sintatica
			auto restore = [&](auto& self) ->void {
				//Passar pro proximo filho
				tree_node->id_child++;
				//Subir na arvore ate chegar em um no que ainda tenha filhos a serem analisados
				while (tree_node->get_parent() && tree_node->id_child == (int)tree_node->children.size()) {
					tree_node = tree_node->get_parent();
				}

				// Se o filho atual for um nao terminal e ainda tiver producao que pode
				// nao ter sido analisada, vai pra ele e verifica
				if (tree_node->id_child < (int)tree_node->children.size()
					&& (tree_node->children[tree_node->id_child])->get_tipo()[0] == '<') {
					if (!(tree_node->children[tree_node->id_child])->is_leaf() &&
						(tree_node->children[tree_node->id_child])->id_child == (int)(tree_node->children[tree_node->id_child])->children.size()) {
						self(self);
					}
					else {
						tree_node = tree_node->children[tree_node->id_child];
					}
				}
			};
			restore(restore);

			id++;
		}
		else if (at[0] != '<') { //terminal com coisa diferente
			tree_node->id_child++;
			while (tree_node->get_parent() && tree_node->id_child >= (int)tree_node->children.size()) {
				tree_node = tree_node->get_parent();
				tree_node->id_child++;
			}
			if (at == "empty") continue;
			//reset_levers();
			//erro analise sintatica
			exc_error(at, aux, tokens[id].get_pos(), id);
		}
		else{ //Nao terminal com outra coisa
			if (predictive_table.table[at][aux].empty()){ //Se tiver numa celula vazia deu ruim
				if (tree_node->get_parent() && tree_node->id_child == (int)tree_node->children.size()) {
					tree_node = tree_node->get_parent();
				}
				//reset_levers();
				exc_error(at, aux, tokens[id].get_pos(), id);
			}
			else { //Remover nao terminal da pilha e adicionar a producao formada na pilha
				/*if (at == "<decl>") {
					inside_decl = true;
				}
				else if (at == "<arg>") {
					function_decl = true;
				}*/
				
				// Armazenar as producoes na pilha e criar aresta entre o pai e cada
				//producao que ele vai gerar
				int sz = (int)predictive_table.table[at][aux].size() - 1;
				for (int i = sz; i >= 0; i--) {
					std::string prod = predictive_table.table[at][aux][sz - i];
					st.push(predictive_table.table[at][aux][i]);
					token tk;
					tk.set_text(prod);
					if (prod[0] != '<') {
						if (prod == "constante") {
							prod = get_real_type(tokens[id].get_type());
						}
						tk.set_pos(tokens[id].get_pos());
						tk.set_text(tokens[id].get_text());
					}
					tk.set_type(prod);
					Node* cria = new Node(tk, tree_node);
					tree_node->add_child(cria);
				}

				// Se o filho de agora for um nao terminal, passa pra ele
				if ((tree_node->children[tree_node->id_child])->get_tipo()[0] == '<') {
					tree_node = tree_node->children[tree_node->id_child++];
				}
			}
		}
	}

	return AST;
}

/*
Funcao para printar erros
Parametros:
	os -> arquivo de saida
*/
void parser::printerrors(std::ostream& os) {
	if (errors.empty()) {
		os << "Any errors/warnings\n"; return;
	}
	os << "Syntatic Errors/Warnings:\n";
	for (auto& [li, col, cara] : errors) {
		os << "Linha: " << li << std::setw(12)
			<< "Coluna: " << col << std::setfill('.') << std::setw(7)
			<< ": " << cara << '\n';
	}
}
