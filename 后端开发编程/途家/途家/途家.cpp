
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
			if (j >= v[i - 1]) //如果和数大于新行中v[i-1]的值，
							// 那么新的方法数等于上一行的方法数加上有了新的值v[i-1]后新的方法数
							// 等于上一行中，j-v[i-1]列的方法数之和
				res[i][j] = res[i - 1][j] + res[i - 1][j - v[i - 1]];  
			else   //当和数小于V[i-1]表明该行增加的数无法构成新的方法，方法数与上一层上等
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