




// 头条第一题
//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <cstring>
//using namespace std;
//
//int maxSubstring(string str, int n)
//{
//	int arr[256];
//
//	memset(arr, -1, 256 * sizeof(int));
//
//	int pre = -1, maxSubLen = 0;
//
//	for (int i = 0; i < n; ++i)
//	{
//		pre = max(pre, arr[str[i]]);
//		maxSubLen = max(maxSubLen, i - pre);
//
//		arr[str[i]] = i;
//	}
//
//	return maxSubLen;
//}
//
//int main()
//{
//	string str;
//	cin >> str;
//
//	cout << maxSubstring(str, str.length()) << endl;
//
//	return 0;
//}

//头条第一题 【王祖武】
//#include<iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	string str1;
//	string str2;
//	vector<int> resaultVec;
//	cin >> str1;
//	for (int i = 0; i != str1.size(); i++)
//	{
//		str2.push_back(str1[i]);
//		int num = str2.find(str1[i + 1]);
//		if (str2.find(str1[i + 1]) != string::npos)
//		{
//			resaultVec.push_back(str2.size());
//			str2.erase();
//		}
//	}
//	sort(resaultVec.begin(), resaultVec.end());
//	cout << *resaultVec.rbegin();
//	return 0;
//}

////头条第二题
//#define _CRT_SECURE_NO_WARNINGS
//#include<cstdio>
//#include<iostream>
//#include<queue>
//using namespace std;
//
//const int maxn = 100;
//
//struct node{
//	int x, y;
//}Node;
//
//int n;//矩阵大小
//int matrix[maxn][maxn];//01矩阵
//bool flag[maxn][maxn] = { false };//记录位置(x,y)是否被记录过
//int X[4] = { 0, 0, 1, -1 };
//int Y[4] = { 1, -1, 0, 0 };
//
///*判断点(x,y)是否被访问过*/
//bool judge(int x, int y){
//	if (x >= n || x<0 || y >= n || y<0)return false;
//	if (matrix[x][y] == 0 || flag[x][y] == true)return false;
//	return true;
//}
//
//void BFS(int x, int y){
//	queue<node> Q;
//	Node.x = x, Node.y = y;
//	Q.push(Node);
//	flag[x][y] = true;//记录该点已经被访问过
//	while (!Q.empty()){
//		node top = Q.front();//取出队首元素
//		Q.pop();//队首元素出队
//		for (int i = 0; i<4; i++){
//			int newX = top.x + X[i];
//			int newY = top.y + Y[i];
//			if (judge(newX, newY)){
//				//如果点(newX,newY)没有被访问过，那么将该点入队并且标记为已经被访问过
//				Node.x = newX, Node.y = newY;
//				Q.push(Node);
//				flag[newX][newY] = true;
//			}
//		}
//	}
//}
//
//int main(){
//	scanf("%d", &n);
//	for (int i = 0; i<n; i++){
//		for (int j = 0; j<n; j++)
//		{
//			scanf("%d", &matrix[i][j]);
//		}
//	}
//	int ans = 0;//记录答案块数
//	for (int x = 0; x<n; x++){
//		for (int y = 0; y<n; y++){
//			if (matrix[x][y] == 1 && flag[x][y] != 1){
//				//如果该点满足要求而且尚未被访问过,那么块数ans++并且执行BFS
//				ans++;
//				BFS(x, y);
//			}
//		}
//	}
//	printf("%d\n", ans);
//	return 0;
//}


//头条第三题  IP地址还原
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//
//vector<string> ans;
//
//void dfs(string &s, int index, int num, string tmp)
//{
//	if ((5 - num) * 3 < s.size() - index)
//		return;
//	if (num == 5) {
//		ans.push_back(tmp);
//		return;
//	}
//	if (s[index] == '0')
//		dfs(s, index + 1, num + 1, tmp + (num == 1 ? "0" : ".0"));
//	else
//	{
//		for (int i = 1; i < 4 && i + index <= s.size(); ++i)
//		if (i < 3 || string(s, index, i) <= "255")
//			dfs(s, index + i, num + 1, tmp + (num == 1 ? "" : ".") + string(s, index, i));
//	}
//}
//
//vector<string> restoreIpAddresses(string s)
//{
//	dfs(s, 0, 1, "");
//	cout << ans.size();
//	return ans;
//}
//
//int main()
//{
//	string str;
//	cin >> str;
//	restoreIpAddresses(str);
//	return 0;
//}


// 头条第四题 UTF-8编码校验
//#include <iostream>
//#include <vector>
//#include <math.h>
//using namespace std;
//
//class UTF8Check
//{
//public:
//	bool validUtf8(vector<int>& data) {
//		for (int i = 0; i < data.size(); ++i) {
//			if (data[i] < 0b10000000) {
//				continue;
//			}
//			else {
//				int cnt = 0, val = data[i];
//				for (int j = 7; j >= 1; --j) {
//					if (val >= pow(2, j)) ++cnt;
//					else break;
//					val -= pow(2, j);
//				}
//				if (cnt == 1) return false;
//				for (int j = i + 1; j < i + cnt; ++j) {
//					if (data[j] > 0b10111111 || data[j] < 0b10000000) return false;
//				}
//				i += cnt - 1;
//			}
//		}
//		return true;
//	}
//};
//
//
//int main()
//{
//	int n;
//	cin >> n;
//	vector<int> data;
//	for (int i = 0; i < n; i++)
//	{
//		int x;
//		cin >> x;
//		data.push_back(x);
//	}
//	UTF8Check S;
//	bool d = S.validUtf8(data);
//	if (d)
//	{
//		cout << "1";
//	}
//	else
//	{
//		cout << "0";
//	}
//	return 0;
//}
// 头条第五题
//#include <iostream>
//#include <vector>
//#include <sstream>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	int N, M;
//	string str;
//	string subStr;
//	getline(cin, str);
//	stringstream ssStr(str);
//	while (ssStr >> M)  //用while循环来获取输入
//	{
//		
//	}
//
//	return 0;
//}
