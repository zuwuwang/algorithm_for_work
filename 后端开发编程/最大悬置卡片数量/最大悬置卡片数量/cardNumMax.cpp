#include <iostream>
#include <limits.h>
using namespace std;

int main()
{
	long long n;
	float length;
	cin >> length;
	float sum = 0;
	for (n = 1; n < INT_MAX; n++)
	{
		sum += (1 /(double)(n + 1));
		if (length <= sum)
			break;
	}
	cout << n << endl;
	system("pause");
	return 0;
}
//��дһ�����غ�����ʵ�ַ����ļӷ�
//���߽�����ת����ΪС��
