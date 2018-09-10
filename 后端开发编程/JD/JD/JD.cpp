//// 京东第二题【王祖武】
//#include <iostream>
//#include <map>
//#include <string>
//#include <algorithm>
//#include <sstream>
//using namespace std;
//
//int main()
//{
//	int N = 0;
//	cin >> N;
//	// 用sstream的时候注意这一点，需要cin.ignore()和cin.clear()
//	cin.ignore();
//	cin.clear();
//	string line;
//	multimap<int, int> prop;
//	int num = 0;
//	int count = 0;
//	int resault = 0;
//	for (int i = 0; i < N; i++)
//	{
//		getline(cin, line);
//		stringstream ssLine(line);
//		while (ssLine >> num)
//		{
//			prop.insert(make_pair(i, num));
//		}
//	}
//	for (int j = 0; j < N; j++)
//	{
//		for (int k = j; k < N; k++)
//		{
//			multimap<int, int>::iterator start = prop.lower_bound(j);
//			multimap<int, int>::iterator end = prop.upper_bound(j);
//			multimap<int, int>::iterator next_start = prop.lower_bound(k);
//			multimap<int, int>::iterator next_end = prop.upper_bound(k);
//			// start != end && next_start != next_end 放在前面判断，避免使用野指针
//			while ((start->second) < (next_start->second))
//			{
//				start++;
//				next_start++;
//				if (start == end && next_start == next_end)
//				{
//					resault++;
//					break;
//				}
//			}
//		}
//	}
//	cout << resault;
//	return 0;
//}


// 京东第二题 【于哲】
#include <iostream>
#include <vector>
using namespace std;

int compare(vector<vector<int>> arr){
	int resault = 0;
	for (int i = 0; i < arr.size(); i++){
		for (int j = 0; j < arr.size(); j++){
			if (i == j){
				continue;
			}
			if (arr[i][0] < arr[j][0] && arr[i][1] < arr[j][1] && arr[i][2] < arr[j][2]) {
				resault++;
				break;
			}
		}
	}
	return resault;
}

int main(){
	int n;
	cin >> n;
	vector<vector<int>> input;
	for (int i = 0; i < n; i++){
		int a, b, c;
		cin >> a >> b >> c;
		input.push_back({ a, b, c });
	}
	cout << compare(input) << endl;


	return 0;
}


//京东第一题【Java版本】
//import java.util.Scanner;
//
//
//public class Main
//{
//	public static void main(String args[])
//	{
//		Scanner sc = new Scanner(er(System.in);
//		int N = sc.nextInt();
//		while (N-- > 0)
//		{
//			int nodenum  = sc.nextInt();
//			int sidenum  = sc.nextInt();
//			boolean nodes[][] = new boolean[nodenum][nodenum];
//			while (sidenum-- > 0)
//			{
//				int i = sc.nextInt()  - 1;
//				int j = sc.nextInt()  - 1;
//				nodes[i][j] = true;
//				nodes[j][i] = true;
//			}
//			int result[] = new int[nodenum];
//			for (int i = 0; i < nodenum; i++)
//			{
//				for (int j = 0; j < nodenum; j++)
//				{
//					if (nodes[i][j]) result[i]++;
//				}
//			}
//			boolean flag = true;
//			for (int i = 0; i < nodenum; i++)
//			{
//				for (int j = i; j < nodenum; j++)
//				{
//					if (!nodes[i][j])
//					{
//						if (result[i] != result[j])
//						{
//							flag = false;
//							break;
//						}
//					}
//				}
//				if (!flag) break;
//			}
//			if (!flag)
//			{
//				System.out.println("No");
//			}
//			else
//			{
//				System.out.println("Yes");
//			}
//		}
//	}
//}