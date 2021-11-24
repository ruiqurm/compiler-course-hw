#include "SLR1.h"
#include<queue>
#include "Rule.h"
#include<tuple>
using std::make_tuple;
void SLR1::pre_find_first_follow(vector<vector<Symbol>>& rules){
	// 拓广文法
	rules.insert(rules.begin(),
		vector<Symbol>{
			Symbol(Symbol::TYPE::nonterminal, rules[0][0].description + "'"),
			Symbol(Symbol::TYPE::nonterminal, rules[0][0].description)
		}
	);
}
SLR1::SLR1(const initializer_list<initializer_list<Symbol>>& rules):Parser(rules) {
	vector<ItemSet> itemsets;
	std::queue<vector<ItemSet>::iterator>q;
	vector<tuple<int, Rule&>> tmp{ tuple<int, Rule&>(0, _rules[0]) };// 暂时存放新的核
	

	
}

//bool SLR1::parse(vector<Symbol>&) {
//
//}
//


ItemSet::ItemSet(int id, const vector<tuple<int, Rule&>>&now_rules, const vector<Rule&>& all_rules):
	set_id(id)
{
	std::queue<tuple<int, Rule&>>q;
	for (auto& v : now_rules) {
		q.push(v);
	}
	while (!q.empty()) {

		auto& [dot, rule] = q.front();
		//如果可以扩展
		if (rule.to().size() > dot) {
			// 如果是非终结符
			if (rule.to()[dot]->is_nonterminal()) {
				// 待约项目
				pending_items.push_back(q.front());
				// 插入所有带这个非终结符的文法
				for (auto& rule : all_rules) {
					if (rule.from() == rule.to()[dot]) q.emplace(tuple<int, Rule&>(0, rule));
				}
			}
			else {
				// 移进项目
				shift_items.push_back(q.front());
			}
		}
		// 是一个规约项目
		else {
			reduce_items.push_back(q.front());

		}
		q.pop();
	}
}
