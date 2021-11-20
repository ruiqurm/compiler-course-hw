#pragma once
#include"Rule.h"
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include<memory>
#include<iostream>
using std::unordered_map;
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;
using std::reference_wrapper;
class Parser
{
public:
	using symbol_set_t = set<Symbol*, symbol_ptr_less>;
	using symbol_lookup_set_t = map<Symbol*, symbol_set_t, symbol_ptr_less>;
	Parser(const initializer_list<initializer_list<Symbol>>& rules);
	symbol_lookup_set_t& first() { return _first; }
	symbol_lookup_set_t& follow() { return _follow; }
protected:
	Symbol* _start_symbol{nullptr};
	Symbol* _null_symbol{ nullptr };
	Symbol* _dollar_symbol{ nullptr };

	vector<Rule> _rules;
	symbol_lookup_set_t _first;
	symbol_lookup_set_t _follow;
	symbol_set_t _valid_terminal;
	symbol_set_t _valid_nonterminal;
	vector<Symbol> all_symbols;
	map<int, bool> _conduce_to_null;

	virtual void pre_find_first_follow() {}
	virtual void debug_parser_table() {}
	virtual bool parse(vector<Symbol>&) { return false; }
private:
	
	void copy_and_tag_symbol(const initializer_list<initializer_list<Symbol>>& rules);
	void find_first();
	void recursive_find_first(Symbol*);
	void recursive_find_follow(Symbol*, map<int, bool>&);
	void find_follow();

};


