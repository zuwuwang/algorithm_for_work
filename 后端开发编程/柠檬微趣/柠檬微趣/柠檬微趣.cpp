// 20180907 柠檬校招 第二题 AC 90【王祖武】
// 求相同元素之间的距离
//#include <iostream>
//#include <algorithm>
//#include <map>
//#include <vector>
//#include <string>
//
//using namespace std;
//
//int main()
//{
//	long long len = 0, n = 0;
//	cin >> len;
//	if (len <= 0)
//		return 0;	
//	string arrStr;
//	vector<long long> resaultVec;
//	map<long long, long long> COUNT;
//	for (long long i = 0; i < len; i++)
//	{
//		cin >> n;
//		arrStr.push_back(n);
//		COUNT[n]++;
//	}
//	string::size_type first = 0;
//	string::size_type last = 0;
//	long long distance = 0;
//	for (map<long long, long long>::iterator itor = COUNT.begin(); itor != COUNT.end(); itor++)
//	{
//		if (itor->second >= 2)
//		{
//			long long arg = itor->first;
//			first = arrStr.find(arg);
//			last = arrStr.rfind(arg);
//			distance = last - first;
//			resaultVec.push_back(distance);
//		}	
//	}
//	if (!resaultVec.empty())
//	{
//		sort(resaultVec.begin(), resaultVec.end());
//		cout << resaultVec[resaultVec.size() - 1];
//	}
//	else
//		cout << 0;
//	return 0;
//}

// 柠檬第三题   AC 91%
//#define _CRT_SECURE_NO_DEPRECATE
//#include<iostream>
//#include<stdlib.h>
//#include<string>
//using namespace std;
//double change(char *input)
//{
//	int i = 0, count1 = 1;  //i作为step 累加
//	double input_int1 = 0, input_xs1 = 0, input_1 = 0;
//	while (*(input + i) != '\0')
//	{
//		while (*(input + i) >= '0'&&*(input + i) <= '9')   // 取出整数部分
//		{
//			input_int1 = input_int1 * 10 + *(input + i) - '0';
//			//i++;
//			input++;  //地址加一和直接加i作用一样的
//		}
//		i++;  // 跳过小数点
//		while (*(input + i) >= '0'&&*(input + i) <= '9')
//		{
//			input_xs1 = input_xs1 * 10 + *(input + i) - '0';  // 取出小数部分
//			count1 *= 10;
//			//i++;
//			input++;
//		}
//	}
//	input_1 = input_int1 + input_xs1 / count1;
//	return input_1;
//}
//int main()
//{
//	char *input;
//	double sum = 0;
//	input = (char*)malloc(10);
//	cout << "输入字符串数:";
//	gets(input);
//	sum = change(input);
//	cout << "转化成浮点数:" << sum << endl;
//	system("pause");
//	return 0;
//}


// 柠檬校招笔试第一题 AC 1
//#include <iostream>
//#include <queue>
//using namespace std;
//
//struct Node
//{
//	Node *pLeftChild;
//	Node *pRightChild;
//	Node *pParent;
//};
//
//int getHeight(Node* pTree)
//{
//	if (pTree == NULL)
//		return 0;
//	int lheight = getHeight(pTree->pLeftChild);
//	int rheight = getHeight(pTree->pRightChild);
//	return max(lheight, rheight) + 1;
//}
//
//Node* initTree()
//{
//	int n = 0;
//	cin >> n;
//	if (n == 0)
//	{
//		return nullptr;
//	}
//	Node* p = new Node();
//	p->pLeftChild = initTree();
//	if (p->pLeftChild)
//	{
//		p->pLeftChild->pParent = p;
//	}
//	p->pRightChild = initTree();
//	if (p->pRightChild)
//	{
//		p->pRightChild->pParent = p;
//	}
//	return p;
//}
//
//int main()
//{
//	int p = 0;
//	Node* pTree = initTree();
//	p = getHeight(pTree);
//	cout << p << endl;
//	return 0;
//}
//
//
//

//// 我的柠檬 AC 1.9
// 柠檬有一道题是重复题 90% 
// 第三题 不适用sort排序 尽可能快
//// 使用快速排序，导致AC 80%，堆栈溢出了，调用太多递归  解决办法--> 数组a[100]空间太小，分配为a[1000000]-->AC
//#include <iostream>
//using namespace std;
//
//int partition(int* arr,int low,int high)
//{
//	int pivot = arr[low];
//	while (low < high)
//	{
//		while (low < high&&arr[high] >= pivot)
//			--high;
//		arr[low] = arr[high];
//		while (low < high&&arr[low] <= pivot)
//			++low;
//		arr[high] = arr[low];
//	}
//	arr[low] = pivot;
//	return low;
//}
//
//void qsort_recursive(int* arr,int low,int high)
//{
//	if (low < high)
//	{
//		int pivot = partition(arr,low,high);
//		qsort_recursive(arr, low, pivot - 1);
//		qsort_recursive(arr, pivot + 1, high);
//	}
//}
//
//int main()
//{
//	int len = 0;
//	int num = 0;
//	int a[100];
//	cin >> len;
//	int i = 0;
//	for ( i= 0; i < len; i++)
//	{
//		cin >> num;
//		a[i] = num;
//	}
//	//printf("排序前的数据为：\n");
//	//for (i = 0; i < len; ++i)
//	//     printf("%d ", a[i]);
//	//printf("\n");
//	qsort_recursive(a, 0, len - 1);
//	//printf("排序后的数据为：\n");
//	for (i = 0; i < len; ++i)
//		printf("%d ", a[i]);
//	printf("\n");
//	return 0;
//}


//#include <iostream>
//#include <string>
//using namespace std;
//
//struct Node
//{
//	int nData;
//	Node *pNext;
//};
//
//typedef Node* pNode;
//
//Node* CreateList(string str,int len)
//{
//	pNode pHead = (pNode)malloc(sizeof(Node));       
//	pNode pTail = pHead;                            
//	pTail->pNext = NULL;                           
//	for (int i = 0; i < len; i++)
//	{
//		pNode pNew = (pNode)malloc(sizeof(Node));    
//		pNew->nData = str[i] - '0';
//		pTail->pNext = pNew;                        
//		pNew->pNext = NULL;                            
//		pTail = pNew;                                 
//	}
//	return pHead;
//}
//
//Node* findNkthNode(Node* pList, int k)
//{
//
//	return NULL;
//}
//int main()
//{
//	int k;
//	string str;
//	string arr;
//	getline(cin, str, '0');
//	for (int i = 0; i < str.size(); i ++)
//	{
//		if ( i%2 ==0 && i < str.size())
//		{
//			arr.push_back(str[i]);
//		}
//	}
//	cin >> k;
//	Node* pHead = NULL; 
//	pHead = CreateList(arr,arr.size());
//	cout << findNkthNode(pHead, k);
//	return 0;
//}


// 【途家 求两个数的最大公约数和最小公倍数】
//#include <iostream>
//using namespace std;
//
//// 求最大公约数
//// 碾转相除法
//// 找出max min
//// max%min 除不尽的话，min除以余数，循环迭代
//// 
//int MaxY(int a, int b)
//{
//	int min, max;
//	max = a>b ? a : b;
//	min = a<b ? a : b;
//	if (max%min == 0)
//		return min;
//	else
//		return MaxY(min, max%min); //递归的实现
//}
//
////求最小公倍数。最小公倍数等于两数之积除以最大公约数
//// max %min
//// 
//int MinY(int a, int b)
//{
//	int min, max;
//	int r;
//	max = a>b ? a : b;
//	min = a<b ? a : b;
//	if (max%min == 0)  //如果可以直接除尽，那么返回最大值作为最小公倍数
//		return max;
//	while (max%min != 0)
//	{
//		r = max%min;
//		max = min;
//		min = r;
//	}
//	return a*b / min;
//}
//int main()
//{
//	int a, b;
//	cin >> a >> b;
//	cout << MaxY(a, b) << endl;
//	cout << MinY(a, b);
//	return 0;
//}


//【途家第二题  数组中元素和为sum的方法数】
// 我的答案，不对
//#include <iostream>
//#include <vector>
//using namespace std;
//
//int find(vector<int> arr, int sum)
//{
//	int a = 0;
//	int count = 0;
//	for (int i = 0; i < arr.size(); i++)
//	{
//		for (int j = i; j < arr.size(); j++)
//		{
//			if (arr[i] == sum)
//				count++;
//			if (arr[i] + arr[j] == sum)
//				count++;
//		}
//	}
//	return count;
//}
//
//int main()
//{
//	int len, sum, num;
//	vector<int> arr;
//	cin >> len >> sum;
//	for (int i = 0; i < len; i++)
//	{
//		cin >> num;
//		arr.push_back(num);
//	}
//	cout << find(arr, sum);
//
//	return 0;
//}

//  【百度答案，看会】
#include <iostream>
using namespace std;
#include <vector>

int main()
{
	int n, sum;
	cin >> n >> sum;
	vector<long> v(n);
	for (int i = 0; i < n; i++)
	{
		cin >> v[i];
	}
	vector<long> s(sum + 1, 0);
	s[0] = 1;
	vector<vector<long> > res(n + 1, s);

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= sum; j++)
		{
			if (j >= v[i - 1])
				res[i][j] = res[i - 1][j] + res[i - 1][j - v[i - 1]];
			else
				res[i][j] = res[i - 1][j];
		}
	}
	cout << res[n][sum] << endl;
	return 0;
}

//第三题 拿小石子比赛
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//int main()
//{
//	int T, n, k;
//	vector<int> line;
//	vector<vector<int>> arr;
//	cin >> T;
//	
//	for (int i = 0; i < T; i++)
//	{
//		cin >> n >> k;
//		line.push_back(n);
//		line.push_back(k);
//		arr.push_back(line);
//	}
//	for (int j = 0; j < arr.size(); j++)
//	{
//		if ((arr[j][0] / arr[j][0])!=1)
//			cout << 'A' << endl;
//		else
//			cout << 'B' << endl;
//		
//	}
//
//	return 0;
//}