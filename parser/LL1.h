#pragma once
#include "Parser.h"
#include "Rule.h"
#include<memory>
#include<iostream>
class LL1: 
	public Parser
{
public:
	LL1(const initializer_list<initializer_list<Symbol>>& rules);
	friend std::ostream& operator<<(std::ostream& os, LL1& r);
	void build() override;
private:
	bool _parse(vector<Symbol>&)override;
	// table前面保存的是非终结符，后面的维度保存的是终结符
	map<Symbol*, map<Symbol*,Rule&,symbol_ptr_less>,symbol_ptr_less>_table;
};

