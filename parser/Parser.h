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
	using symbol_set_t = map<string,Symbol*>;
	using symbol_lookup_set_t = map<Symbol*, set<Symbol*,symbol_ptr_less>, symbol_ptr_less>;
	Parser(const initializer_list<initializer_list<Symbol>>& rules);
	symbol_lookup_set_t& first() { return _first; }
	symbol_lookup_set_t& follow() { return _follow; }
	virtual bool parse(vector<Symbol>&);
	virtual void build();

protected:
	Symbol* _start_symbol{nullptr};
	Symbol* _null_symbol{ nullptr };
	Symbol* _dollar_symbol{ nullptr };

	vector<vector<Symbol>>* _tmp_rules_pointer{nullptr};
	vector<Rule> _rules;
	symbol_lookup_set_t _first;
	symbol_lookup_set_t _follow;
	symbol_set_t _valid_terminal;
	symbol_set_t _valid_nonterminal;
	vector<Symbol> all_symbols;
	vector<bool> _conduce_to_null;
	int _max_id{ 0 };
	bool _is_build{ false };
	
	virtual void debug_parser_table() {
		if (!_is_build) {
			build();
		}
	}
	virtual bool _parse(vector<Symbol>&) { return false; }

private:
	
	void copy_and_tag_symbol(vector<vector<Symbol>>& rules);
	void find_first();
	void recursive_find_first(Symbol*);
	void recursive_find_follow(Symbol*, std::vector<bool>&);
	void find_follow();
	void find_conduce_to_null();//找到致空集
};


