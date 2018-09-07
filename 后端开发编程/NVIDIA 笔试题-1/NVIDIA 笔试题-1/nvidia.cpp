// NVIDIA第一题
//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <numeric>
//using namespace std;
//
//int mian()
//{
//	int lines;
//	string records;
//	vector<string> logs;
//	cin >> lines;
//	for (int i = 0; i < lines; i++)
//	{
//		cin >> records;
//		if (records.size() < 13)
//			return -1;
//		logs.push_back(records);
//	}
//	int days = 0;
//	vector<string> hh;
//	vector<string> mm;
//	vector<string> message_text;
//	for (int j = 0; j < lines; j++)
//	{
//		hh[j] = logs[j].substr(1, 5);
//		mm[j] = logs[j].substr(7, 9);
//		message_text[j] = logs[j].substr(14);
//	}
//	for (int k = 0; k < lines-1; k++)
//	{
//		if (hh[k] == hh[k + 1])
//		{
//			if (message_text[k] == message_text[k + 1])
//			{
//				days++;
//			}
//		}
//		else
//		{
//			if (!hh[k].compare(hh[k + 1]))
//				if (message_text[k] == message_text[k + 1])
//					days++;
//		}
//	}
//	cout << days << endl;
//	return 0;
//}


// 第二题【王鑫睿】

//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//vector<int> find_nums(vector<int>& Ai)
//{
//	vector<int> Ai_tmp;
//	vector<int> shunxu;
//	vector<int>::iterator ret;
//	for (int j = 0; j < Ai.size(); j++)
//	{
//		Ai_tmp.push_back(Ai[j]);
//	}
//	sort(Ai_tmp.begin(), Ai_tmp.end());
//	for (int k = 0; k < Ai.size(); k++)
//	{
//		ret = find(Ai.begin(), Ai.end(), Ai_tmp[k]);
//		auto index = distance(begin(Ai), ret);
//		shunxu.push_back(Ai[index]);
//	}
//	sort(shunxu.begin(), shunxu.end());
//	shunxu.erase(unique(shunxu.begin(), shunxu.end()), shunxu.end());
//	return shunxu;
//}
//
//int main()
//{
//	int N;
//	cin >> N;
//	vector<int> Ai;
//	vector<int> nums;
//	for (int i = 0; i < N; i++)
//	{
//		int x;
//		cin >> x;
//		Ai.push_back(x);
//	}
//	nums = find_nums(Ai);
//	cout << nums.size();
//	cout << endl;
//	for (int j = 0; j < nums.size(); j++)
//	{
//		cout << nums[j];
//		cout << " ";
//	}
//	system("pasue");
//	return 0;
//}

// 第二题【王祖武】
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	int N;
//	int ml;
//	vector<int> Ai;
//	cin >> N;
//	for (int i = 0; i < N; i++)
//	{
//		cin >> ml;
//		Ai.push_back(ml);
//	}
//	sort(Ai.begin(), Ai.end());
//	Ai.erase(unique(Ai.begin(), Ai.end()), Ai.end());
//	cout << Ai.size() << endl;
//	for (int j = 0; j < Ai.size(); j++)
//	{
//		cout << Ai[j] << ' ';
//	}
//	system("pasue");
//	return 0;
//}
//
//


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

//从输入流获取输入，固定行不固定列的动态二维数组

//#include <iostream>
//#include <vector>
//#include <string>
//#include <sstream>
//using namespace std;
//
//void main()
//{
//	string input;
//	int N;
//	cin >> N;
//	cin.clear();  // cin.clear与cin.ignore的区别与联系，看C++中关于cin的介绍
//	cin.ignore();  // cin.ignore清除缓冲区
//	//cin.sync();//在有些编译器上可能不能正确清除缓冲区
//	int num;
//	vector<int> a;
//	vector<vector<int>> b;
//	for (int j = 0; j < N; j++)
//	{
//		//cin.clear();
//		getline(cin, input,'0');  // 区别与cin.getlin() 适用于数组字符串
//		stringstream stringin(input);
//		while (stringin >> num)
//		{
//			a.push_back(num);
//		}
//		b.push_back(a);
//		a.clear();
//	}
//	for (int i = 0; i < N; i++)
//	{
//		for (int j = 0; j < b[i].size(); j++)
//			cout << b[i][j] << ' ';
//	}
//	system("pause");
//}

