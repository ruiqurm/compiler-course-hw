#pragma once
#include"Rule.h"
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<iostream>
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;
class Parser
{
public:
	Parser(vector<Rule>&& rules):_rules(std::move(rules)) {
		for (auto rule : _rules) {
			if(rule.from().is_terminal())
				_valid_terminal.insert(rule.from());
			else
				_valid_nonterminal.insert(rule.from());
			for (auto sym : rule.to()) {
				if (sym.is_terminal())
					_valid_terminal.insert(sym);
				else
					_valid_nonterminal.insert(sym);
			}
		}
		find_first();
		cout << "first set:" << endl;
		for (auto& f : _first) {
			cout << f.first.description << "---";
			for(auto &item:f.second){
				cout << item.description << ",";
			}
			cout << endl;
		}
		cout << "follow set:" << endl;
		find_follow();
		for (auto& f : _follow) {
			cout << f.first.description << "---";
			for (auto& item : f.second) {
				cout << item.description << " ";
			}
			cout << endl;
		}
	}
	map<Symbol, set<Symbol>>& first() { return _first; }
	map<Symbol, set<Symbol>>& follow() { return _follow; }
private:

	vector<Rule> _rules;
	map<Symbol, set<Symbol>> _first;
	map<Symbol, set<Symbol>> _follow;
	set<Symbol> _valid_terminal;
	set<Symbol> _valid_nonterminal;
	void find_first();
	void recursive_find_first(Symbol&, map<Symbol, bool>&);
	void recursive_find_follow(Symbol&, map<Symbol, bool>&);
	void find_follow();

};


