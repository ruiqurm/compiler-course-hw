#include "../src/simplelex.h"

#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;

int main(){
	auto filename = "F:/program/clex/build/test/test_data/literal.c";
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