#include "../src/simplelex.h"

#include<fstream>
#include<sstream>
#include<algorithm>
#include <windows.h>
using namespace std;

int main(int argc,const char*argv[]){
	SetConsoleOutputCP(65001);
	if(argc!=2) {cout<<"argc=2\n";return -1;}
	string filename(argv[1]);
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
		sort(result->errors.begin(),result->errors.end());
		cout<<"tokens"<<endl;
		for(auto &token:result->tokens){
			cout<<token;
		}
		cout<<"errors"<<endl;
		for(auto &error:result->errors){
			cout<<error;
		}
	}else{
		cout<<"No such file";
		return -1;
	}
	return 0;
}