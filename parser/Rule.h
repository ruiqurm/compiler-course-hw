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
	//Rule(const initializer_list<Symbol>& l) {
	//	if (l.size() <= 1)
	//		throw std::runtime_error("不合法的表达式");
	//	_from = *l.begin();
	//	std::cout << _from.description << "-->";
	//	for (auto iter = l.begin() + 1; iter != l.end(); iter++) {
	//		std::cout << iter->description;
	//		_to.emplace_back(*iter);
	//	
	//	}
	//	std::cout << std::endl;
	//}
	Symbol* from() { return _from; }
	vector<Symbol*>& to() { return _to; }
private:
	Symbol* _from;
	vector<Symbol*> _to;
};



