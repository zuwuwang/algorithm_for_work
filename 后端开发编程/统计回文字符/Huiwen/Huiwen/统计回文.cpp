#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool HuiWen(string str)
{
	int length = str.length();
	//int mid = length / 2 - 1;�����ߵ��ڱ���һ��ʱ���ߺ����ڱ��±��ǰ���ڱ��±����ȽϽ���
	for (int i = 0; i < length; i++)
	{
		if (str[i] != str[length -1])
			return false;
		length--;
	}
	return true;
	
}

int main()
{
	string A;
	string B;
	string tmp;
	cin >> A;
	cin >> B;
	int count = 0;
	tmp = A;
	// insertԪ�غ󣬵�����λ�û���£���������֪����������ַ����ĳ��ȣ�����ѭ����ֹλ��
	// ��θ��µ�������ʹ��insert����erase���صĵ��������в������ж����������»�ȡ������ֵ v.end��������
	int length = A.length()+1;  
	for (int i = 0; i<length; i++)
	{
		A = tmp;
 		A.insert(i, B);
	    //cout << A << endl;
		if (HuiWen(A))
			count++;
		//cout << count << endl;
	}
	cout << count << endl;
	 system("pause");
	return 0;
}

// ��β���� ɾ�� �鿴Ԫ��