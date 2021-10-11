#include<map>
#include<string>
#include<cctype>
#include<vector>
using namespace std;

struct Token{
	Token(int type,int line,int pos):
		type(type),line(line),pos(pos),start(nullptr){}
	Token(int type,int line,int pos,char c):
		type(type),line(line),pos(pos),start(nullptr),content(c){}
	Token(int type,int line,int pos):
		type(type),line(line),pos(pos){}
	void set_str(const char* start,const char* end){
		content.assign(start,end,end-start+1);
	}
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
	
};
enum TOKEN_TYPE{
	TOKEN_MACRO,
	TOKEN_IDENTITY,
	TOKEN_KEYWORD,
	TOKEN_NUMBER,
	TOKEN_COMMENT,
	TOKEN_ASSIGN,
};
inline void set_str(vector<Token>& result,std::string::iterator& a,std::string::iterator &b){
	result[result.size()-1].set_str(a,b);
}
void parse(string &code,vector<Token>& result){
	auto iter = code.begin();
	auto last_IDLE_pointer = code.begin();
	auto state = IDLE;
	auto line =0,pos=0;
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
			}else if(ch=='=' && iter!=code.end() && *(iter+1)!='=' ){
				// 不需要改变状态
				result.push_back(Token{TOKEN_ASSIGN,line,pos});
			}else if(ch=='/'){
				state = SLASH_COMMENT;
				result.push_back(Token{TOKEN_COMMENT,line,pos});
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
				state = NUMBER_INTEGER
			}else if(ch=='E' || ch =='e'){
				state = NUMBER_EXP_SYMBOL
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
				state = NUMBER_EXP_SYMBOL
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
				state = SLASH_COMMENT_CONTENT
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
		default:
			throw "unknow state";
			break;
		}
		if(!check_again){
			if(ch=='\n'){
				pos = 0;
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

int main(){

}