#pragma once
#include"Rule.h"
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<iostream>
#include<list>
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;
using std::list;
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
	Symbol* _start_symbol{nullptr}; // 文法起始符号
	Symbol* _null_symbol{ nullptr }; // null符号
	Symbol* _dollar_symbol{ nullptr }; // $符号

	vector<vector<Symbol>>* _tmp_rules_pointer{nullptr}; // 一个暂时存数据的地方
	vector<Rule> _rules;// 文法
	symbol_lookup_set_t _first; // first集
	symbol_lookup_set_t _follow;// follow集
	symbol_set_t _valid_terminal;// 有效终结符
	symbol_set_t _valid_nonterminal;// 有效非终结符
	list<Symbol> all_symbols;// 全体符号，保存唯一拷贝
	vector<bool> _conduce_to_null;// 致空集
	int _max_id{ 0 };// 最大符号id
	bool _is_build{ false };// 是否构建完成
	
	virtual void debug_parser_table() {
		if (!_is_build) {
			build();
		}
	}
	virtual bool _parse(vector<Symbol>&) { return false; }

private:
	
	void copy_and_tag_symbol(vector<vector<Symbol>>& rules); 
	void find_first(); // 找first集
	void recursive_find_first(Symbol*);// 递归找first集
	void recursive_find_follow(Symbol*, std::vector<bool>&);// 递归找follow集
	void find_follow();// 找follow集
	void find_conduce_to_null();//找到致空集
};


