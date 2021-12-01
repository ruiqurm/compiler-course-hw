#include "LL1.h"
#include"Symbol.h"
#include<stack>
#include<algorithm>
#include<set>
using std::set;
using std::erase_if;
using std::stack;

LL1::LL1(const initializer_list<initializer_list<Symbol>>& rules) :Parser(rules) {
	// 消左递归,没多少时间了,写得比较丑陋.
	bool should_check = false;
	auto& real_rules = *_tmp_rules_pointer;// 实际的文法
	do {
		should_check = false;
		Symbol s;
		for (auto& rule : real_rules) {
			if (rule.begin()->description == (rule.begin()+1)->description) {
				s = rule[0];
				should_check = true;
				break;
			}
		}
		if (!should_check)break;
		// 找到所有相关的项目
		// 形如A->Aa | b
		vector<std::pair<vector<Symbol>::iterator, vector<Symbol>::iterator>> a_poses;
		vector<std::pair<vector<Symbol>::iterator, vector<Symbol>::iterator>> b_poses;
		int cnt = 0;
		for (auto i = real_rules.begin(); i != real_rules.end(); i++) {
			if ( i->begin()->description == s.description) {
				// A->Aa
				if ((i->begin()+1)->description == s.description) {
					a_poses.emplace_back(i->begin() + 2,i->end());
				}
				else {
				// A->b
					b_poses.emplace_back(i->begin() + 1, i->end());
				}
			}
			else {
				cnt++;
			}
		}
		if (a_poses.size() == 0) {
			throw "can't resolve left recurrence";
		}
		Symbol new_sym(Symbol::TYPE::nonterminal, s.description + '\'');
		vector<vector<Symbol>>tmp;
		if (b_poses.empty()) {
			// 插入A->epsilon
			vector<Symbol> v{ s, make_null_symbol() };
			tmp.push_back(v);
		}
		else {
			for (auto x : b_poses) {
				vector<Symbol> v{ s};
				while (x.first != x.second) {
					v.push_back(*x.first);
					x.first++;
				}
				v.push_back(new_sym);
				tmp.push_back(v);
			}
		}

		for (auto& x : a_poses) {
			vector<Symbol> v{ new_sym};
			while (x.first != x.second) {
				v.push_back(*x.first);
				x.first++;
			}
			v.push_back(new_sym);
			tmp.emplace_back(v);
		}
		vector<Symbol> v{ new_sym,make_null_symbol()};
		tmp.emplace_back(v);
		erase_if(real_rules, [=](const vector<Symbol>& v) {return v[0].description == s.description; });
		if (s.description == rules.begin()->begin()->description) {
			real_rules.insert(real_rules.begin(), tmp.begin(), tmp.end());
		}
		else {
			real_rules.insert(real_rules.end(), tmp.begin(), tmp.end());
		}
	} while (should_check);

}
void LL1::build(){
	Parser::build();
	for (auto& rule : _rules) {
		// 对于文法A->α
		auto alpha = rule.to()[0];
		auto from = rule.from();
		auto has_null = false;
		// 遍历α的first集
		for (auto& sym_advanced_alpha : _first[alpha]) {
			if (sym_advanced_alpha->is_null()) {
				has_null = true;
				continue;
			}
			// 在分析表M[A,alpha]插入
			if (sym_advanced_alpha->is_terminal()) {
				_table[from].insert(std::pair<Symbol*, Rule&>(sym_advanced_alpha, rule));
			}
		}
		// 如果first集中符号致空
		if (has_null) {
			// 遍历A的follow集，在分析表M[A,a_follow]中插入
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
		// 如果栈顶是非终结符或者结束符
		if (top->is_eof() || top->is_terminal()) {
			// 如果输入符号和栈顶符号相同
			if (*top == *ip) {
				// 出栈，输入指针前移
				cout << "match " << ip->description << endl;
				s.pop();
				ip++;

			}
			else {
				cout << "runtime error";
				return false;
			}
		}
		else{
			// 找到分析表中此时对应的文法r2
			auto r1 = _table.find(s.top());
			if (r1 != _table.end()) {
				auto r2 = (ip != input.end())?r1->second.find(&(*ip)): r1->second.find(_dollar_symbol);
				if (r2 != r1->second.end()) {
					// 出栈，将栈顶符号替换为文法对应的符号。注意此时逆序插入
					cout << "pop " << top->description << ",insert";
					cout << r2->second.from()->description << " --> ";
					for (auto& sym : r2->second.to()) {
						cout << sym->description;
					}
					cout << endl;
					s.pop();
					auto r = r2->second.to();
					for (auto it = r.rbegin(); it != r.rend(); ++it) {
						if( !(*it)->is_null())
							s.push(*it);
					}
				}
			}

		}
	} while (!s.empty());
	return true;
}


void LL1::debug_parser_table() {
	Parser::debug_parser_table();
	cout << "LL1:\t";
	for (auto& ter_sym : _valid_terminal) {
		if (ter_sym.second->is_null())continue;
		cout << ter_sym.second->description << "\t";
	}
	cout << endl;
	for (auto& noter_sym : _valid_nonterminal) {
		cout << noter_sym.second->description << "\t";
		auto outer = _table.find(noter_sym.second);
		if (outer == _table.end()) {
			cout << endl;
			continue;
		}
		for (auto& ter_sym : _valid_terminal) {
			if (ter_sym.second->is_null())continue;
			if (auto rule = outer->second.find(ter_sym.second); rule != outer->second.end()) {
				auto& r = rule->second;
				cout << r.from()->description << "->";
				for (auto& x : r.to()) {
					cout << x->description;
				}
				cout << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}