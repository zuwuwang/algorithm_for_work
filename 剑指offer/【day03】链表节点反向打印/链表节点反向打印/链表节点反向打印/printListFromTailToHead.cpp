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

// ����ڵ�ṹ
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
		val(x), next(NULL) {		
		}
};


// ʹ��vector�ķ�ת����ʵ��
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
		// ʹ��vector�ķ�ת����ʵ��
		reverse(ArrayList.begin(), ArrayList.end());
		return ArrayList;
	}
};


// ʹ��stackʵ��
// Stack<ListNode*> reverse;
#include <stack>
class Solution {
public:
	vector<int> printListFromTailToHead(ListNode* head) {
		vector<int> ArrayList;
		stack<ListNode*> reverse;
		if (head == NULL)
			return ArrayList;
		ListNode *p = head; //��ͷָ��������
		while (p != NULL)
		{
			reverse.push(p); //���ڵ�ṹ����ջ��ջ�д洢��Ԫ�ص�����ΪListNode*�ͣ��ǽڵ�ָ��
			p = p->next;
		}
		while (!reverse.empty())
		{
			p = reverse.top();//ȡ��ջ��,ע����top������ջ����tail����
			ArrayList.push_back(p->val);
			reverse.pop(); //Ԫ�س�ջ����ɾ������pop����
		}
		return ArrayList;
	}
};


// �ݹ飬�ݹ鵽�������һ���ڵ�λ��

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