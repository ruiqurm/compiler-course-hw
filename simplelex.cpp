#include"simplelex.h"
#include<cctype>
#include<array>
#include<stack>
#include<tuple>

using std::tuple;
using std::make_tuple;
using std::stack;
inline void set_type(vector<Token>& result,int type){
	result[result.size()-1].set_type(type);
}
inline void set_str(vector<Token>& result,const std::string::const_iterator & a,const std::string::const_iterator &b){
	result[result.size()-1].set_str(a,b);
}
inline void cat_str(vector<Token>& result,const std::string::const_iterator & a,const std::string::const_iterator &b){
	result[result.size()-1].cat_str(a,b);
}
shared_ptr<LexReport> parse(const string &code){
	auto report = make_shared<LexReport>();

	auto iter = code.begin();
	auto last_IDLE_pointer = code.begin();

	// 控制信号
	auto state = IDLE;
	auto jump_line = false;
	
	// 行号记录，位置记录
	auto line =1,pos=1;
	auto cusum_pos = 0;
	// 暂时没用上的三目表达式识别
	std::array<int, 32> ternary_op_stack = {};
	auto ternary_op_sp = -1;

	// 括号栈
	std::stack<tuple<char,int,decltype(iter)> > parentheses_stack;

	auto &result = report->tokens;
	try{
	while(iter!=code.end()){
		auto& ch = *iter;// 当前字符
		auto last = *last_IDLE_pointer;//上一字符
		switch (state)
		{
		case IDLE:   // 初始状态
			if(ch =='#'){ 
				// 宏
				state = MACRO;
				result.push_back(Token{TOKEN_MACRO,line,pos});
			}else if (isalpha(ch) || ch =='_'){ 
				// 标识符
				state = ID;
				result.push_back(Token{TOKEN_IDENTITY,line,pos});
			}else if(isdigit(ch)){
				// 整数（可能是）
				state = NUMBER_INTEGER;
				result.push_back(Token{TOKEN_INTEGER,line,pos});
			}else if(ch=='/'){
				if( (iter+1)!=code.end() && *(iter+1)=='/' ){
					// 注释
					state = SLASH_COMMENT;
					result.push_back(Token{TOKEN_COMMENT,line,pos});
				}else if((iter+1)!=code.end() && *(iter+1)=='*'){
					// 段注释
					state = ASTERISK_COMMENT_BS;
					result.push_back(Token{TOKEN_COMMENT,line,pos});
				}else{
					// 除法
					state = OP;
					result.push_back(Token{TOKEN_OP,line,pos});
				}
			}else if(ch=='.'){
				if((iter+1)!=code.end() && isdigit(*(iter+1) )){
					// 小数
					state = NUMBER_DECIMAL;// 形如 .2的小数
					result.push_back(Token{TOKEN_FLOAT,line,pos});
				}else{
					// .操作符
					state = IDLE;
					result.push_back(Token{TOKEN_OP,line,pos,'.'});
				}
			}else if(ch =='=' || ch=='<' || ch =='>' || ch == '!' || ch =='|' || ch =='&' || ch =='^' || ch =='*' || ch=='%' || ch=='~'){
				// 操作符
				state = OP;
				result.push_back(Token{TOKEN_OP,line,pos});
			}else if(ch=='+' || ch=='-'){
				// +,-
				state = PLUS_MINUS;
				result.push_back(Token{TOKEN_OP,line,pos});
			}else if(ch=='?'){
				// 三目表达式
				ternary_op_stack[ternary_op_sp++] = result.size();
				result.push_back(Token{TOKEN_OP,line,pos});
			}else if(ch == ':'){
				// 分号
				ternary_op_sp--;
			}else if(ch == '('||ch=='['||ch=='{'){
				// 括号
				state = IDLE;
				auto token = ch=='('?TOKEN_PARENTHESES:ch=='['?TOKEN_BRACKET:TOKEN_BRACE;
				// 确定括号的类型
				result.push_back(Token{token,line,pos,ch});
				parentheses_stack.emplace(ch,result.size()-1,iter);
			}else if(ch == ')'|| ch==']' || ch=='}'){
				// 括号
				state = IDLE;
				auto top = parentheses_stack.top();
				auto last_bracket = std::get<0>(top);
				if(last_bracket=='('&& ch==')'||
				   last_bracket=='['&& ch==']'||
				   last_bracket=='{'&& ch=='}'){
					auto token = std::get<1>(top);
					result[token].set_str(std::get<2>(top),iter+1);
					parentheses_stack.pop();
				}else{
					throw "括号未闭合";
				}
			}else if(ch == '[' || ch ==']'){
				// 中括号
				state = IDLE;
				result.push_back(Token{TOKEN_BRACKET,line,pos,ch});
			}else if(ch == '{' || ch =='}'){
				// 大括号
				state = IDLE;
				result.push_back(Token{TOKEN_BRACE,line,pos,ch});
			}else if(ch == '"'){
				// 字符串
				state = STRING_LITERAL;
				result.push_back(Token{TOKEN_STRING,line,pos});
			}else if (ch=='\''){
				// 字符
				state = CHAR_LITERAL;
				result.push_back(Token{TOKEN_CHAR,line,pos});
			}else{
				// do nothing
			}

			last_IDLE_pointer =  iter;
			break;

		case MACRO:     // 宏
			if(ch=='\n'){
				if(!jump_line){
					state = IDLE;
					set_str(result,last_IDLE_pointer,iter);
					// 不需要重新检查\n
				}else{
					jump_line = false;
				}
			}else if(ch=='\\'){
				jump_line = true;
			}
			break;
		case ID:      // 标识符
			if (!(isalpha(ch) || isdigit(ch) || ch=='_')){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN; // 重新检查当前字符
			}
			break;

		case NUMBER_INTEGER:    // 数字整数
			if(isdigit(ch)){
				state = NUMBER_INTEGER;
			}else if(ch=='E' || ch =='e'){
				state = NUMBER_EXP_SYMBOL;
				set_type(result,TOKEN_FLOAT);
			}else if(ch=='.'){
				state = NUMBER_DOT_SIGN;
				set_type(result,TOKEN_FLOAT);
			}else if(last =='0' && ch=='x'){
				//16进制
				state = NUMBER_HEX_FIRST;
			}else if(ch=='u'||ch=='U'||ch=='L'||ch=='l'){
				state = NUMBER_INTEGER_SUFFIX;
			}else{
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN; // 重新检查当前字符
			}
			break;
		
		case NUMBER_DOT_SIGN:   // 数字小数点
			if(isdigit(ch)){
				state = NUMBER_DECIMAL;
			}else{
				throw "error NUMBER_DOT_SIGN";
			}
			break;

		case NUMBER_DECIMAL:  // 数字分数
			if(ch=='E'||ch == 'e'){
				state = NUMBER_EXP_SYMBOL;
			}else if(!isdigit(ch)){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN; // 重新检查当前字符
			}
			break;
		case NUMBER_EXP_SYMBOL: // 数字E
			if(isdigit(ch)){
				state = NUMBER_EXP;
			}else if(ch=='+'||ch=='-'){
				state = NUMBER_EXP_SIGN;
			}else{
				throw "error NUMBER_EXP_SYMBOL";
			}
			break;
		case NUMBER_EXP_SIGN: // 数字指数部分的符号
			if(isdigit(ch)){
				state = NUMBER_EXP;
			}else{
				throw "NUMBER_EXP_SIGN";
			}
		case NUMBER_EXP:
			if (isdigit(ch)){
				state = NUMBER_EXP;
			}else{
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN; // 重新检查当前字符
			}
			break;
		case NUMBER_HEX_FIRST:
			if(!(isdigit(ch) || ch=='a'|| ch=='A'||
			 ch=='b'|| ch=='B'|| ch=='c'|| ch=='C'||
			 ch =='d'||ch=='D' || ch=='e' || ch=='E'||
			 ch =='f'|| ch=='F')){
				 throw "invalid hex";
			 }else{
				 state = NUMBER_HEX;
			 }
			 break;
		case NUMBER_HEX:
			if(ch=='u'||ch=='U'||ch=='L'||ch=='l'){
				state = NUMBER_INTEGER_SUFFIX;
			}else if(!(isdigit(ch) || ch=='a'|| ch=='A'||
			 ch=='b'|| ch=='B'|| ch=='c'|| ch=='C'||
			 ch =='d'||ch=='D' || ch=='e' || ch=='E'||
			 ch =='f'|| ch=='F')){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN;
			 }
			break;
		case NUMBER_INTEGER_SUFFIX:
			if(ch=='u'||ch=='U'||ch=='L'||ch=='l'){
				state = NUMBER_INTEGER_SUFFIX;
			}else{
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN;
			}
			break;
		case SLASH_COMMENT:
			if (ch=='/'){
				state = SLASH_COMMENT_CONTENT;
			}else{
				throw "error SLASH_COMMENT";
			}
			break;
		case SLASH_COMMENT_CONTENT:
			if (ch=='\n'){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				// 不需要重新检查\n
			}
			break;
		case ASTERISK_COMMENT:
			if(ch=='*')state=ASTERISK_COMMENT_ESLASH;
			break;

		case ASTERISK_COMMENT_BS:
			if(ch=='*')state=ASTERISK_COMMENT;
			else throw "ASTERISK_COMMENT2";
			break;

		case ASTERISK_COMMENT_ESLASH:
			if(ch=='/'){
				state=IDLE;
				set_str(result,last_IDLE_pointer,iter+1);
			}
			else state = ASTERISK_COMMENT;
			break;
		case OP:
			state = IDLE;
			if (last =='='){
				if(ch=='='){
					set_str(result,last_IDLE_pointer,iter+1);
				}else{
					set_str(result,last_IDLE_pointer,iter);
					goto CHECK_AGAIN;// 重新检查
				}
			}else if(last == '*' || last =='/' || last =='%' || last =='!'|| last == '^' || last =='~'){
				if(ch=='='){
					set_str(result,last_IDLE_pointer,iter+1);
				}else{
					set_str(result,last_IDLE_pointer,iter);
					goto CHECK_AGAIN;// 重新检查
				}
			}else if(last=='&' || last  =='|'){
				if ( (ch=='&' && last =='&') || (ch =='|' && last=='|') ){
					//&& ||
					set_str(result,last_IDLE_pointer,iter+1);
				}else if(ch=='='){
					set_str(result,last_IDLE_pointer,iter+1);
				}else{
					set_str(result,last_IDLE_pointer,iter);
					goto CHECK_AGAIN;// 重新检查
				}
			}else if(last == '>' || last == '<'){
				if(ch=='='){
					// >= <=
					set_str(result,last_IDLE_pointer,iter+1);
				}else if(ch == '>' || ch =='<'){
					state = SHIFT_ASSIGN;
					set_str(result,last_IDLE_pointer,iter+1);
				}else{
					set_str(result,last_IDLE_pointer,iter);
					goto CHECK_AGAIN;// 重新检查
				}
			}else{
				throw "ss";
			}
			break;
		case SHIFT_ASSIGN:
			state = IDLE;
			if(ch=='='){
				set_str(result,last_IDLE_pointer,iter+1);
			}else{
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN;// 重新检查
			}
			break;
		case PLUS_MINUS:
			state = IDLE;
			if(ch == last || last == '-' && ch =='>' || ch =='='){
				set_str(result,last_IDLE_pointer,iter+1);
			}else{
				set_str(result,last_IDLE_pointer,iter);
				goto CHECK_AGAIN;// 重新检查
			}
		case STRING_LITERAL:
			if(ch=='"'){
				state = IDLE;
				cat_str(result,last_IDLE_pointer,iter+1);
			}else if(ch=='\\' && (iter+1)!=code.end() && (*(iter+1)==' '|| *(iter+1)=='\n') ){
				jump_line = true;
				cat_str(result,last_IDLE_pointer,iter);
			}else if(ch=='\n'){
				if(!jump_line)
					throw "STRING_LITERAL";
				else{
					last_IDLE_pointer = iter;
					jump_line = false;
				}
			}
			break;
		case CHAR_LITERAL:
			if(ch=='\\'){
				state=CHAR_TRANSFORM;
			}else{
				state = CHAR_END;
			}
			break;
		case CHAR_TRANSFORM:
			if(ch=='x'){
				// '\x12'
				state = CHAR_X;
			}else if(isdigit(ch)){
				// '\123'
				state = CHAR_DIGIT;
			}else{
				// '\n'
				state = CHAR_END;
			}
			break;
		case CHAR_X:
			if(isdigit(ch)){
				state = CHAR_DIGIT;
			}else{
				"CHAR_DIGIT error";	
			}
			break;
		case CHAR_DIGIT:
			if(isdigit(ch)){
				state  = CHAR_DIGIT;
			}else if(ch=='\''){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter+1);
			}else{
				throw "CHAR_DIGIT error";
			}
			break;
		case CHAR_END:
			if(ch=='\''){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter+1);
			}else{
				throw "CHAR_DIGIT error";
			}
			break;
		default:
			throw "unknow state";
			break;
		}

		NORMAL:
			if(ch=='\n'){
				cusum_pos += pos;
				pos = 1;
				line+=1;
			}else{
				pos++;
			}
			iter++;
			continue;

		CHECK_AGAIN:
			// 重新检查当前字符
			state = IDLE;
			continue;

		JUMP:// 向后多跳一格
			if(ch=='\n')
				{cusum_pos += pos;pos = 1;line+=1;}
			else 
				pos++;
			iter++;
			if(ch=='\n')
				{cusum_pos += pos;pos = 1;line+=1;}
			else 
				pos++;
			last_IDLE_pointer =  iter;
			iter++;
			continue;
	
	}
	}catch(const char* str){
		printf("%s",str);
	}

	// 一些统计信息
	report->lines = line;
	report->count = code.size();
	for(auto& token:result){
		if(token.type == TOKEN_IDENTITY || token.type == TOKEN_KEYWORD)
			report->words++;
	}
	report->failed = report->errors.size() == 0;
	return report;
}

ostream& operator <<(ostream& os,const Token& token){
	if(!token.filename.empty()){
		os<<token.filename<<":";
	}
	os<<token.line<<":"<<token.pos<<" "<<"type: ";
	switch (token.type)
	{
	case TOKEN_MACRO:os<<"宏";break;
	case TOKEN_IDENTITY:os<<"标识符";break;
	case TOKEN_KEYWORD:os<<"关键字";break;
	case TOKEN_FLOAT:os<<"浮点数";break;
	case TOKEN_INTEGER:os<<"整数";break;
	case TOKEN_COMMENT:os<<"注释";break;
	case TOKEN_OP:os<<"操作符";break;
	case TOKEN_CHAR:os<<"字符";break;
	case TOKEN_STRING:os<<"字符串";break;
	case TOKEN_PARENTHESES:os<<"小括号";break;
	case TOKEN_BRACKET:os<<"中括号";break;
	case TOKEN_BRACE:os<<"大括号";break;
	default:os<<"未知";break;
	}
	os<<" content:"<<token.content<<"\n";
	return os;
}

ostream& operator <<(ostream& os,const Error& error){
	if(!error.filename.empty()){
		os<<error.filename<<":";
	}
	os<<error.line<<":"<<error.pos<<" "<<"错误: "<<error.description;
	return os;
}

string Token::filename {};
string Error::filename {};

#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;

int main(){
	auto filename = "./test_data/damnit.c";
	ifstream fs(filename);
	stringstream buf;
	Token::filename.assign(filename);
	Error::filename.assign(filename);
	if(fs.is_open()){
		vector<Token> v;
		buf << fs.rdbuf();
		const string code { buf.str()};
		auto result = parse(code);
		sort(result->tokens.begin(),result->tokens.end());
		for(auto &token:result->tokens){
			cout<<token;
		}
	}else{
		cout<<"No such file";
		return -1;
	}
	return 0;
}