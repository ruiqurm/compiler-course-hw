#include "LR1.h"
#include<queue>
#include<deque>
using std::deque;
auto LR1_Item_less = [](const Item& lhs, const Item& rhs) {
	return *get<1>(lhs) < *get<1>(rhs) || (*get<1>(lhs) == *get<1>(rhs) && get<0>(lhs) < get<0>(rhs)) ||
		*get<1>(lhs) == *get<1>(rhs) && get<0>(lhs) == get<0>(rhs) && get<2>(lhs)->id < get<2>(rhs)->id;
};

ItemSet::ItemSet(int id, const vector<Item>& now_rules, vector<Rule>& all_rules,
	Parser::symbol_lookup_set_t& first) :
	set_id(id)
{
	std::queue<Item>q;// BFS 队列
	set <Item, decltype(LR1_Item_less)> all_items(LR1_Item_less);
	for (auto& v : now_rules) {
		q.push(v);
		all_items.insert(v);
	}
	while (!q.empty()) {
		item_count++;
		auto [dot, rule,psymbol] = q.front();
		//如果可以扩展
		if (rule->to().size() > dot) {
			// 如果是非终结符
			auto& this_sym = *rule->to()[dot];

			// 移进项目
			auto& shift_item = shift_items[&this_sym];
			shift_item.push_back(q.front());
			goto_func[&this_sym] = nullptr;

			if (rule->to()[dot]->is_nonterminal()) {
				// 待约项目
				// 插入所有带这个非终结符的文法
				for (auto& r : all_rules) {
					if (*r.from() == this_sym) {
						// 如果出现重复，不插入
						auto old_size = all_items.size();
						if (rule->to().size() <= dot + 1) {
							auto new_tuple = Item(0, &r, psymbol);
							all_items.insert(new_tuple);
							if (old_size != all_items.size()) {
								q.emplace(new_tuple);
							}
						}
						else {
							auto beta = rule->to()[dot + 1];
							for (auto sym : first[beta]) {
								if (!sym->is_null()) {
									// 否则加入beta，即当前非终结符的follow集
									auto new_tuple = Item(0, &r, sym);
									all_items.insert(new_tuple);
									if (old_size != all_items.size()) {
										q.emplace(new_tuple);
									}
								}
								else {
									// 如果致空，加入原来的符号
									auto new_tuple = Item(0, &r, psymbol);
									all_items.insert(new_tuple);
									if (old_size != all_items.size()) {
										q.emplace(new_tuple);
									}
								}
							}
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

}


LR1::LR1(const initializer_list<initializer_list<Symbol>>& rrr) :Parser(rrr) {
	auto& rules = *_tmp_rules_pointer;
	// 拓广文法
	rules.insert(rules.begin(),
		vector<Symbol>{
		Symbol(Symbol::TYPE::nonterminal, rules[0][0].description + "'"),
			Symbol(Symbol::TYPE::nonterminal, rules[0][0].description)
	}
	);
}


void LR1::build() {
	Parser::build();
	using itemset_pointer = ItemSet*;
	using tmp_vec_pointer = vector<Item>::iterator;

	auto itemsets_id = 0;
	deque<ItemSet> itemsets;	 // 保存实体项目集
	std::queue<itemset_pointer>q; // BFS遍历所有可能的项目集
	vector<Item> tmp{ Item(0, &_rules[0],_dollar_symbol)};// 暂时存放新的核
	std::map<vector<Item>, itemset_pointer> kernel;// 所有核的集合。防止重复加入

	// 初始核
	kernel.emplace(tmp, nullptr);
	auto& _ = itemsets.emplace_back(itemsets_id++, tmp, _rules,_first);
	kernel[tmp] = &_;
	q.push(&_);

	while (!q.empty()) {
		auto iter = q.front();

		// 拓展新的节点
		for (const auto& [key, value] : iter->shift_items) {
			// key是Symbol*,value是item列表

			// 替换tmp表内容，等下用来初始化itemset
			tmp.clear();
			for (auto& v : value) {
				tmp.push_back(Item(get<0>(v) + 1, get<1>(v),get<2>(v)));
			}

			// 如果 核kernel 和之前的是一样的，说明重复了不需要再加入队列。
			if (auto find_p = kernel.find(tmp); find_p == kernel.end()) {
				auto& ret = itemsets.emplace_back(itemsets_id++, tmp, _rules, _first);
				kernel[tmp] = &ret;
				if (itemsets.back().shift_items.size() > 0) {
					q.push(&ret);
				}
				// 链接旧的节点和新的节点
				iter->goto_func[key] = &ret;
			}
			else {
				iter->goto_func[key] = find_p->second;
			}
		}
		q.pop();

	}

	// 查看DFA构建状况
	for (auto& ref : itemsets) {
		cout << "-----------------";
		cout << "id" << ref.set_id << endl;
		cout << "count" << ref.item_count << endl;
		cout << "shift:" << endl;

		for (auto& shift_ref : ref.shift_items) {
			for (auto& p : shift_ref.second) {
				auto& rule = get<1>(p);
				auto pos = get<0>(p);
				auto lookahead = get<2>(p);
				cout << rule->from()->description << "->";
				for (auto sym : rule->to()) {
					if (pos == 0)cout << '.';
					cout << sym->description;
					pos--;
				}
				cout << " " << lookahead->description;
				cout << endl;
			}
		}
		cout << "reduce:" << endl;
		for (auto& reduce_ref : ref.reduce_items) {
			auto rule = get<1>(reduce_ref);
			auto lookahead = get<2>(reduce_ref);
			cout << rule->from()->description << "->";
			for (auto sym : rule->to()) {
				cout << sym->description;
			}
			cout << ".";
			cout << " " << lookahead->description;
			cout << endl;
		}
		cout << "goto:" << endl;
		for (auto& [key, value] : ref.goto_func) {
			cout << "[" << ref.set_id << "," << key->description << "]" << value->set_id << endl;
		}
		cout << endl;
	}

	// 构建分析表
	//_max_state = itemsets_id;
	//_action = std::make_shared<unordered_map<Symbol*, variant<Rule*, int>>[]>(_max_state);
	//_goto = std::make_shared<unordered_map<Symbol*, int>[]>(_max_state);
	//for (auto& ref : itemsets) {
	//	// 遍历所有归约项
	//	for (auto& shift_ref : ref.shift_items) {
	//		auto p = shift_ref.first;
	//		if (p->is_terminal()) {
	//			// 如果是终结符，写入action表
	//			_action[ref.set_id][p] = std::variant<Rule*, int>(ref.goto_func[p]->set_id);
	//		}
	//		else {
	//			// 如果是非终结符，写入goto表
	//			_goto[ref.set_id][p] = ref.goto_func[p]->set_id;
	//		}
	//	}
	//	for (auto& reduce_ref : ref.reduce_items) {
	//		auto rule = get<1>(reduce_ref);
	//		for (auto& follow_x : _follow[rule->from()]) {
	//			_action[ref.set_id][follow_x] = std::variant<Rule*, int>(rule);
	//		}

	//	}

	//}
}