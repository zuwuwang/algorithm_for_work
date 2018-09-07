// ɾ���������ظ�Ԫ�أ��������ظ���Ԫ�طŵ����棬Ȼ����erase����
// sort(v.begin(), v.end());
// v.erase(unique(v.begin(), v.end()), v.end());
// https://blog.csdn.net/HE19930303/article/details/50579996

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// vector_son��������Ӵ�
// �޳��Ӵ�����ͬԪ�� ����sort������ͬ��Ԫ�ر����ں��棬���� erase��������unique������������ͬ����Ԫ�ؽ��в���
// �Ӵ�Ԫ����� accumulate����
// /unique()�������ظ���Ԫ�طŵ�vector��β�� Ȼ�󷵻�ָ���һ���ظ�Ԫ�صĵ�����
// ����erase�������������Ԫ�ص����Ԫ�ص����е�Ԫ��
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
	// ���Ӵ�
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
			vector_son.push_back(tmp);  //��ÿ���׵�ַ��ʼ�����һ���Ӵ�
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
	// sizeof(*array)  sizeof(array[0]) sizeof(int)���ֱ�ʾ��������ʾarray�ĵ�ÿ��Ԫ����ռ�ռ��С
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