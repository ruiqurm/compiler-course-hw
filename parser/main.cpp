#include "Rule.h"
#include "Parser.h"
#include <vector>
using namespace std;
using TYPE = Symbol::TYPE;

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
	Parser p({
		{
			Symbol(TYPE::nonterminal, "E"),
			Symbol(TYPE::nonterminal, "A"),
		},
		{
			Symbol(TYPE::nonterminal, "E"),
			Symbol(TYPE::nonterminal, "B"),
		},
		{
			Symbol(TYPE::nonterminal, "A"),
			Symbol(TYPE::terminal, "num"),
		},
		{
			Symbol(TYPE::nonterminal, "A"),
			Symbol(TYPE::terminal, "id"),
		},
		{
			Symbol(TYPE::nonterminal, "B"),
			Symbol(TYPE::terminal, "("),
			Symbol(TYPE::nonterminal, "L"),
			Symbol(TYPE::terminal, ")"),
		},
		{
			Symbol(TYPE::nonterminal, "L"),
			Symbol(TYPE::nonterminal, "E"),
			Symbol(TYPE::nonterminal, "L'"),
		},
		{
			Symbol(TYPE::nonterminal, "L'"),
			Symbol(TYPE::nonterminal, "E"),
			Symbol(TYPE::nonterminal, "L'"),
		},
		{
			Symbol(TYPE::nonterminal, "L'"),
			Symbol(TYPE::null, "null"),
		},
		});
}