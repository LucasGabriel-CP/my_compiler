#include "HashMatrix.h"

std::string HashMatrix::add_id(std::string id_name, std::string decl_type, std::string scope, std::pair<int, int> posi) {
	std::string local = "line " + std::to_string(posi.first) + " col " + std::to_string(posi.second), sc = scope;
	while (sc != "NULL") {
		if (table[sc].find(id_name) != table[sc].end()) {
			return "Variavel [" + id_name + "] ja declarada: " + table[sc][id_name][1];
		}
		sc = parent[sc];
	}
	table[scope][id_name] = { decl_type, local };
	return "";
}

bool HashMatrix::check_decl(std::string id_name, std::string scope, std::string &par) {
	while (scope != "NULL") {
		if (table[scope].find(id_name) != table[scope].end()) {
			par = scope;
			return true;
		}
		scope = parent[scope];
	}
	return false;
}

HashMatrix::HashMatrix() { }

HashMatrix::~HashMatrix() { }
