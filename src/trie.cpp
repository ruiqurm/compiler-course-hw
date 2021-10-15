#include"trie.h"

Trie::Trie(const vector<string>&v){
	for(auto str&:v){
		TrieNode* now = &root;
		for(auto c:str){
			if(c<96)c+=32;
			if (now->key[c-'a']==nullptr){
				now->key[c-'a'] = new TrieNode();
				now = now->key[c-'a'];
			}
		}
	}
}