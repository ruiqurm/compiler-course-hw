#include "Parser.h"
#include <algorithm>
#include <queue>
Parser::Parser(const initializer_list<initializer_list<Symbol>> & _rules){
	// ugly design
	_tmp_rules_pointer = new vector<vector<Symbol>> ;
	for (auto iter:_rules) {
		_tmp_rules_pointer->emplace_back(iter.begin(), iter.end());
	}
}
void Parser::build() {
	// 暂存的rules，全部转换后会释放
	auto& rules = *_tmp_rules_pointer;
	// 把Symbol转化成Symbol&，同时保存一份Symbol表
	copy_and_tag_symbol(rules);

	// 找first集
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
	// 找follow集
	find_follow();
	for (auto& f : _follow) {
		cout << f.first->description << "---";
		for (auto& item : f.second) {
			cout << item->description << " ";
		}
		cout << endl;
	}
	delete _tmp_rules_pointer;
	_tmp_rules_pointer = nullptr;
}
void Parser::find_first() {
	// 插入所有非总结符
	for (auto sym : _valid_terminal) {
		_first[sym.second].insert(sym.second);
	}
	// 插入终结符
	for (auto sym : _valid_nonterminal) {
		recursive_find_first(sym.second);
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
	// 如果是文法开始符号，那么follow集加上$
	for (auto& rule : _rules) {
		// 文法开始符号加上$
		if (rule.from() == _start_symbol) {
			_follow[rule.from()].insert(_dollar_symbol);
		}
	}
	// 插入所有非总结符
	for (auto& sym : _valid_terminal) {
		_follow[sym.second].insert(sym.second);
	}
	// 确定是否有被找过
	std::vector<bool> has_finded_follow_set;
	has_finded_follow_set.resize(_max_id + 1);
	std::fill(has_finded_follow_set.begin(), has_finded_follow_set.end(), false);
	// 非终结符
	for (auto& sym : _valid_nonterminal) {
		has_finded_follow_set[sym.second->id] = false;
	}
	for (auto sym : _valid_nonterminal) {
		if (!has_finded_follow_set[sym.second->id]) {
			recursive_find_follow(sym.second,has_finded_follow_set);
			has_finded_follow_set[sym.second->id] = true;
		}
	}
}
void Parser::recursive_find_follow(Symbol* sym, std::vector<bool>& has_finded_follow_set) {
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

void Parser::copy_and_tag_symbol(vector<vector<Symbol>>& rules) {
	map<string,Symbol*> symbol_set;
	int id = 0;
	int rule_id = 0;
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
			if (rule.begin()->is_terminal())
				_valid_terminal[name] = &all_symbols.back();
			else
				_valid_nonterminal[name] = &all_symbols.back();
		}
		auto addr = symbol_set[name];
		_rules.emplace_back(*addr);// 插入一条Rule,它的from是It->second的引用


		for (auto sym = rule.begin() + 1; sym != rule.end();sym++) {
			const string& name = sym->description;
			auto it = symbol_set.find(name);
			if (it == symbol_set.end()) {
				// 如果之前没有加过这个符号
				all_symbols.push_back(*sym);
				all_symbols.back().id = id++;
				symbol_set.insert(std::pair<string, Symbol*>(name, &all_symbols.back()));
				if (sym->is_terminal())
					_valid_terminal[name] = &all_symbols.back();
				else
					_valid_nonterminal[name] = &all_symbols.back();
			}
			auto addr = symbol_set[name];

			_rules.back().to().push_back(addr);
		}
		_rules.back().set_id(rule_id++);
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
	_max_id = id;

	find_conduce_to_null();

}
void Parser::find_conduce_to_null() {
	std::queue<int> q;
	_conduce_to_null.resize(_max_id+1);
	std::fill(_conduce_to_null.begin(), _conduce_to_null.end(), false);
	for (auto& rule : _rules) {
		if (rule.to()[0]->is_null()) {
			_conduce_to_null[rule.from()->id] = true;
			q.push(rule.from()->id);
		}
	}
	while(!q.empty()){
		int id = q.front();
		for (auto& rule : _rules) {
			if (rule.to()[0]->id ==id) {
				_conduce_to_null[rule.from()->id] = true;
				q.push(rule.from()->id);
			}
		}
		q.pop();
	}
}
bool Parser::parse(vector<Symbol>&v) {
	for (auto& sym : v) {
		if (sym.is_terminal()) {
			sym.id = _valid_terminal[sym.description]->id;
		}else {
			sym.id = _valid_nonterminal[sym.description]->id;
		}
	}
	return _parse(v);
}
