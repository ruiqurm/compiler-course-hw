#include "Parser.h"
#include <algorithm>
void Parser::find_first() {
	map<Symbol, bool> conduce_to_null;
	for (auto& rule : _rules) {
		if(rule.to()[0].is_null())
			conduce_to_null[rule.from()] = true;
	}
	for (auto sym : _valid_terminal) {
		_first[sym].insert(sym);
	}
	for (auto sym : _valid_nonterminal) {
		recursive_find_first(sym,conduce_to_null);
	}

}
void Parser::recursive_find_first(Symbol &sym, map<Symbol, bool>& conduce_to_null) {
	// 如果找到了first集，那么跳过
	if (_first.find(sym) != _first.end())return;
	auto _ = _first.emplace(sym, set<Symbol>{});
	auto &sym_set = _first[sym];
	for (auto rule : _rules) {
		// 如果是无关项，跳过
		auto& first_sym = rule.to()[0];
		if (rule.from() != sym || first_sym == sym)continue;
		if (first_sym.is_terminal()) {
			// 如果第一个是终结符或者null
			_first[sym].insert(first_sym);
		}else{
			// 如果第一个是非终结符
			// 找第一个非终结符的first集
			recursive_find_first(first_sym,conduce_to_null);
			// 合并集合
			sym_set.insert(_first[first_sym].begin(), _first[first_sym].end());
			
			if (conduce_to_null[first_sym]){
				// 如果非终结符致空
				// 找到第一个非致空终结符
				bool find_non_null_noterminal = false;
				auto iter = rule.to().begin()+1;
				while (iter != rule.to().end()) {
					if(iter->is_nonterminal()){
						find_non_null_noterminal = true;
						break;
					}
					iter++;
				}
				// 如果找到了，合并集合
				if (find_non_null_noterminal) {
					recursive_find_first(first_sym, conduce_to_null);
					sym_set.insert(_first[first_sym].begin(), _first[first_sym].end());
				}
			}
		}
	}
}
void Parser::find_follow() {
	//for (auto& rule : _rules) {
	//	// 文法开始符号加上$
	//	if (rule.from().is_start()) {
	//		_follow[rule.from()].insert()
	//	}
	//}
	map<Symbol, bool> conduce_to_null;
	for (auto& rule : _rules) {
		if (rule.to()[0].is_null())
			conduce_to_null[rule.from()] = true;
	}
	for (auto sym : _valid_terminal) {
		_follow[sym].insert(sym);
	}
	for (auto sym : _valid_nonterminal) {
		recursive_find_follow(sym, conduce_to_null);
	}
}
void Parser::recursive_find_follow(Symbol& sym, map<Symbol, bool>& conduce_to_null) {
	// 如果找到了first集，那么跳过
	if (_follow.find(sym) != _follow.end())return;
	auto _ = _follow.emplace(sym, set<Symbol>{});
	auto& sym_set = _follow[sym];
	for (auto rule : _rules) {
		std::vector<Symbol>::iterator pfind=rule.to().begin();
		bool next_conduce_to_null = false;
		bool first_scan = false;
		while ( (pfind = std::find(pfind, rule.to().end(), sym))!= rule.to().end()) {
			if (!first_scan) {
				// 检查一下后面是不是全部致空
				first_scan = true;
				auto next = pfind + 1;
				while (next != rule.to().end() && conduce_to_null[*next]) {
					next++;
				}
				if (next == rule.to().end())next_conduce_to_null = true;
			}
			if (next_conduce_to_null) {
				// 判断条件要改一下
				auto p_from_follow_set = _follow.find(rule.from());
				if (p_from_follow_set == _follow.end()) {
					recursive_find_follow(rule.from(), conduce_to_null);
					p_from_follow_set = _follow.find(rule.from());
					sym_set.insert(p_from_follow_set->second.begin(), p_from_follow_set->second.end());
				}
				else {
					sym_set.insert(p_from_follow_set->second.begin(), p_from_follow_set->second.end());
				}
			}
			else {
				auto& next_symbol = *(pfind + 1);
				for (auto& symbol_in_first_set : _first[next_symbol]) {
					if (!symbol_in_first_set.is_null()) {
						sym_set.insert(symbol_in_first_set);
					}
				}
			}
			pfind++;
		}
	}
}