
// ��;�� �������������Լ������С��������
//#include <iostream>
//using namespace std;
//
//// �����Լ��
//// ��ת�����
//// �ҳ�max min
//// max%min �������Ļ���min����������ѭ������
//// 
//int MaxY(int a, int b)
//{
//	int min, max;
//	max = a>b ? a : b;
//	min = a<b ? a : b;
//	if (max%min == 0)
//		return min;
//	else
//		return MaxY(min, max%min); //�ݹ��ʵ��
//}
//
////����С����������С��������������֮���������Լ��
//// max %min
//// 
//int MinY(int a, int b)
//{
//	int min, max;
//	int r;
//	max = a>b ? a : b;
//	min = a<b ? a : b;
//	if (max%min == 0)  //�������ֱ�ӳ�������ô�������ֵ��Ϊ��С������
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


//��;�ҵڶ���  ������Ԫ�غ�Ϊsum�ķ�������
// �ҵĴ𰸣�����
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

//  ���ٶȴ𰸣����᡿
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
			if (j >= v[i - 1]) //�����������������v[i-1]��ֵ��
							// ��ô�µķ�����������һ�еķ��������������µ�ֵv[i-1]���µķ�����
							// ������һ���У�j-v[i-1]�еķ�����֮��
				res[i][j] = res[i - 1][j] + res[i - 1][j - v[i - 1]];  
			else   //������С��V[i-1]�����������ӵ����޷������µķ���������������һ���ϵ�
				res[i][j] = res[i - 1][j];
		}
	}
	cout << res[n][sum] << endl;
	return 0;
}

//������ ��Сʯ�ӱ���
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