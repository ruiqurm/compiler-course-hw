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
private:
    void pre_find_first_follow(vector<vector<Symbol>>& rules)override;
    bool _parse(vector<Symbol>&)override;
    map<Symbol, map<Symbol, variant<Rule&, int>>>_action;
    map<Symbol, int>_goto;
    bool valid{ true };
};


struct ItemSet {
    /*
    * 项目集
    * */
public:
    ItemSet(int id, const vector<tuple<int, Rule&>>&,const vector<Rule&>&);
    
    // 状态id
    int set_id;
    // 项目集中的项目
    vector<tuple<int, Rule&>> shift_items;// 移进
    vector<tuple<int, Rule&>> reduce_items;// 规约
    vector<tuple<int, Rule&>> pending_items;// 待约

};