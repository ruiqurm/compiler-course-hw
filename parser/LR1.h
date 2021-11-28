#pragma once
#include "Parser.h"
#include<tuple>
#include<memory>
#include<unordered_map>
#include<variant>
using std::variant;
using std::unordered_map;
using std::shared_ptr;
using std::tuple;

using Item = tuple<int, Rule*,Symbol*>;

struct ItemSet {
    /*
    * 项目集
    * */

public:
    ItemSet(int id, const vector<Item>&, vector<Rule>&, Parser::symbol_lookup_set_t&);

    // 状态id
    int set_id;
    int item_count{ 0 };
    // 项目集中的项目
    map<Symbol*, vector<Item>, symbol_ptr_less> shift_items;// 移进
    vector<Item> reduce_items;// 规约
    map<Symbol*, ItemSet*> goto_func; // goto函数，仅用于debug

};

class LR1 :
    public Parser
{
public:
    LR1(const initializer_list<initializer_list<Symbol>>& rules);
    void build() override;
    //void debug_parser_table()override;
    //bool parse(vector<Symbol>&)override;
private:
    shared_ptr<unordered_map<Symbol*, variant<Rule*, int>>[]> _action;//action表
    shared_ptr<unordered_map<Symbol*, int>[]> _goto;//goto表
    bool valid{ true };
    unsigned _max_state{ 0 };
};

