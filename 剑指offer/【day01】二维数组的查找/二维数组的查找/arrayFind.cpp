#include "arrayFind.h"


void main()
{
	Solution solution;
	int b[4][4] = { 1, 2, 8, 9, 2, 4, 9, 12, 4, 7, 10, 13,6,8,11,15 };
	vector<vector<int> > testArray(4,vector<int> (4)); //�ö�ά�����ʼ��vector�����Ķ�ά����
	for (int i = 0; i < testArray.size(); i++)
	{
		for (int j = 0; j < testArray[0].size(); j++)
		{
			testArray[i][j] = b[i][j];
			cout << testArray[i][j] << endl;
		}
	}
	solution.Find(7, testArray);
	system("pause");
}