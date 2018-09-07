#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool HuiWen(string str)
{
	int length = str.length();
	//int mid = length / 2 - 1;当两边的哨兵到一起时或者后面哨兵下标比前面哨兵下标大，则比较结束
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
	// insert元素后，迭代器位置会更新，这里事先知道所插入的字符串的长度，定义循环终止位置
	// 如何更新迭代器？使用insert或者erase返回的迭代器进行操作，判断条件中重新获取迭代器值 v.end或其他。
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

// 首尾插入 删除 查看元素