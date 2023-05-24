#include "HashMatrix.h"

std::string HashMatrix::add_id(std::string id_name, std::string decl_type, std::string scope, std::pair<int, int> posi) {
	std::string local = "line " + std::to_string(posi.first) + " col " + std::to_string(posi.second);
	if (table[scope].find(id_name) != table[scope].end()) {
		return "Variavel ja declarada: " + table[scope][id_name][1];
	}
	table["global"][id_name] = { decl_type, local };
	return "";
}

bool HashMatrix::check_decl(std::string id_name, std::string scope) {
	return table[scope].find(id_name) != table[scope].end();
}

HashMatrix::HashMatrix() { }

HashMatrix::~HashMatrix() { }
