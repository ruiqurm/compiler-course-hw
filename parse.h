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

	void set_str(const std::string::const_iterator& a,const std::string::const_iterator& b){
		content.assign(a,b);
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
	NUMBER_INTEGER,NUMBER_DOT_SIGN,NUMBER_DECIMAL,NUMBER_EXP_SYMBOL,NUMBER_EXP_SIGN,NUMBER_EXP, // 处理数字
	SLASH_COMMENT,SLASH_COMMENT_CONTENT, // 处理注释
	ASTERISK_COMMENT,ASTERISK_COMMENT_BS1,ASTERISK_COMMENT_BS2,ASTERISK_COMMENT_ESLASH,
	
};
enum TOKEN_TYPE{
	TOKEN_MACRO,
	TOKEN_IDENTITY,
	TOKEN_KEYWORD,
	TOKEN_NUMBER,
	TOKEN_COMMENT,
	TOKEN_ASSIGN,
	TOKEN_ARTHOP,
	TOKEN_LOGOP,
	TOKEN_BITWISEOP,
	TOKEN_STRING,
	TOKEN_PARENTHESES,
	TOKEN_BRACKET,
	TOKEN_BRACE,
	
};

void parse(const string &code,vector<Token>& result);
