#pragma once
#include<string>
#include<vector>
#include<map>
#include<iostream>
#include<memory>
#include"trie.h"
using std::shared_ptr;
using std::make_shared;
using std::ostream;
using std::string;
using std::vector;
using std::map;
/**
 * @brief 符号信息
 * 
 */
struct Token{
	Token(int type,int line,int pos):
		type(type),line(line),pos(pos){}
	Token(int type,int line,int pos,char c):
		type(type),line(line),pos(pos),content(1,c){}
	Token(int type,int line,int pos,const char *str):
		type(type),line(line),pos(pos),content(str){}
	void set_type(int _type){
		type = _type;
	}
	void set_str(const std::string::const_iterator& a,const std::string::const_iterator& b){
		content.assign(a,b);
	}
	void cat_str(const std::string::const_iterator& a,const std::string::const_iterator& b){
		content.append(a,b);
	}
	bool operator <(const Token& t1){
		return line<t1.line || line==t1.line && pos <t1.pos;
	}
	bool operator >(const Token& t1){
		return line>t1.line || line==t1.line && pos >t1.pos;
	}
	friend ostream& operator <<(ostream& os,const Token& token);
	static string filename; 
	int type;
	int line; // 行号
	int pos; // 在每行的位置
	string content; // 具体的内容
};

struct Error{
	public:
		Error(int line,int pos,const std::string::const_iterator& iter,string &description):
			line(line),pos(pos),iter(iter),description(description){}
		Error(int line,int pos,const std::string::const_iterator& iter,string description):
			line(line),pos(pos),iter(iter),description(description){}	
		friend ostream& operator <<(ostream& os,const Error& error);
		bool operator <(const Error& t1){
		return line<t1.line || line==t1.line && pos <t1.pos;
		}
		bool operator >(const Error& t1){
			return line>t1.line || line==t1.line && pos >t1.pos;
		}

		int line;
		int pos;
		std::string::const_iterator iter; // 字符串内的位置
		string description;
		static string filename;
};


enum{
	TOKEN_MACRO, 
	TOKEN_IDENTITY, 
	TOKEN_KEYWORD, 
	TOKEN_FLOAT, 
	TOKEN_INTEGER, 
	TOKEN_COMMENT, 
	TOKEN_OP, 
	TOKEN_CHAR, 
	TOKEN_STRING, 
	TOKEN_PARENTHESES, 
	TOKEN_BRACKET,
	TOKEN_BRACE, 
};

struct LexReport{
	LexReport():lines(0),words(0),count(0),is_failed(true),is_error(true){}
	vector<Token> tokens;
	vector<Error> errors;
	int lines;  // 行数
	int words; // 单词总数
	int count; // 字符总数
	bool is_error;  // 是否出现错误
	bool is_failed; //是否异常终止
};
const vector<string> keywords {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};
shared_ptr<LexReport> parse(const string &code);
