#include "Parser.h"
#include <algorithm>

Parser::Parser(const initializer_list<initializer_list<Symbol>> & rules){
	pre_find_first_follow();
	// 把Symbol转化成Symbol&，同时保存一份Symbol表
	copy_and_tag_symbol(rules);
	
	find_first();
	cout << "first set:" << endl;
	for (auto& f : _first) {
		cout << f.first->description << "---";
		for (auto& item : f.second) {
			cout << item->description << ",";
		}
		cout << endl;
	}
	cout << "follow set:" << endl;
	find_follow();
	for (auto& f : _follow) {
		cout << f.first->description << "---";
		for (auto& item : f.second) {
			cout << item->description << " ";
		}
		cout << endl;
	}
}
void Parser::find_first() {
	

	for (auto sym : _valid_terminal) {
		_first[sym].insert(sym);
	}
	for (auto sym : _valid_nonterminal) {
		recursive_find_first(sym);
	}

}
void Parser::recursive_find_first(Symbol *sym) {
	// 如果找到了first集，那么跳过
	if (_first.find(sym) != _first.end())return;
	//auto _ = _first.emplace(sym, set<Symbol*>{});
	auto &sym_set = _first[sym];
	for (auto rule : _rules) {
		// 如果是无关项，跳过
		auto& first_sym = rule.to()[0];
		if (rule.from() != sym || first_sym == sym)continue;
		if (first_sym->is_terminal()) {
			// 如果第一个是终结符或者null
			_first[sym].insert(first_sym);
		}else{
			// 如果第一个是非终结符
			// 找第一个非终结符的first集
			recursive_find_first(first_sym);
			// 合并集合
			sym_set.insert(_first[first_sym].begin(), _first[first_sym].end());
			
			if (_conduce_to_null[first_sym->id]) {
				// 如果非终结符致空
				// 找到第一个非致空终结符
				bool find_non_null_noterminal = false;
				auto iter = rule.to().begin()+1;
				while (iter != rule.to().end()) {
					if((*iter)->is_nonterminal()) {
						find_non_null_noterminal = true;
						break;
					}
					iter++;
				}
				// 如果找到了，合并集合
				if (find_non_null_noterminal) {
					recursive_find_first(first_sym);
					sym_set.insert(_first[first_sym].begin(), _first[first_sym].end());
				}
			}
		}
	}
}
void Parser::find_follow() {
	for (auto& rule : _rules) {
		// 文法开始符号加上$
		if (rule.from() == _start_symbol) {
			_follow[rule.from()].insert(_dollar_symbol);
		}
	}
	map<int, bool> has_finded_follow_set;
	for (auto& sym : _valid_terminal) {
		_follow[sym].insert(sym);
	}
	for (auto& sym : _valid_nonterminal) {
		has_finded_follow_set[sym->id] = false;
	}
	for (auto sym : _valid_nonterminal) {
		if (!has_finded_follow_set[sym->id]) {
			recursive_find_follow(sym,has_finded_follow_set);
			has_finded_follow_set[sym->id] = true;
		}
	}
}
void Parser::recursive_find_follow(Symbol* sym,map<int, bool>& has_finded_follow_set) {
	// 如果找到了first集，那么跳过
	auto& sym_set = _follow[sym];
	for (auto rule : _rules) {
		std::vector<Symbol*>::iterator pfind=rule.to().begin();
		bool next_conduce_to_null = false;
		bool first_scan = false;
		while ( (pfind = std::find(pfind, rule.to().end(), sym))!= rule.to().end()) {
			if (!first_scan) {
				// 检查一下后面是不是全部致空
				first_scan = true;
				auto next = pfind + 1;
				while (next != rule.to().end() && _conduce_to_null[(* next)->id]) {
					next++;
				}
				if (next == rule.to().end())next_conduce_to_null = true;
			}
			if (next_conduce_to_null && rule.from() != sym) {
				if (!has_finded_follow_set[rule.from()->id]) {
					// 如果没插入过
					recursive_find_follow(rule.from(),has_finded_follow_set);
				}
				auto& s = _follow[rule.from()];
				sym_set.insert(s.begin(),s.end());
			}
			if (pfind + 1 != rule.to().end()) {
				auto& next_symbol = *(pfind+1);
				for (auto& symbol_in_first_set : _first[next_symbol]) {
					if (!symbol_in_first_set->is_null()) {
						sym_set.insert(symbol_in_first_set);
					}
				}
			}
			pfind++;
		}
	}
}

void Parser::copy_and_tag_symbol(const initializer_list<initializer_list<Symbol>>& rules) {
	map<string,Symbol*> symbol_set;
	int id = 0;
	set<string>check;
	for (auto a : rules) {
		for (auto b : a) {
			check.insert(b.description);
		}
	}
	all_symbols.reserve(check.size() + 10);
	for (auto rule : rules) {

		// 插入表达式左边的值
		const string& name = rule.begin()->description;
		auto it = symbol_set.find(name);
		if (it == symbol_set.end()) {
			// 如果之前没有加过这个符号
			all_symbols.push_back(*rule.begin());
			all_symbols.back().id = id++;
			symbol_set.insert(std::pair<string, Symbol*>(name,&all_symbols.back()));
		}
		auto addr = symbol_set[name];
		if (rule.begin()->is_terminal())
			_valid_terminal.insert(addr);
		else
			_valid_nonterminal.insert(addr);
		_rules.emplace_back(*addr);// 插入一条Rule,它的from是It->second的引用


		for (auto sym = rule.begin() + 1; sym != rule.end();sym++) {
			const string& name = sym->description;
			auto it = symbol_set.find(name);
			if (it == symbol_set.end()) {
				// 如果之前没有加过这个符号
				all_symbols.push_back(*sym);
				all_symbols.back().id = id++;
				symbol_set.insert(std::pair<string, Symbol*>(name, &all_symbols.back()));
			}
			auto addr = symbol_set[name];
			if (sym->is_terminal())
				_valid_terminal.insert(addr);
			else
				_valid_nonterminal.insert(addr);
			_rules.back().to().push_back(addr);
		}
		cout << _rules.back().from()->description << "-->";
		for (auto& x : _rules.back().to()) {
			cout << x->description;
		}
		cout << endl;
	}
	// 插入dollar符号
	all_symbols.push_back(make_dollar_symbol(id++));
	_dollar_symbol = &all_symbols.back();

	_start_symbol = &all_symbols[0];
	for (auto& rule : _rules) {
		if (rule.to()[0]->is_null())
			_conduce_to_null[rule.from()->id] = true;
		else
			_conduce_to_null[rule.from()->id] = false;
	}
}