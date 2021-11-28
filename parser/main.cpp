#include "Rule.h"
#include "Parser.h"
#include "LL1.h"
#include <vector>
#include "SLR1.h"
using namespace std;
using TYPE = Symbol::TYPE;
ostream& operator<<(ostream& os, Rule& r) {
	os << r.from()->description << "->";
	for (auto& x : r.to()) {
		os << x->description;
	}
	return os;
}
ostream& operator<<(ostream& os, LL1& l) {
	os << "表格\t";
	for (auto& ter_sym : l._valid_terminal) {
		os << ter_sym.second->description << "\t";
	}
	os << endl;
	for (auto& noter_sym : l._valid_nonterminal) {
		os << noter_sym.second->description << "\t";
		auto outer = l._table.find(noter_sym.second);
		if ( outer == l._table.end()) {
			os << endl;
			continue;
		}
		for (auto& ter_sym : l._valid_terminal) {
			if (auto rule = outer->second.find(ter_sym.second);rule != outer->second.end()) {
				os << rule->second << "\t";
			}
			else {
				os << "\t";
			}
		}
		os << endl;
	}
	return os;
}

int main() {
	//vector<Rule> rules{
	//	{
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::terminal, "+"),
	//		Symbol(TYPE::nonterminal, "T"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::terminal, "-"),
	//		Symbol(TYPE::nonterminal, "T"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "T"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "T"),
	//		Symbol(TYPE::nonterminal, "T"),
	//		Symbol(TYPE::terminal, "*"),
	//		Symbol(TYPE::nonterminal, "F"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "T"),
	//		Symbol(TYPE::nonterminal, "T"),
	//		Symbol(TYPE::terminal, "/"),
	//		Symbol(TYPE::nonterminal, "F"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "T"),
	//		Symbol(TYPE::nonterminal, "F"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "F"),
	//		Symbol(TYPE::terminal, "("),
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::terminal, ")"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "F"),
	//		Symbol(TYPE::terminal, "num"),
	//	},
	//};

	//LL1 ll({
	//	{
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "A"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "B"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "A"),
	//		Symbol(TYPE::terminal, "num"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "A"),
	//		Symbol(TYPE::terminal, "id"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "B"),
	//		Symbol(TYPE::terminal, "("),
	//		Symbol(TYPE::nonterminal, "L"),
	//		Symbol(TYPE::terminal, ")"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "L"),
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "L'"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "L'"),
	//		Symbol(TYPE::nonterminal, "E"),
	//		Symbol(TYPE::nonterminal, "L'"),
	//	},
	//	{
	//		Symbol(TYPE::nonterminal, "L'"),
	//		Symbol(TYPE::null, "null"),
	//	},
	//	});
	//auto s = string2symbol("(a(b(2))(c))");
	//s[1].description = "id";
	//s[3].description = "id";
	//s[5].description = "num";
	//s[9].description = "id";
	//ll.parse(s);

	SLR1 slr1({
			{
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::terminal, "+"),
				Symbol(TYPE::nonterminal, "T"),
			},
			{
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::terminal, "-"),
				Symbol(TYPE::nonterminal, "T"),
			},
			{
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::nonterminal, "T"),
			},
			{
				Symbol(TYPE::nonterminal, "T"),
				Symbol(TYPE::nonterminal, "T"),
				Symbol(TYPE::terminal, "*"),
				Symbol(TYPE::nonterminal, "F"),
			},
			{
				Symbol(TYPE::nonterminal, "T"),
				Symbol(TYPE::nonterminal, "T"),
				Symbol(TYPE::terminal, "/"),
				Symbol(TYPE::nonterminal, "F"),
			},
			{
				Symbol(TYPE::nonterminal, "T"),
				Symbol(TYPE::nonterminal, "F"),
			},
			{
				Symbol(TYPE::nonterminal, "F"),
				Symbol(TYPE::terminal, "("),
				Symbol(TYPE::nonterminal, "E"),
				Symbol(TYPE::terminal, ")"),
			},
			{
				Symbol(TYPE::nonterminal, "F"),
				Symbol(TYPE::terminal, "num"),
			},
		});
	//lr1.build();
	slr1.debug_parser_table();
	//auto s = string2symbol("((a)a(aa))");
	//slr1.parse(s);
}