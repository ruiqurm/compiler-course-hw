#include"parse.h"
#include<cctype>



inline void set_str(vector<Token>& result,const std::string::const_iterator & a,const std::string::const_iterator &b){
	result[result.size()-1].set_str(a,b);
}
void parse(const string &code,vector<Token>& result){
	auto iter = code.begin();
	auto last_IDLE_pointer = code.begin();
	auto state = IDLE;
	auto line =1,pos=1;
	bool check_again = false;
	while(iter!=code.end()){
		auto& ch = *iter;
		switch (state)
		{
		case IDLE:   // 初始状态
			if(ch =='#'){
				state = MACRO;
				result.push_back(Token{TOKEN_MACRO,line,pos});
			}else if (isalpha(ch) || ch =='_'){
				state = ID;
				result.push_back(Token{TOKEN_IDENTITY,line,pos});
			}else if(isdigit(ch)){
				state = NUMBER_INTEGER;
				result.push_back(Token{TOKEN_NUMBER,line,pos});
			}else if(ch=='=' && (iter+1)!=code.end() && *(iter+1)!='=' ){
				// 不需要改变状态
				result.push_back(Token{TOKEN_ASSIGN,line,pos,'='});
			}else if(ch=='/'){
				if( (iter+1)!=code.end() && *(iter+1)=='/' ){
					state = SLASH_COMMENT;
					result.push_back(Token{TOKEN_COMMENT,line,pos});
				}else{
					state = ASTERISK_COMMENT;
					result.push_back(Token{TOKEN_COMMENT,line,pos});
				}
			}else{
				// can't detect;do nothing
			}

			last_IDLE_pointer =  iter;
			break;

		case MACRO:     // 宏
			if(ch=='\n'){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				// 不需要重新检查\n
			}
			break;
		case ID:      // 标识符
			if (!(isalpha(ch) || isdigit(ch) || ch=='_')){
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				check_again = true;// 重新检查当前字符
			}
			break;

		case NUMBER_INTEGER:    // 数字整数
			if(isdigit(ch)){
				state = NUMBER_INTEGER;
			}else if(ch=='E' || ch =='e'){
				state = NUMBER_EXP_SYMBOL;
			}else if(ch=='.'){
				state = NUMBER_DOT_SIGN;
			}else{
				state = IDLE;
				set_str(result,last_IDLE_pointer,iter);
				check_again = true; // 重新检查当前字符
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
				check_again = true;// 重新检查当前字符
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
				check_again = true;// 重新检查当前字符
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
			if(ch=='*')state=ASTERISK_COMMENT_BS1;
			else throw "ASTERISK_COMMENT1";
			break;

		case ASTERISK_COMMENT_BS1:
			if(ch=='*')state=ASTERISK_COMMENT_BS2;
			else throw "ASTERISK_COMMENT2";
			break;

		case ASTERISK_COMMENT_BS2:
			if(ch=='*')
			state=ASTERISK_COMMENT_ESLASH;
			break;
		case ASTERISK_COMMENT_ESLASH:
			if(ch=='/'){
				state=IDLE;
				set_str(result,last_IDLE_pointer,iter+1);
			}
			else state = ASTERISK_COMMENT_BS2;
			break;
		default:
			throw "unknow state";
			break;
		}
		if(!check_again){
			if(ch=='\n'){
				pos = 1;
				line+=1;
			}else{
				pos++;
			}
			iter++;
			continue;
		}else{
			check_again = false;
		}	
	
	}
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
	case TOKEN_NUMBER:os<<"数字";break;
	case TOKEN_COMMENT:os<<"注释";break;
	case TOKEN_ASSIGN:os<<"赋值";break;
	default:os<<"未知";break;
	}
	os<<" content:"<<token.content<<"\n";
	return os;
}

#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;
string Token::filename {};
int main(){
	ifstream fs("./test_data/numeric.c");
	stringstream buf;
	Token::filename.assign("./test_data/numeric.c");
	if(fs.is_open()){
		vector<Token> v;
		buf << fs.rdbuf();
		const string code { buf.str()};
		parse(code,v);
		sort(v.begin(),v.end());
		for(auto &token:v){
			cout<<token;
		}
	}else{
		cout<<"No such file";
		return -1;
	}

}