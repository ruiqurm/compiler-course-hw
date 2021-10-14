#pragma once
#include<string>
#include<vector>
#include<map>
#include<iostream>
using std::ostream;
using std::string;
using std::vector;
using std::map;
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
		int line;
		int pos;
		string content;
};



enum STAGE{
	IDLE=0,
	MACRO,
	ID,
	
	//数字
	NUMBER_INTEGER,NUMBER_DOT_SIGN,NUMBER_DECIMAL,NUMBER_EXP_SYMBOL,NUMBER_EXP_SIGN,NUMBER_EXP, 
	NUMBER_HEX_FIRST,NUMBER_HEX,//16进制
	NUMBER_INTEGER_SUFFIX,//整数后缀UL

	// 处理注释
	SLASH_COMMENT,SLASH_COMMENT_CONTENT, 
	ASTERISK_COMMENT,ASTERISK_COMMENT_BS,ASTERISK_COMMENT_ESLASH,

	// 操作符
	OP,
	SHIFT_ASSIGN,
	PLUS_MINUS, //+ -
	TERNARY_OP, //三目运算符
	
	
	// 括号
	// PARENTHESES,
	// BRACKET,
	// BRACE,

	// 字符串
	STRING_LITERAL,
	CHAR_LITERAL,CHAR_TRANSFORM,CHAR_X,CHAR_DIGIT,
	CHAR_END
};
enum TOKEN_TYPE{
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

// class LexReport{
// 	private:
// 		vector<Token> v;
// 		vector<Error> e;
// };

void parse(const string &code,vector<Token>& result);
