#include "stdafx.h"  //预编译头的头文件位置要放对
#include <iostream>
#include <Windows.h>
using namespace std;
//本体关键在于求序列分解求和 aX+bY==K，确定a,b的值。
// 我感觉可以设a，b，sum表示总的可能数，两个for循环，0<=a <=A ，0<=b<=B，判断条件 ：aX+bY==K  。
//写出一个求阶乘的函数计算组合数，对于每一种a,b的值，sum = CaA+CbB，  sum+=sum
//或者使用“滑窗”的思想，从AB相邻的位置开始滑窗，初始滑窗包括一个A一个B，判断其和与K的关系。
//当aX+bY==K时，再判断出a,b的值

int fact(int n)
{
	if (n == 1|| n==0)
		return 1;
	if (n > 1)
		return n*fact(n - 1);
}

int main()
{
	int K;
	int X, A, Y, B;
	int resault = 0;
	int x, y;
	cin >> K;
	cin >> A  >> X >> Y >> B;
	//cout << k << a << x << y << b << endl;
	for (x = 0; x <= X; x++)
	{
		for (y = 0; y <= Y; y++)
		{
			if (x*A + y*B == K)
				resault = resault + (fact(X) / (fact(x)*fact(X - x)))*(fact(Y) / (fact(y)*fact(Y - y)));
		}
	}
	cout << resault % 1000000007 << endl;
	system("pause");
	return resault;
}
