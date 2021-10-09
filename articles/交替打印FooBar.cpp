/*		1115. 交替打印FooBar
	我们提供一个类：
	
	class FooBar {
	  public void foo() {
	    for (int i = 0; i < n; i++) {
	      print("foo");
	    }
	  }
	
	  public void bar() {
	    for (int i = 0; i < n; i++) {
	      print("bar");
	    }
	  }
	}
	两个不同的线程将会共用一个 FooBar 实例。其中一个线程将会调用 foo() 方法，另一个线程将会调用 bar() 方法。
	
	请设计修改程序，以确保 "foobar" 被输出 n 次。
	
	 
	
	示例 1:
	
	输入: n = 1
	输出: "foobar"
	解释: 这里有两个线程被异步启动。其中一个调用 foo() 方法, 另一个调用 bar() 方法，"foobar" 将被输出一次。
	示例 2:
	
	输入: n = 2
	输出: "foobarfoobar"
	解释: "foobar" 将被输出两次。

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/print-foobar-alternately
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


class FooBar {
private:
	int n;

	bool					m_Flag{};	// 初始化为false
	mutex					m_Mutex;
	condition_variable		m_CV;

public:
	FooBar(int n) {
		this->n = n;
	}

	void foo(function<void()> printFoo) {

		for (int i = 0; i < n; i++)
		{
			unique_lock< mutex > lock( m_Mutex );
			m_CV.wait( lock, [ this ]{ return !m_Flag; } );

			printFoo();

			m_Flag = true;
			m_CV.notify_one();
		}
	}

	void bar(function<void()> printBar) {

		for (int i = 0; i < n; i++)
		{
			unique_lock< mutex > lock( m_Mutex );
			m_CV.wait( lock, [ this ]{ return m_Flag; } );

			printBar();

			m_Flag = false;
			m_CV.notify_one();
		}
	}
};

/*
解题思路
	就是利用condition_variable::wait的断言功能，维护一个bool标记，来回切换标记即可实现线程交替。
	使用屏障（std::barrier）估计也能实现相同的功能。
*/
int main1115()
{
	FooBar foobar{ 10 };

	thread t1( [ & ]{ foobar.foo( []{ cout << "foo"; } ); } );
	thread t2( [ & ]{ foobar.bar( []{ cout << "bar" << endl; } ); } );
	t1.join();
	t2.join();

	return 0;
}