 //快速排序的实现
#include<iostream>
#include<stack>
using namespace std;

int partition(int *arr, int low, int high)  
{
	int pivo = arr[low]; //以第一个数为基准数
	while (low < high)
	{
		while (low < high && arr[high] >= pivo)
			--high;
		arr[low] = arr[high]; // a[low]已经备份到pivot，不用开辟新空间，直接交换
		while (low < high && arr[low] <= pivo)
			++low;
		arr[high] = arr[low];  // a[low]为找到的大于基准数的数，赋给high
	}						 // 当low == high的时候，交换无影响
	arr[low] = pivo; // 基准数归位，此时low=high，同 a[high] = pivot ，交换pivot与a[high]的值
	cout << *arr;
	return low;
}

// 快速排序 递归
void qsort(int *arr, int low, int high)
{
	if (low < high)
	{
		int pivo = partition(arr, low, high);
		qsort(arr, low, pivo - 1);
		qsort(arr, pivo + 1, high);
	}
}

// 快速排序 非递归，搞不懂
void qsort_no_recursive(int *arr, int low, int high)
{
	stack<int> s;
	int pivo;
	if (low < high)
	{
		pivo = partition(arr, low, high);
		if (low < pivo - 1)
		{
			s.push(low);
			s.push(pivo - 1);
		}
		if (pivo + 1 < high)
		{
			s.push(pivo + 1);
			s.push(high);
		}
		while (!s.empty())
		{
			high = s.top();
			s.pop();
			low = s.top();
			s.pop();
			pivo = partition(arr, low, high);
			if (low < pivo - 1)
			{
				s.push(low);
				s.push(pivo - 1);
			}
			if (pivo + 1 < high)
			{
				s.push(pivo + 1);
				s.push(high);
			}
		}//while
	}//if
}

//简单示例  
int main(void)
{
	int i;
	int a[11] = { 20, 11, 12, 5, 6, 13, 8, 9, 14, 7, 10 };
	//int a[10] = { 6, 1, 2, 7, 9, 3, 4, 5, 10, 8 };
	printf("排序前的数据为：\n");
	for (i = 0; i < 11; ++i)
		printf("%d ", a[i]);
	printf("\n");
	// 递归排序
	// qsort(a, 0,10); 
	//非递归排序
	qsort_no_recursive(a, 0, 10);
	printf("排序后的数据为：\n");
	for (i = 0; i < 11; ++i)
		printf("%d ", a[i]);
	printf("\n");
	return 0;
}



//网易校招第一题
//#include<iostream>
//#include <algorithm>
//#include <string>
//#include <vector>
//using namespace std;
//
//int main()
//{
//	string inputStr;
//	string str_1;
//	string str_2;
//	string str_3;
//	int count = 0;
//	vector<int> resaultVec;
//	cin >> inputStr;
//	string::size_type partition;
//	for (string::size_type i = 0; i < inputStr.size(); i++)
//	{
//		str_1 = inputStr.substr(0, i + 1);
//		str_2 = inputStr.substr(i+1, inputStr.size());
//		if (str_1.size()>=1)
//			reverse(str_1.begin(), str_2.end()); //不能对单个字符进行翻转
//		if (str_2.size()>=1)
//			reverse(str_2.begin(), str_2.end());
//		//string str_12(str_1.rbegin(), str_1.rend());  //以初始化的方式
//		//string str_22(str_2.rbegin(), str_2.rend());
//		str_3 = str_1 + str_2;
//		//如何判断字符串中是黑白相间的字符？count
//		/*for (int j = 0; j < str_3.size(); j++)
//		{
//
//		}*/
//		resaultVec.push_back(count);
//	}
//	return 0;
//}

