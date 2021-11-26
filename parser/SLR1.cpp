#include "SLR1.h"
#include<queue>
#include "Rule.h"
#include<tuple>
#include<list>
#include<deque>
using std::deque;
using std::list;
using std::make_tuple;

SLR1::SLR1(const initializer_list<initializer_list<Symbol>>& rrr) :Parser(rrr) {
	auto& rules = *_tmp_rules_pointer;
	// 拓广文法
	rules.insert(rules.begin(),
		vector<Symbol>{
		Symbol(Symbol::TYPE::nonterminal, rules[0][0].description + "'"),
			Symbol(Symbol::TYPE::nonterminal, rules[0][0].description)
	}
	);
}


void SLR1::build() {
	Parser::build();
	using itemset_pointer = ItemSet*;
	using tmp_vec_pointer = vector<Item>::iterator;
	
	auto itemsets_id = 0;
	deque<ItemSet> itemsets; // 保存实体项目集
	std::queue<itemset_pointer>q; // BFS遍历所有可能的项目集
	vector<Item> tmp{ Item(0, &_rules[0]) };// 暂时存放新的核
	std::map<vector<Item>, itemset_pointer> kernel;// 所有核的集合。防止重复加入

	// 初始核
	kernel.emplace(tmp, nullptr);
	auto _ = itemsets.emplace_back(itemsets_id++, tmp, _rules);
	kernel[tmp] = &_;
	q.push(&_);

	while (!q.empty()) {
		auto iter = q.front();
		map<Symbol*,vector<tmp_vec_pointer>,symbol_ptr_less> symbol_to_item;// 每一个symbol对项目的字典，用于拓展新的项目集

		// 统计shift表的信息，放入到字典中。
		for (auto vec_pointer = iter->shift_items.begin(); vec_pointer != iter->shift_items.end();vec_pointer++) {
			auto pos = get<0>(*vec_pointer);
			auto &rule = get<1>(*vec_pointer);
			if (auto find_p = symbol_to_item.find(rule->to()[pos]); find_p != symbol_to_item.end()) {
				find_p->second.push_back(vec_pointer);
			}
			else {
				symbol_to_item[rule->to()[pos]] = { vec_pointer };
			}
		}
		q.pop();

		// 拓展新的节点
		for (const auto& [key,value] : symbol_to_item) {
			tmp.clear();
			for (auto v : value) {
				tmp.push_back(Item(get<0>(*v) + 1, get<1>(*v)));
			}
			if (auto find_p = kernel.find(tmp); find_p != kernel.end()) {
				// 已经有了，不需要拓展
				//find_p->second->
			}
			else {
				// 暂时创建一个新节点
				auto &ret = itemsets.emplace_back(itemsets_id++,tmp, _rules);
				kernel[tmp] = &ret;
				if (itemsets.back().shift_items.size() > 0) {
					q.push(&ret);
				}
			}
		}
		
	}
	cout << "end;";

}

ItemSet::ItemSet(int id, const vector<tuple<int, Rule*>>&now_rules,  vector<Rule>& all_rules):
	set_id(id)
{
	std::queue<Item>q;// BFS 队列
	set <Item,Item_less> all_items;
	for (auto& v : now_rules) { 
		q.push(v);
		all_items.insert(v);
	}
	while (!q.empty()) {
		item_count++;
		auto [dot, rule] = q.front();
		//如果可以扩展
		if (rule->to().size() > dot) {
			// 移进项目
			shift_items.push_back(q.front());

			// 如果是非终结符
			auto& this_sym = *rule->to()[dot];
			if (rule->to()[dot]->is_nonterminal()) {
				// 待约项目
				// 插入所有带这个非终结符的文法
				for (auto & rule : all_rules) {
					if (*rule.from() == this_sym) {
						// 如果出现重复，不插入
						auto old_size = all_items.size();
						auto new_tuple = Item(0, &rule);
						all_items.insert(new_tuple);
						if (old_size != all_items.size()) {
							q.emplace(new_tuple);
						}
					}
				}
			}
		}
		// 是一个规约项目
		else {
			reduce_items.push_back(q.front());

		}
		q.pop();
	}
	
	// 清空goto表，让别的函数填goto表
	fun_goto.resize(item_count);
	std::fill(fun_goto.begin(), fun_goto.end(), nullptr);
}
