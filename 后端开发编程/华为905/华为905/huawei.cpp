
//��Ϊ�ڶ��⡾�����䡿
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	string A, B;
//	int count = 0;
//	cin >> A >> B;
//	if (A.size() <= B.size())
//		return -1;
//	if (A.size() + B.size() < 5)
//		return -1;
//	sort(A.begin(), A.end());
//	sort(B.begin(), B.end());
//	A.erase(unique(A.begin(), A.end()), A.end());
//	B.erase(unique(B.begin(), B.end()), B.end());
//	for (int i = 0; i < B.size(); i++)
//	{
//		for (int j = 0; j < A.size(); j++)
//		{
//			if (B[i] == A[j])
//			{
//				count++;
//			}
//		}
//	}
//	if (count == B.size())
//		cout <<"true";
//	else
//		cout << "false";
//	return 0;
//}
//

//��Ϊ�����⡾�����䡿
// ��ԭ���ַ����зֳ���������ĸ�ַ����͸����ַ���
//#include <iostream>
//#include <map>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <functional>
////#include <numeric>
//#include <sstream>
//
//using namespace std;
//int main()
//{
//	string str;  //�����ַ���
//	map<int, string, greater<int> > strMap;
//	cin >> str;
//	string chs;  //����зֳ������ַ�
//	string nums; //����зֳ���������
//	nums = str;
//	for (int i = 0; i < str.size(); i++)
//	{
//		if (nums[i] >= 'a' &&nums[i] <= 'z')
//		{
//			chs.push_back(nums[i]);
//			nums[i] = ' ';
//		}
//		else
//		{
//			chs.push_back(' ');
//		}
//	}
//	cout << chs << endl << nums << endl;
//	// Ȼ���ٽ�ת��������iostringstream��sstreamͷ�ļ��У������н�ÿ���ַ�����������ŵ������б��ڲ���
//	stringstream ssChs(chs);
//	stringstream ssNums(nums);
//	vector<string> chsVec;
//	vector<int> numsVec;
//	string s;
//	int n;
//	while (ssChs >> s)
//	{
//		chsVec.push_back(s);
//	}
//	while (ssNums >> n)
//	{
//		numsVec.push_back(n);
//	}
//	// strMapӳ�乤����������
//	// ��Ӧ����map��,����һ��ӳ���ϵ��Ȼ�󵥶���vector���򣬶�Ӧȡ��ֵ���С�
//	for (int i = 0; i < numsVec.size(); i++)
//	{
//		strMap[numsVec[i]] = chsVec[i];
//	}
//	// Map�ڲ��Զ�ʹ�ú�������ռ�ֵ��С�������򣬲���Ҫ�ٵ����Լ�����
//	// �����Ҫ���մӴ�С�������Լ�����ȽϺ�����ʹ��functional�е�greater����
//	// sort(numsVec.begin(), numsVec.end()); 
//	int loops = 0;
//	for (map<int, string>::iterator itor = strMap.begin(); itor != strMap.end(); itor++)
//	{
//		loops = itor->first;
//		while (loops != 0)
//		{
//			cout << itor->second;
//			loops--;
//		}
//	}
//	system("pause");
//	return 0;
//}
//

// ��Ϊ��һ�⡾�����䡿
////  60%+�ٵ���С��26�����  Ӧ��û���� 100%
//#include <iostream>
//#include <math.h>
//#include <string>
//#include <vector>
//using namespace std;
//
//int main()
//{
//	string A, B, D;
//	vector<int> C;
//	long long AValue = 0;
//	long long BValue = 0;
//	long long CValue = 0;
//	cin >> A >> B;
//	for (int i = 0; i < A.size(); i++)
//	{
//		AValue += (A[i] - 97) * pow(26, A.size() - i - 1);
//	}
//	for (int i = 0; i < B.size(); i++)
//	{
//		BValue += (B[i] - 97) * pow(26, B.size() - i - 1);
//	}
//	CValue = AValue + BValue;
//	int residue = 0;
//	// ����ʱ���ٿ����˺�С��26�����
//	if (CValue<26)
//	{
//		D.push_back(97 + CValue);
//		cout << D;
//		return 0;
//	}
//	while (CValue >= 26)
//	{
//		residue = CValue % 26;
//		C.push_back(residue);
//		CValue = CValue / 26;
//		if (CValue < 26)
//			C.push_back(CValue);
//	}
//	
//	//�������е�Ԫ�ؽ��з�ת
//	//reverse(C.begin(), C.end());
//	for (int i = C.size() - 1; i >= 0; i--)
//	{
//		D.push_back(C[i] + 97);
//	}
//	cout << D;
//	return 0;
//}


// ��Ϊ��һ�� �������ܡ� 80%
//#include <iostream>
//#include <string.h>
//
//using namespace std;
//
//int char2integer(char*);
//int correct(char*);
//
//int main(int argc, char** argv)
//{
//	char ch1[100];
//	char ch2[100];
//	char res[100];
//	int a;
//	long sum;
//
//	while (cin >> ch1 && cin >> ch2){
//
//		int flag1 = 0;
//		int flag2 = 0;
//
//		flag1 = correct(ch1);
//		flag2 = correct(ch2);
//
//		if (flag1)
//			return -1;
//		if (flag2)
//			return -1;
//
//		sum = char2integer(ch1) + char2integer(ch2);
//
//		int i = 0;
//
//		while (sum)
//		{
//			a = sum % 26;
//			sum /= 26;
//			res[i] = a + 97;
//			i++;
//		}
//		for (int j = strlen(res) - 1; j >= 0; j--)
//			cout << char(res[j]);
//
//		cout << endl;
//	}
//
//	return 0;
//}
//
//int correct(char* ch)
//{
//	int flag = 0;
//	int len;
//	len = strlen(ch);
//	for (int i = 0; i<len; i++)
//	{
//		if (ch[i] >= 'a' && ch[i] <= 'z')
//			flag = 0;
//		else{
//			flag = 1;
//			break;
//		}
//	}
//	return flag;
//}
//
//int char2integer(char* ch)
//{
//	int len;
//	int resault = ch[0] - 97;
//
//	len = strlen(ch);
//	for (int i = 1; i<len; i++)
//	{
//		resault = 26 * resault + ch[i] - 97;
//	}
//	return resault;
//
//}

// ��Ϊ��һ�� ������� 
//#include<iostream>
//#include<string>
//using namespace std;
//
//string sums_26(string s1, string s2)
//{
//	string jinzhi = "abcdefghijklmnopqrstuvwxyz1";
//	string min;
//	string max;
//	string sum;
//	if (s1.size() > s2.size())
//	{
//		max = s1;
//		min = s2;
//	}
//	else
//	{
//		max = s2;
//		min = s1;
//	}
//
//	int j = max.size() - 1;
//	for (int i = min.size() - 1; i >= 0; i--)
//	{
//		size_t found1 = jinzhi.find(min[i]);
//		size_t found2 = jinzhi.find(max[j]);
//
//		if ((found1 + found2) >= 26)
//		{
//			max[j] = jinzhi[(found1 + found2) - 26];
//			int tmp = j - 1;
//			if (j == 0)
//			{
//				max[j] = jinzhi[0];
//				max.insert(0, "a");
//				break;
//			}
//			while (jinzhi.find(max[tmp]) == 25 && tmp >= 0)
//			{
//				max[tmp] = jinzhi[0];
//				if (tmp == 0)
//				{
//					max.insert(0, "a");
//				}
//				if (--tmp < 0)
//				{
//					break;
//				}
//			}
//			if (tmp >= 0)
//			{
//				size_t found3 = jinzhi.find(max[tmp]);
//				max[tmp] = jinzhi[found3 + 1];
//			}
//		}
//		else
//		{
//			max[j] = jinzhi[found1 + found2];
//		}
//		j--;
//	}
//	return max;
//}
//
//int main()
//{
//	string s1, s2;
//	string s3;
//	cin >> s1 >> s2;
//	int a = 0;
//	s3 = sums_26(s1, s2);
//	cout << s3;
//	system("pause");
//	return 0;
//}
