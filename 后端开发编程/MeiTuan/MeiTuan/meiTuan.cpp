// 美团第一题
// 图的遍历



// 美团第二题 
// 区间统计
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

int main()
{
	int n, m, k, t;
	int count = 0;
	string str;
	string strSub;
	map<int, int> strMap;
	cin >> n >> k >> t;
	for (int i = 0; i < n; i++)
	{
		cin >> m;
		str.push_back(m);
	}
	int l = 1;
	int r = l + k - 1;
	for (l = 1; r < str.size(); l++)
	{
		strSub = str.substr(l, r);
		for (int j = 0; j < strSub.size(); j++)
		{
			strMap[strSub[j]]++;
		}
		strSub.erase(unique(strSub.begin(), strSub.end()), strSub.end());
		for (int i = 0; i < strSub.size(); i++)
		{
			if (strMap[strSub[i]] == t)
			{
				count++;
			}
		}
	}
	cout << count;
	return 0;
}



// 美团第二题
// 区间统计
#include<iostream>
#include<vector>
#include<map>
using namespace std;

int findqujian(int n, int k, int t, vector<int> v)
{
	map<int, int> nums;
	vector<vector<int> > tmp;
	int count = 0;
	for (int j = 0; j<v.size() - k + 1; j++)
	{
		vector<int> result(v.begin() + j, v.begin() + j + k);// vector初始化方式，取其它容器中的元素来复制
		tmp.push_back(result);
		result.clear();
	}
	for (int k = 0; k < tmp.size(); k++)
	{
		for (auto m = tmp[k].cbegin(); m < tmp[k].cend(); ++m)
		{
			++nums[*m];  //对每个区间内部的元素重复次数进行统计
		}
		for (map<int, int>::iterator ltr = nums.begin(); ltr != nums.end(); ltr++)
		{
			if (ltr->second >= t)
			{
				count++;
			}
		}
		nums.clear(); //清空，统计下一个区间
	}
	return count;
}

int main()
{
	int n, k, t;
	vector<int> v;  // 输入的字符串
	cin >> n >> k >> t;
	for (int i = 0; i < n; i++)
	{
		int x;
		cin >> x;
		v.push_back(x);
	}
	//int n=5, k=3, t=2;
	//vector<int> v = {3,1,1,1,2};
	int num = findqujian(n, k, t, v);
	cout << num;
	return 0;
}

