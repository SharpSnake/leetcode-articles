/*		1116. 打印零与奇偶数
	假设有这么一个类：
	
	class ZeroEvenOdd {
	  public ZeroEvenOdd(int n) { ... }      // 构造函数
	  public void zero(printNumber) { ... }  // 仅打印出 0
	  public void even(printNumber) { ... }  // 仅打印出 偶数
	  public void odd(printNumber) { ... }   // 仅打印出 奇数
	}
	相同的一个 ZeroEvenOdd 类实例将会传递给三个不同的线程：
	
	线程 A 将调用 zero()，它只输出 0 。
	线程 B 将调用 even()，它只输出偶数。
	线程 C 将调用 odd()，它只输出奇数。
	每个线程都有一个 printNumber 方法来输出一个整数。请修改给出的代码以输出整数序列 010203040506... ，其中序列的长度必须为 2n。
	
	 
	
	示例 1：
	
	输入：n = 2
	输出："0102"
	说明：三条线程异步执行，其中一个调用 zero()，另一个线程调用 even()，最后一个线程调用odd()。正确的输出为 "0102"。
	示例 2：
	
	输入：n = 5
	输出："0102030405"

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/print-zero-even-odd
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


class ZeroEvenOdd {
private:
	int n;

	int					m_Flag{};	// initialize to zero
	mutex				m_Mutex;
	condition_variable	m_CV;

public:
	ZeroEvenOdd(int n) {
		this->n = n;
	}

	void zero(function<void(int)> printNumber) {
		for( int i = 1; i <= n; ++i )
		{
			unique_lock< mutex > locker( m_Mutex );
			m_CV.wait( locker, [ this ]{ return m_Flag == 0; } );

			printNumber( 0 );

			m_Flag = ( i & 1 ) ? 1 : 2;	// 奇数则唤醒odd，否则唤醒even
			m_CV.notify_all();
		}
	}

	void even(function<void(int)> printNumber) {
		for( int i = 2; i <= n; i += 2 )
		{
			unique_lock< mutex > locker( m_Mutex );
			m_CV.wait( locker, [ this ]{ return m_Flag == 2; } );

			printNumber( i );

			m_Flag = 0;
			m_CV.notify_all();
		}
	}

	void odd(function<void(int)> printNumber) {
		for( int i = 1; i <= n; i += 2 )
		{
			unique_lock< mutex > locker( m_Mutex );
			m_CV.wait( locker, [ this ]{ return m_Flag == 1; } );

			printNumber( i );

			m_Flag = 0;
			m_CV.notify_all();
		}
	}
};

/*
解题思路
	思路和“交替打印foobar”那道题基本一致，仍然是利用条件变量的等待、唤醒功能，只是flag标记变成了三态。
	另外多说一句：一些实现用了类似自旋锁“死等”的方法，这种一般会增加cpu占用率。而且就算是“死等”，
	最好也不要用std::this_thread::yield，应该使用各平台（编译器）提供的cpu relax方法（指令），如msvc的_mm_pause。
*/
int main1116()
{
	ZeroEvenOdd zeo{ 9 };
	auto print = []( int i ){ cout << i; };

	thread t1( [ & ]{ zeo.zero( print ); } );
	thread t2( [ & ]{ zeo.even( print ); } );
	thread t3( [ & ]{ zeo.odd( print ); } );

	t1.join();
	t2.join();
	t3.join();

	cout << endl;	// flush

	return 0;
}