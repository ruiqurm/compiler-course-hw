#include<vector>
#include<string>

using std::vector;
using std::string;
template<int KEY_NUM>
class TrieNode{
	// TrieNode(char c):letter(c){}
	// char letter;
	TrieNode(){
		for(auto i=0;i<KEY_NUM;i++)key[i] = nullptr;
	}
	TrieNode* key[KEY_NUM];
};
template<int KEY_NUM>
class Trie{
	public:
		Trie(const vector<string>&v)explicit;
		bool compare(const string& str);
		TrieNode<KEY_NUM> root;
};
