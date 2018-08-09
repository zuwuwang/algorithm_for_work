/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
#include <iostream>
#include <vector>
using namespace std;

// 链表节点结构
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
		val(x), next(NULL) {		
		}
};


// 使用vector的反转函数实现
class Solution {
public:
	vector<int> printListFromTailToHead(ListNode* head) {
		vector<int> ArrayList;
		if (head == NULL)
			return ArrayList;
		ListNode *p = head;
		while (p != NULL)
		{
			ArrayList.push_back(p->val);
			p = p->next;
		}
		// 使用vector的反转函数实现
		reverse(ArrayList.begin(), ArrayList.end());
		return ArrayList;
	}
};


// 使用stack实现
// Stack<ListNode*> reverse;
#include <stack>
class Solution {
public:
	vector<int> printListFromTailToHead(ListNode* head) {
		vector<int> ArrayList;
		stack<ListNode*> reverse;
		if (head == NULL)
			return ArrayList;
		ListNode *p = head; //将头指针重命名
		while (p != NULL)
		{
			reverse.push(p); //将节点结构体入栈，栈中存储的元素的类型为ListNode*型，是节点指针
			p = p->next;
		}
		while (!reverse.empty())
		{
			p = reverse.top();//取出栈顶,注意是top方法，栈底是tail方法
			ArrayList.push_back(p->val);
			reverse.pop(); //元素出栈，即删除，是pop方法
		}
		return ArrayList;
	}
};


// 递归，递归到链表最后一个节点位置

class Solution {
public:
	vector<int> ArrayList;
	vector<int>& printListFromTailToHead(struct ListNode* head) {
		if (head != NULL) {
			if (head->next != NULL) {
				ArrayList = printListFromTailToHead(head->next);
			}
			ArrayList.push_back(head->val);
		}
		return ArrayList;
	}
};