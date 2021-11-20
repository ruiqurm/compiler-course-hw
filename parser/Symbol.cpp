#include "Symbol.h"
#include<memory>
Symbol make_dollar_symbol(int id) {
	return Symbol(Symbol::TYPE::eof, "$",id);
}
Symbol make_null_symbol(int id) {
	return Symbol(Symbol::TYPE::null, "ε",id);
}