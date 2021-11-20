#pragma once
#include<iostream>
#include<vector>
#include<initializer_list>
#include<exception>
#include"Symbol.h"

using std::vector;
using std::ostream;
using std::initializer_list;
using std::reference_wrapper;
class Rule
{
public:
	Rule(Symbol& from) :_from(&from) {}
	Symbol* from() { return _from; }
	vector<Symbol*>& to() { return _to; }
	friend ostream& operator<<(ostream& os,const Rule&r);
private:
	Symbol* _from;
	vector<Symbol*> _to;
};


