#include"trie.h"

Trie::Trie(const vector<string>&v){
	for(auto &str:v){
		TrieNode* now = &root;
		for(auto cc:str){
			if(!isalpha(cc))throw;
			char c = tolower(cc);
			if (now->key[c-'a']==nullptr){
				now->key[c-'a'] = new TrieNode();
				now = now->key[c-'a'];
			}
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
// 	auto t = new Trie({"abcd"});
// 	cout<<t->check("abcd");
// 	cout<<t->check("abc");
// 	return 0;
// }