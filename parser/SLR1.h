#pragma once
#include "Parser.h"
#include "Rule.h"
#include<unordered_map>
#include<variant>
#include<tuple>
#include<deque>
#include<memory>
using std::unordered_map;
using std::shared_ptr;
using std::deque;
using std::tuple;
using std::variant;


using SLRItem = tuple<int, Rule*>;

struct SLRItemSet {
    /*
    * 项目集
    * */

public:
    SLRItemSet(int id, const vector<SLRItem>&, vector<Rule>&);

    // 状态id
    int set_id;
    int item_count{ 0 };
    // 项目集中的项目
    map<Symbol*, vector<SLRItem>, symbol_ptr_less> shift_items;// 移进
    vector<SLRItem> reduce_items;// 规约
    map<Symbol*, SLRItemSet*> goto_func; // goto函数，仅用于debug


};

class SLR1 :
    public Parser
{
public:
    SLR1(const initializer_list<initializer_list<Symbol>>& rules);
    void build() override;
    void debug_parser_table()override;
    bool parse(vector<Symbol>&)override;

private:
    shared_ptr<unordered_map<Symbol*, variant<Rule*, int>>[]> _action;//action表
    shared_ptr<unordered_map<Symbol*, int>[]> _goto;//goto表
    bool valid{ true };
    unsigned _max_state{ 0 };
    Symbol* relocate_symbol(std::vector<Symbol>::iterator& iter,vector<Symbol>&v) {
        if (iter == v.end())return _dollar_symbol;
        if (iter->is_terminal()) {
            if (auto p = _valid_terminal.find(iter->description); p != _valid_terminal.end()) {
                return p->second;
            }
            else {
                //找不到符号
                return nullptr;
            }
        }
        else {
            if (auto p = _valid_nonterminal.find(iter->description); p != _valid_nonterminal.end()) {
                return p->second;
            }
            else {
                //找不到符号
                return nullptr;
            }
        }
    }
};


