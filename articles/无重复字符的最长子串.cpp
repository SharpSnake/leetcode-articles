/*		3. 无重复字符的最长子串
	给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
	
	示例 1:
	
	输入: s = "abcabcbb"
	输出: 3 
	解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
	示例 2:
	
	输入: s = "bbbbb"
	输出: 1
	解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
	示例 3:
	
	输入: s = "pwwkew"
	输出: 3
	解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
	     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
	示例 4:
	
	输入: s = ""
	输出: 0

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/longest-substring-without-repeating-characters
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 将char转为int（0~255之间）
#define C2INT( c ) ( int )( ( uint8_t )c )

// 窗口类，记录起始和尾后位置，可以比大小
struct Window
{
	int				Begin{};
	int				End{};

	constexpr auto Width()	const noexcept	{ return End - Begin; }

	constexpr bool operator>( const Window &right )	const noexcept	{
		return this->Width() > right.Width();
	}
};

class Solution {
public:
	int lengthOfLongestSubstring( string s )
	{
		if( s.empty() )
			return 0;

		auto cs = s.data();
		int sz = s.size();
		Window wnd{}, maxwnd{};

		vector< int > hmap( 256, -1 );	// 简化的hash map，记录某个字符出现的位置
		hmap[ C2INT( cs[ 0 ] ) ] = 0;	// 首个字符的位置是0

		while( ++wnd.End < sz )
		{
			auto key = C2INT( cs[ wnd.End ] );
			if( auto pos = hmap[ key ]; pos != -1 )	// 当前字符已存在，将窗口移到pos的下个位置
			{
				for( int i = wnd.Begin; i <= pos; ++i )	// 重置pos之前所有字符的hash map
					hmap[ C2INT( cs[ i ] ) ] = -1;

				if( wnd > maxwnd )
					maxwnd = wnd;
				wnd.Begin = ++pos;
			}

			hmap[ key ] = wnd.End;
		}

		return wnd > maxwnd ? wnd.Width() : maxwnd.Width();
	}
};

/*
解题思路
	Sliding window
*/
int main3()
{
	Solution sln{};
	cout << sln.lengthOfLongestSubstring( "abcabcbb" ) << endl;
	cout << sln.lengthOfLongestSubstring( "pwwkew" ) << endl;
	return 0;
}