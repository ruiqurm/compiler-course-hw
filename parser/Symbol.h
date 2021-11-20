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
	Symbol() :type(TYPE::null), description("ε"),id(-1) {}
	Symbol(TYPE type, const char* description,int id=-1) :
		description(description), type(type),id(id) {}
	Symbol(TYPE type, const string description,int id=-1) :
		description(description), type(type),id(id) {}
	int id;
	TYPE type;
	string description;

	bool is_terminal()const {
		return type == TYPE::terminal || type == TYPE::null;
	}
	bool is_nonterminal()const {
		return type == TYPE::nonterminal;
	}
	bool is_null()const {
		return type == TYPE::null;
	}
	bool is_eof()const {
		return type == TYPE::eof;
	}
	bool operator!=(const Symbol& sym)const {
		return id != sym.id;
	}
	bool operator==(const Symbol& sym)const {
		return id == sym.id;
	}
	bool operator <(const Symbol& sym)const {
		return id < sym.id;
	}
};

Symbol make_dollar_symbol(int id=-1);
Symbol make_null_symbol(int id=-1);

struct symbol_ptr_less {
	bool operator()(const Symbol* lhs, const Symbol* rhs)const {
		return *lhs < *rhs;
	}
};