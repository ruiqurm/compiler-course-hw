#include<vector>
#include<string>
#include<cctype>
using std::vector;
using std::string;
constexpr int KEY_NUM=26;
class Trie;
class TrieNode{
	public:
	TrieNode(){
		for(auto i=0;i<KEY_NUM;i++)key[i] = nullptr;
		is_final = false;
	}
	void set_final(){is_final=true;}
	void unset_final(){is_final=false;}
	bool check_is_final()const{return is_final;}
	private:
		friend Trie;
		TrieNode* key[KEY_NUM];
		bool is_final;
};
class Trie{
	public:
		Trie(const vector<string>&v);
		bool check(const string& str);
	private:
		TrieNode root;
};

