#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> v;
	int number;
	cin >> number;
	//�ڶ����������ݣ��Կո�ֿ�
	//cin�������Ὣ��Щ���ݷֱ���뵽vector��
	// �����ж�ȡ����֮ǰ�������ո񣬶���֮�������ո�ͽ�����һ�����������룬֮����еڶ������������롣
	// �ո������Ľ�����־
	for (int i = 0; i < number; i++)
	{
		int temp;
		cin >> temp;
		v.push_back(temp);
		//����string����Ҳ�ǿ��Եģ���ÿ�������Ԫ�طŵ�string��
	}
	for (vector<int>::iterator itor = v.begin(); itor < v.end(); itor++)
	{
		cout << *itor << endl;
	}
	system("pause");
	return 0;
}

// �����
// ��ȡ���룬ָ�����У�ʹ��vectorʵ�ֶ�ά����
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



//�����
// ����������ȡ���룬����������Ԫ�ظ�ֵ����
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
//	vector<vector<int> > vec;//������vector��Ԫ��ֵΪ0
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			cin >> value;
//			/*vec[i][j] = value;*/
//			v.push_back(value);//push_back()�ڿ�vector��������Ԫ��
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



//�����
//����������ȡ���룬�̶��в��̶��еĶ�̬��ά����
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
	cin.clear();  // cin.clear��cin.ignore����������ϵ����C++�й���cin�Ľ���
	cin.ignore();  // cin.ignore���������
	//cin.sync();//����Щ�������Ͽ��ܲ�����ȷ���������
	int num;
	vector<int> a;
	vector<vector<int>> b;
	for (int j = 0; j < N; j++)
	{
		//cin.clear();
		getline(cin, input);  // ������cin.getlin() �����������ַ���
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
