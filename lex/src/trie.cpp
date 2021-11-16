#include"trie.h"
Trie::Trie(const map<string,int>&v){
	for(auto &p:v){
		TrieNode* now = &root;
		auto &str = p.first;
		if(p.second<0)continue;
		for(auto cc:str){
			if(!isalpha(cc))throw;
			char c = tolower(cc);
			if (now->key[c-'a']==nullptr){
				now->key[c-'a'] = new TrieNode();
			}
			now = now->key[c-'a'];

		}
		now->type = p.second;
		now->set_final();
	}
}

int Trie::check(const string& str){
	TrieNode* now = &root;

	for (auto &cc:str){
		if(!isalpha(cc))return -1;
		char c = tolower(cc);
		if(now->key[c-'a']!=nullptr){
			now = now->key[c-'a'];
		}else{
			return -1;
		}
	}
	if(now->is_final)return now->type;
	else return -1;
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