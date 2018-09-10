// 
//#include <iostream>
//#include <string>
//using namespace std;
//class A{
//public:
//	A(long a)
//	{
//		x = a++;
//	}
//public:
//	long x;
//};
//
//long calculate(A s)
//{
//	if (s.x < 2)
//	{
//		return 1;
//	}
//	return s.x + calculate(A(s.x - 1));
//}
//
//int main()
//{
//
//	int sum = 0;
//	for (int i = 0; i < 5; i++)
//	{
//		sum += calculate(A(i));
//	}
//	cout << sum;
//}


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;


//二分查找 数组版，非递归
//找到返回值，未找到返回error
//考虑指针的实现?定义两个指针哨兵
//考虑递归的实现
int bFind(char* arr, char ch)
{
	int low = 0; //定义两个哨兵
	int high = strlen(arr);
	while (low < high)  // 二分循环条件
	{
		int mid = (low + high) / 2;
		if (ch < arr[mid])
			high = mid;
		else if (ch>arr[mid])
			low = mid + 1;
		else
			return mid;
	}
	return NULL;
}

int main()
{
	char ch[100];
	cin.getline(ch, 100);  //会将空格也作为输入，可行
	char arr[100];
	char value;
	//cin >> str;  //cin会过滤掉空格 tab结束符，不可行
	int len = strlen(ch);
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (ch[i] != ','&&ch[i] != ' ')
		{
			arr[j] = (ch[i] - '0');
			j++;
		}	
	}
	arr[j] = '\0';
	int l = strlen(arr);
	value = arr[strlen(arr)-1];
	arr[strlen(arr)-1] = '\0';
	int  resault = bFind(arr, 4);
	printf("%d\n", resault);
	printf("%d\n", arr[resault]);
	return 0;
}



// 小赢科技笔试题【第一题】
//#include<iostream>
//#include<string>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//int find_2_max(vector<int>& num, int th)
//{
//	int maxvalue = num[0];;
//	int len = num.size();
//	if (num[len / 2] == th)
//	{
//		return num[(len / 2) - 1];
//	}
//	for (int i = 0; i < len; i++)
//	{
//		if (num[i] < th)
//		{
//			if (maxvalue < num[i])
//				maxvalue = num[i];
//		}
//	}
//	return maxvalue;
//}
//
//int toInt(const string& str)
//{
//	return atoi(str.c_str());
//}
//
//int main()
//{
//	string str;
//	char num[1000];
//	cin.getline(num, 1000);
//	int len = strlen(num);
//	for (int i = 0; i < len; i++)
//	{
//		str.push_back(num[i]);
//	}
//	str.erase(remove(str.begin(), str.end(), ','), str.end());
//	str.erase(remove(str.begin(), str.end(), ' '), str.end());
//
//	vector<int> v;
//	//transform(str[0], str[str.size()-1], v.begin(), toInt);
//	for (int j = 0; j < str.size(); j++)
//	{
//		int tmp = str[j] - '0';
//		v.push_back(tmp);
//	}
//	int th = v[v.size() - 1];
//	v.pop_back();
//	int max = find_2_max(v, th);
//	cout << max << endl;
//	system("pause");
//	return 0;
//}
