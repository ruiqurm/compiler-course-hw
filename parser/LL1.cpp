#include "LL1.h"
#include<stack>
using std::stack;
//void LL1::pre_find_first_follow(vector<vector<Symbol>> &rules) {
	//// 消左递归
	//bool check = false;
	//do {
	//	Symbol* s = nullptr;
	//	for (auto& rule : rules) {
	//		if (rule[0] == rule[1]) {
	//			s = &rule[0];
	//			check = true;
	//			break;
	//		}
	//	}
	//	if (!check)break;
	//	// 找到所有相关的项目
	//	// 形如A->Aa | b
	//	vector<int> a_poses;
	//	vector<int> b_poses;
	//	for (auto i = 0; i < rules.size();i++) {
	//		if (rules[i][0] == *s) {
	//			if (rules[i][1] == *s) {
	//				a_poses.push_back(i);
	//			}
	//			else {
	//				b_poses.push_back(i);
	//			}
	//		}
	//	}
	//	Symbol new_sym(Symbol::TYPE::nonterminal, s->description + '\'');
	//	for (auto& x : b_poses) {
	//		vector<Symbol>v;
	//		v.assign(rules[x].begin(), rules[x].end()); // A->bA'
	//		v.push_back(new_sym);
	//	}

	//} while (check);
//}
LL1::LL1(const initializer_list<initializer_list<Symbol>>& rules) :Parser(rules) {

}
void LL1::build(){
	Parser::build();
	for (auto& rule : _rules) {
		auto alpha = rule.to()[0];
		auto from = rule.from();
		auto has_null = false;
		for (auto& sym_advanced_alpha : _first[alpha]) {
			if (sym_advanced_alpha->is_null()) {
				has_null = true;
				continue;
			}
			if (sym_advanced_alpha->is_terminal()) {
				_table[from].insert(std::pair<Symbol*, Rule&>(sym_advanced_alpha, rule));
			}
		}
		if (has_null) {
			for (auto sym_followed_from : _follow[from]) {
				_table[from].insert(std::pair<Symbol*, Rule&>(sym_followed_from, rule));
			}
		}
	}
}
bool LL1::_parse(vector<Symbol>& input){
	stack<Symbol*> s;
	s.push(_start_symbol);
	auto ip = input.begin();
	do {
		auto top = s.top();
		if (top->is_eof() || top->is_terminal()) {
			if (*top == *ip) {
				s.pop();
				ip++;
			}
			else {
				cout << "runtime error";
				return false;
			}
		}
		else{
			auto r1 = _table.find(s.top());
			if (r1 != _table.end()) {
				auto r2 = r1->second.find(&(*ip));
				if (r2 != r1->second.end()) {
					s.pop();
					auto r = r2->second.to();
					for (auto it = r.rbegin(); it != r.rend(); ++it) {
						s.push(*it);
					}
				}
			}

		}
	} while (s.top()->is_eof());
	return true;
}

