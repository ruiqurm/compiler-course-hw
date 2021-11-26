#pragma once
#include "Parser.h"
#include "Rule.h"
#include<variant>
#include<tuple>
using std::tuple;
using std::variant;
class SLR1 :
    public Parser
{
public:
    SLR1(const initializer_list<initializer_list<Symbol>>& rules);
    void build() override;

private:
    //void pre_find_first_follow(vector<vector<Symbol>>& rules)override;
    bool _parse(vector<Symbol>&)override { return false; }
    //map<Symbol, map<Symbol, variant<Rule&, int>>>_action;
    //map<Symbol, int>_goto;
    bool valid{ true };
};

using Item = tuple<int, Rule*>;

struct Item_less {
    bool operator()(const Item& lhs, const Item& rhs)const {
        return *get<1>(lhs) < *get<1>(rhs) || (*get<1>(lhs) == *get<1>(rhs) && get<0>(lhs) < get<0>(rhs));
    }
};

struct ItemSet {
    /*
    * 项目集
    * */
public:
    ItemSet(int id, const vector<Item>&, vector<Rule>&);
    
    // 状态id
    int set_id;
    int item_count{ 0 };
    // 项目集中的项目
    vector<Item> shift_items;// 移进
    vector<Item> reduce_items;// 规约
    vector<ItemSet*> fun_goto; // goto函数

};
