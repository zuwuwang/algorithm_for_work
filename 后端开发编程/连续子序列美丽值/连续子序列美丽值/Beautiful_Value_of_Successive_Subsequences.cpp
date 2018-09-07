// 删除容器中重复元素，先排序，重复的元素放到后面，然后再erase擦除
// sort(v.begin(), v.end());
// v.erase(unique(v.begin(), v.end()), v.end());
// https://blog.csdn.net/HE19930303/article/details/50579996

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// vector_son存放所有子串
// 剔除子串的相同元素 ：先sort排序，相同的元素被排在后面，再用 erase方法，对unique方法排序后的相同容器元素进行擦除
// 子串元素求和 accumulate方法
// /unique()函数将重复的元素放到vector的尾部 然后返回指向第一个重复元素的迭代器
// 再用erase函数擦除从这个元素到最后元素的所有的元素
vector<vector<int> > removalSameElement(vector<vector<int> >& vector_son)
{
	for (int i = 0; i < vector_son.size(); i++)
	{
		//sort(vector_son[i].begin(), vector_son[i].end());
		vector_son[i].erase(unique(vector_son[i].begin(), vector_son[i].end()), vector_son[i].end());
	}
	return vector_son;
}

int vector_sum(vector<vector<int> >& vector_son)
{
	int sum = 0;
	vector<int> num_sum;
	for (int i = 0; i < vector_son.size(); i++)
	{
		num_sum.push_back(accumulate(vector_son[i].begin(), vector_son[i].end(), 0));
	}
	sum = accumulate(num_sum.begin(), num_sum.end(), 0);
	return sum;
}


int beauty_of_array(int array[])
{
	vector<vector<int> > vector_son;
	vector<int> tmp;
	cout << sizeof(array) << '\t' << sizeof(array[0]) << endl;
	int len = 3; //sizeof(array) / sizeof(array[0]);
	// 求子串
	for (int i = 0; i < len; i++)
	{
		for (int j = i; j < len; j++)
		{
			int m = i;
			while (m <= j)
			{
				tmp.push_back(array[m]);
				++m;
			}
			vector_son.push_back(tmp);  //对每个首地址开始处求得一个子串
			tmp.clear();
		}
	}
	removalSameElement(vector_son);
	int sum = vector_sum(vector_son);
	return sum;
}

int main()
{
	int array[] = { 1, 2, 3 };
	// sizeof(*array)  sizeof(array[0]) sizeof(int)三种表示方法都表示array的第每个元素所占空间大小
	cout << sizeof(array) << '\t' << sizeof(*array) << endl;  
	for (int i = 0; i < sizeof(array) / sizeof(*array); i++)
	{
		cout << array[i] << ' ';
	}
	cout << endl;
	int beauty_of_sum = 0;
	beauty_of_sum = beauty_of_array(array);
	cout << beauty_of_sum << endl;
	system("pause");
	return 0;
}