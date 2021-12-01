#include "Rule.h"
#include "Parser.h"
#include "LL1.h"
#include <vector>
#include "SLR1.h"
#include "LR1.h"

using namespace std;
using TYPE = Symbol::TYPE;
ostream& operator<<(ostream& os, Rule& r) {

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
	//		{
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "E'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "E'"),
	//			Symbol(TYPE::terminal, "+"),
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "E'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "E'"),
	//			Symbol(TYPE::terminal, "-"),
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "E'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "E'"),
	//			Symbol(TYPE::null, "null"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::nonterminal, "T'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T'"),
	//			Symbol(TYPE::terminal, "*"),
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::nonterminal, "T'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T'"),
	//			Symbol(TYPE::terminal, "/"),
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::nonterminal, "T'"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T'"),
	//			Symbol(TYPE::null, "null"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::terminal, "("),
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::terminal, ")"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::terminal, "num"),
	//		},
	//	});
	//LL1 ll1{
	//		{
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::terminal, "+"),
	//			Symbol(TYPE::nonterminal, "T"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::terminal, "-"),
	//			Symbol(TYPE::nonterminal, "T"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::nonterminal, "T"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::terminal, "*"),
	//			Symbol(TYPE::nonterminal, "F"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::terminal, "/"),
	//			Symbol(TYPE::nonterminal, "F"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "T"),
	//			Symbol(TYPE::nonterminal, "F"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::terminal, "("),
	//			Symbol(TYPE::nonterminal, "E"),
	//			Symbol(TYPE::terminal, ")"),
	//		},
	//		{
	//			Symbol(TYPE::nonterminal, "F"),
	//			Symbol(TYPE::terminal, "num"),
	//		},
	//};
	//ll1.debug_parser_table();
	//auto s = string2symbol("1+(2+3)*4+5/6");
	//for (auto &i : s) {
	//	if (i.description[0] >= '0' && i.description[0] <= '9') {
	//		i.description = "num";
	//	}
	//}
	//ll1.parse(s);

	LR1 slr1({
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
	slr1.debug_parser_table();
	auto s = string2symbol("1+(2+3)*4+5/6");
	for (auto &i : s) {
		if (i.description[0] >= '0' && i.description[0] <= '9') {
			i.description = "num";
		}
	}
	slr1.parse(s);
}