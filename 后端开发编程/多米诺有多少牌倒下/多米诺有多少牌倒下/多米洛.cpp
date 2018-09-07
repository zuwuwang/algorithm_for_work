#include<iostream>
#include <vector>
using namespace std;

/*
²âÊÔÑùÀý
4
16 5
20 5
10 10
18 2

3 1 3 1
*/

int main()
{
	int *p = NULL;
	//int a = sizeof(p);
	int N;
	cin >> N;
	vector<int> X;
	vector<int> H;
	for (int j = 0; j < N; j++)
	{
		int x, h;
		cin >> x >> h;
		X.push_back(x);
		H.push_back(h);
	}
	for (int k = 0; k < N; k++)
	{
		int num = 0;
		int xmin, xmax;
		xmin = X[k] + 1;
		xmax = X[k] + H[k] - 1;
		for (int i = 0; i < N; i++)
		{
			int check = X[i];
			if (X[i] >= xmin &&X[i] <= xmax)
			{
				num++;
			}
		}
		cout << num + 1 << " ";
	}
	system("pause");
	return 1;
}