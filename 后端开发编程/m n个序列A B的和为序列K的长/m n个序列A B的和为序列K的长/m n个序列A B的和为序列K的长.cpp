#include "stdafx.h"  //Ԥ����ͷ��ͷ�ļ�λ��Ҫ�Ŷ�
#include <iostream>
#include <Windows.h>
using namespace std;
//����ؼ����������зֽ���� aX+bY==K��ȷ��a,b��ֵ��
// �Ҹо�������a��b��sum��ʾ�ܵĿ�����������forѭ����0<=a <=A ��0<=b<=B���ж����� ��aX+bY==K  ��
//д��һ����׳˵ĺ������������������ÿһ��a,b��ֵ��sum = CaA+CbB��  sum+=sum
//����ʹ�á���������˼�룬��AB���ڵ�λ�ÿ�ʼ��������ʼ��������һ��Aһ��B���ж������K�Ĺ�ϵ��
//��aX+bY==Kʱ�����жϳ�a,b��ֵ

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
