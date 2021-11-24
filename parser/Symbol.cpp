#include "Symbol.h"
#include<memory>
Symbol make_dollar_symbol(int id) {
	return Symbol(Symbol::TYPE::eof, "$",id);
}
Symbol make_null_symbol(int id) {
	return Symbol(Symbol::TYPE::null, "ε",id);
}

vector<Symbol> string2symbol(const string str) {
	vector<Symbol> v;
	for (auto& ch : str) {
		v.emplace_back(Symbol::TYPE::terminal, string{ ch });
	}
	return v;
}