/*		剑指 Offer II 033. 变位词组
	定一个字符串数组 strs ，将 变位词 组合在一起。 可以按任意顺序返回结果列表。
	
	意：若两个字符串中每个字符出现的次数都相同，则称它们互为变位词。
	
	
	
	示例 1:
	
	输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
	输出: [["bat"],["nat","tan"],["ate","eat","tea"]]
	示例 2:
	
	输入: strs = [""]
	输出: [[""]]
	示例 3:
	
	输入: strs = ["a"]
	输出: [["a"]]
	
来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/sfvd7V
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;


class Solution
{
public:
	vector< vector< string > > groupAnagrams( const vector< string > &strs )
	{
		unordered_map< string, vector< string > > smap;

		for( auto &str : strs )
		{
			auto c = str;
			sort( c.begin(), c.end() );

			if( auto iter = smap.find( c ); iter != smap.end() )
				iter->second.emplace_back( str );
			else
				smap.emplace( std::move( c ), vector< string >{ str } );
		}

		vector< vector< string > > groups{};
		groups.reserve( smap.size() );
		for( auto&[ k, group ] : smap )
			groups.emplace_back( std::move( group ) );

		return groups;
	}
};

/*
解题思路
	用哈希表来组织最终的分组是总思路，关键是如何计算每个字符串的哈希值？
	将每个字符串当做字符数组进行排序当做哈希，这是脑袋里冒出的第一个也是最容易的一个方法。
	但可能还有更巧妙、性能更好的方法有待开发。。。
	
	看了几个实现，这个思路的代码基本大同小异，我这个实现就是从语言层面细扣优化了一些。
*/
int main033()
{
	auto groups = Solution{}.groupAnagrams( { "eat", "tea", "tan", "ate", "nat", "bat" } );
	for( auto &g : groups )
	{
		for( auto &s : g )
			cout << s << ", ";
		cout << endl;
	}

	return 0;
}