#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

using namespace std;

int main(){
	int n = 0;
	vector<int> apple;
	vector<int> sum;
	vector<int> res;
	cin >> n;
	for (int i = 0; i<n; ++i){
		int num = 0;
		cin >> num;
		apple.push_back(num);
	}
	for (vector<int>::iterator i = apple.begin(); i < apple.end(); i++)
	{
		sum.push_back(accumulate(apple.begin(), i,0));
	}
	int m = 0;
	cin >> m;
	for (int i = 0; i<m; ++i)
	{
		int qq = 0;
		cin >> qq;
		/*int add = 0;
		for (int j = 0; j<n; ++j){
			add += apple[j];
			if (add >= qq){
				res.push_back(j);
				break;
			}
		}*/
		cout << res[i] + 1 << endl;
		for (vector<int>::iterator i = sum.begin(); i < sum.end(); i++){
			if (qq < (*i))
			{
				/*cout << */
			}
				
		}
		
	}
	return 0;
}