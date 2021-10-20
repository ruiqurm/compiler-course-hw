#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include "trie.h"
using std::make_shared;
using std::map;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;
/**
 * @brief 符号信息
 * 
 */
struct Token
{
	Token(int type, int line, int pos) : type(type), line(line), pos(pos) {}
	Token(int type, int line, int pos, char c) : type(type), line(line), pos(pos), content(1, c) {}
	Token(int type, int line, int pos, const char *str) : type(type), line(line), pos(pos), content(str) {}
	void set_type(int _type)
	{
		type = _type;
	}
	void set_str(const std::string::const_iterator &a, const std::string::const_iterator &b)
	{
		content.assign(a, b);
	}
	void cat_str(const std::string::const_iterator &a, const std::string::const_iterator &b)
	{
		content.append(a, b);
	}
	bool operator<(const Token &t1)
	{
		return line < t1.line || line == t1.line && pos < t1.pos;
	}
	bool operator>(const Token &t1)
	{
		return line > t1.line || line == t1.line && pos > t1.pos;
	}
	friend ostream &operator<<(ostream &os, const Token &token);
	static string filename;
	int type;
	int line;		// 行号
	int pos;		// 在每行的位置
	string content; // 具体的内容
};

struct Error
{
public:
	Error(int line, int pos, const std::string::const_iterator &iter, string &description) : line(line), pos(pos), iter(iter), description(description) {}
	Error(int line, int pos, const std::string::const_iterator &iter, string description) : line(line), pos(pos), iter(iter), description(description) {}
	friend ostream &operator<<(ostream &os, const Error &error);
	bool operator<(const Error &t1)
	{
		return line < t1.line || line == t1.line && pos < t1.pos;
	}
	bool operator>(const Error &t1)
	{
		return line > t1.line || line == t1.line && pos > t1.pos;
	}

	int line;
	int pos;
	std::string::const_iterator iter; // 字符串内的位置
	string description;
	static string filename;
};

enum
{
	TOKEN_MACRO,
	TOKEN_IDENTITY,
	TOKEN_KEYWORD,
	TOKEN_FLOAT_NUMBER,
	TOKEN_INTEGER,
	TOKEN_COMMENT,
	TOKEN_OP,
	TOKEN_CHAR_LITERAL,
	TOKEN_STRING,
	TOKEN_PARENTHESES,
	TOKEN_BRACKET,
	TOKEN_BRACE,

	TOKEN_AUTO,
	TOKEN_BREAK,
	TOKEN_CASE,
	TOKEN_CHAR,
	TOKEN_CONST,
	TOKEN_CONTINUE,
	TOKEN_DEFAULT,
	TOKEN_DO,
	TOKEN_DOUBLE,
	TOKEN_ELSE,
	TOKEN_ENUM,
	TOKEN_EXTERN,
	TOKEN_FLOAT,
	TOKEN_FOR,
	TOKEN_GOTO,
	TOKEN_IF,
	TOKEN_INT,
	TOKEN_LONG,
	TOKEN_REGISTER,
	TOKEN_RETURN,
	TOKEN_SHORT,
	TOKEN_SIGNED,
	TOKEN_SIZEOF,
	TOKEN_STATIC,
	TOKEN_STRUCT,
	TOKEN_SWITCH,
	TOKEN_TYPEDEF,
	TOKEN_UNION,
	TOKEN_UNSIGNED,
	TOKEN_VOID,
	TOKEN_VOLATILE,
	TOKEN_WHILE,
};

struct LexReport
{
	LexReport() : lines(0), words(0), count(0), is_failed(true), is_error(true) {}
	vector<Token> tokens;
	vector<Error> errors;
	int lines;		// 行数
	int words;		// 单词总数
	int count;		// 字符总数
	bool is_error;	// 是否出现错误
	bool is_failed; //是否异常终止
};
const map<string, int> keywords{{"auto", TOKEN_AUTO},
								{"break", TOKEN_BREAK},
								{"case", TOKEN_CASE},
								{"char", TOKEN_CHAR},
								{"const", TOKEN_CONST},
								{"continue", TOKEN_CONTINUE},
								{"default", TOKEN_DEFAULT},
								{"do", TOKEN_DO},
								{"double", TOKEN_DOUBLE},
								{"else", TOKEN_ELSE},
								{"enum", TOKEN_ENUM},
								{"extern", TOKEN_EXTERN},
								{"float", TOKEN_FLOAT},
								{"for", TOKEN_FOR},
								{"goto", TOKEN_GOTO},
								{"if", TOKEN_IF},
								{"int", TOKEN_INT},
								{"long", TOKEN_LONG},
								{"register", TOKEN_REGISTER},
								{"return", TOKEN_RETURN},
								{"short", TOKEN_SHORT},
								{"signed", TOKEN_SIGNED},
								{"sizeof", TOKEN_SIZEOF},
								{"static", TOKEN_STATIC},
								{"struct", TOKEN_STRUCT},
								{"switch", TOKEN_SWITCH},
								{"typedef", TOKEN_TYPEDEF},
								{"union", TOKEN_UNION},
								{"unsigned", TOKEN_UNSIGNED},
								{"void", TOKEN_VOID},
								{"volatile", TOKEN_VOLATILE},
								{"while", TOKEN_WHILE}};
shared_ptr<LexReport> parse(const string &code);
