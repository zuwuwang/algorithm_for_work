//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	string str;
//	int i = 0;
//	vector<char> priceArray;
//	vector<char> resaultArray;
//	cin >> str;
//	while (str[i] != '\n')
//	{
//		if (str[i] != ' ')
//		{
//			priceArray.push_back(str[i]);
//		}
//		i++;
//		cout << str[i];
//	}
//	int count = 0;
//	for (vector<char>::iterator first = priceArray.begin(); first != priceArray.end(); first++)
//	{
//		count++;
//		for (vector<char>::iterator second = priceArray.begin() +count; second != priceArray.end(); second++)
//		{
//			resaultArray.push_back(*second - *first);
//		}
//	}
//	sort(resaultArray.begin(), resaultArray.end());
//	cout << *resaultArray.end() << endl;
//	system("pause");
//	return 0;
//}
//
//
///*
//
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//int i;
//vector<int> priceArray;
//vector<int> resaultArray;
//while ( cin.get()!='\n')
//{
//scanf("%d", &i);
//priceArray.push_back(i);
//}
//cout << i << endl;
//int count = 0;
//for (vector<int>::iterator first = priceArray.begin(); first != priceArray.end(); first++)
//{
//count++;
//for (vector<int>::iterator second = priceArray.begin() +count; second != priceArray.end(); second++)
//{
//resaultArray.push_back(*second - *first);
//}
//}
//sort(resaultArray.begin(), resaultArray.end());
//cout << *resaultArray.end() << endl;
//system("pause");
//return 0;
//}
//*/
