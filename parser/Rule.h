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
	//const Symbol* from() const{ return _from; }
	vector<Symbol*>& to() { return _to; }
	//const vector<Symbol*>& to() const{ return _to; }

	friend ostream& operator<<(ostream& os,const Rule&r);
	void set_id(int id) { _id = id; }
	bool operator <(const Rule& r)const {
		return _id < r._id;
	}
	bool operator ==(const Rule& r)const {
		return _id == r._id;
	}
private:
	int _id{ 0 };
	Symbol* _from;
	vector<Symbol*> _to;
};


