#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> v;
	int number;
	cin >> number;
	//第二行输入数据，以空格分开
	//cin输入流会将这些数据分别存入到vector中
	// 从流中读取数据之前会跳过空格，读完之后遇到空格就结束第一个变量的输入，之后进行第二个变量的输入。
	// 空格是流的结束标志
	for (int i = 0; i < number; i++)
	{
		int temp;
		cin >> temp;
		v.push_back(temp);
		//或者string操作也是可以的，将每次输入的元素放到string中
	}
	for (vector<int>::iterator itor = v.begin(); itor < v.end(); itor++)
	{
		cout << *itor << endl;
	}
	system("pause");
	return 0;
}

// 情况二
// 获取输入，指定行列，使用vector实现二维数组
//#include <iostream>
//#include <vector>
//using namespace std;
//
//void main()
//{
//	int N;
//	int M;
//	int value;
//	cin >> M >> N;
//	vector<vector<int> > vec(M,vector<int>(N));
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			cin >> value;
//			vec[i][j] = value;
//		}
//	}
//
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			cout << vec[i][j] << ' ';
//		}
//	}
//	system("pause");
//}
//



//情况三
// 从输入流获取输入，容器的容器元素赋值方法
//#include <iostream>
//#include <vector>
//using namespace std;
//
//void main()
//{
//	int N;
//	int M;
//	int value;
//	cin >> M >> N;
//	vector<int> v;
//	vector<vector<int> > vec;//创建空vector，元素值为0
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			cin >> value;
//			/*vec[i][j] = value;*/
//			v.push_back(value);//push_back()在空vector后继续添加元素
//		}
//		vec.push_back(v);
//		v.clear();
//	}
//
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			cout << vec[i][j] << ' ';
//		}
//	}
//	system("pause");
//}
//



//情况四
//从输入流获取输入，固定行不固定列的动态二维数组
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


void main()
{
	string input;
	int N;
	cin >> N;
	cin.clear();  // cin.clear与cin.ignore的区别与联系，看C++中关于cin的介绍
	cin.ignore();  // cin.ignore清除缓冲区
	//cin.sync();//在有些编译器上可能不能正确清除缓冲区
	int num;
	vector<int> a;
	vector<vector<int>> b;
	for (int j = 0; j < N; j++)
	{
		//cin.clear();
		getline(cin, input);  // 区别与cin.getlin() 适用于数组字符串
		stringstream stringin(input);
		while (stringin >> num)
		{
			a.push_back(num);
		}
		b.push_back(a);
		a.clear();
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < b[i].size(); j++)
			cout << b[i][j] << ' ';
	}
	system("pause");
}
