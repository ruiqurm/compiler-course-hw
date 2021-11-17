#pragma once
#include<string>
using std::string;
struct Symbol
{	

	enum class TYPE
	{
		nonterminal,
		terminal,
		null,
		eof
	};
	Symbol() :type(TYPE::null), description("ε") {}
	Symbol(TYPE type, const char* description) :
		description(description), type(type){}
	Symbol(TYPE type, const string description) :
		description(description), type(type) {}
	TYPE type;
	string description;

	bool is_terminal()const {
		return type == TYPE::terminal;
	}
	bool is_nonterminal()const {
		return type == TYPE::nonterminal;
	}
	bool is_null()const {
		return type == TYPE::null;
	}
	bool operator!=(const Symbol& sym)const {
		return description != sym.description;
	}
	bool operator==(const Symbol& sym)const {
		return description == sym.description;
	}
	bool operator <(const Symbol& sym)const {
		return description < sym.description;
	}
};

