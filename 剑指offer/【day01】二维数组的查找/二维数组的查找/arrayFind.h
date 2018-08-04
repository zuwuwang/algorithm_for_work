#include <iostream>
#include <vector>
using namespace std;

/*
// 循环遍历实现，比较耗时间
// 学习了基于vector方式的二维数组创建与查找，二维数组的一些方法
// vector的下标操作
class Solution
{
public:
	bool Find(int target, vector<vector<int> > array)
	{
		if (array.empty())
			return false;
		int rows = array.size();
		int cols = array[0].size();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (target == array[i][j])
				{
					cout << "找到啦" << "在第" << "(" << i << "," << j << ")" << endl;
					return true;
				}
			}
		}
		cout << "没找到!!!" << endl;
		return false;
	}
};  // 测试样例测试用时：12ms，1384kb
*/

// 缩小搜索空间。非常规思路
// 非常规思路，从右上角寻找
class Solution {
public:
	bool Find(int target, vector<vector<int> > array) 
	{
		int rows = array.size();
		int cols = array[0].size();
		int row = 0 ;
		int col = cols -1;
		while ( row < rows && col >= 0)
		{
			cout << "当前元素为："<< array[row][col] << endl;

			if ( array[row][col] == target)
			{
				 cout << "哈哈，我找到啦！要找的元素为："<< target << endl;
				 return true;
			}
			else if (array[row][col] > target)
			{
				col --;
			}
			else
				row ++;
		}
		return false;
	}
};// 测试样例测试用时：11ms，1496kb