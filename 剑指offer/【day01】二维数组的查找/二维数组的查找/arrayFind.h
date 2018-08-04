#include <iostream>
#include <vector>
using namespace std;

/*
// ѭ������ʵ�֣��ȽϺ�ʱ��
// ѧϰ�˻���vector��ʽ�Ķ�ά���鴴������ң���ά�����һЩ����
// vector���±����
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
					cout << "�ҵ���" << "�ڵ�" << "(" << i << "," << j << ")" << endl;
					return true;
				}
			}
		}
		cout << "û�ҵ�!!!" << endl;
		return false;
	}
};  // ��������������ʱ��12ms��1384kb
*/

// ��С�����ռ䡣�ǳ���˼·
// �ǳ���˼·�������Ͻ�Ѱ��
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
			cout << "��ǰԪ��Ϊ��"<< array[row][col] << endl;

			if ( array[row][col] == target)
			{
				 cout << "���������ҵ�����Ҫ�ҵ�Ԫ��Ϊ��"<< target << endl;
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
};// ��������������ʱ��11ms��1496kb