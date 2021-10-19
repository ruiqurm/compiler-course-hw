#include"trie.h"
#include<iostream>
using namespace std;
Trie::Trie(const vector<string>&v){
	for(auto &str:v){
		TrieNode* now = &root;
		for(auto cc:str){
			if(!isalpha(cc))throw;
			char c = tolower(cc);
			if (now->key[c-'a']==nullptr){
				now->key[c-'a'] = new TrieNode();
			}
			now = now->key[c-'a'];

		}
		now->set_final();
	}
}

bool Trie::check(const string& str){
	TrieNode* now = &root;

	for (auto &cc:str){
		if(!isalpha(cc))return false;
		char c = tolower(cc);
		if(now->key[c-'a']!=nullptr){
			cout<<c;
			now = now->key[c-'a'];
		}else{
			return false;
		}
	}
	if(now->is_final)return true;
	else return false;
}

// #include<iostream>
// using namespace std;
// int main(){
// 	const vector<string> keywords {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};

// 	auto t = new Trie(keywords);
// 	cout<<t->check("int");
// 	cout<<t->check("main");
// 	return 0;
// }