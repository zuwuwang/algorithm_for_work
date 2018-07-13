#ifndef ASTAR_H
#define ASTAR_H
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include<algorithm>
using namespace std;

typedef struct Node
{
	int x, y;
	int g; //��ʼ�㵽��ǰ��ʵ�ʴ���
	int h;//��ǰ�ڵ㵽Ŀ��ڵ����·���Ĺ��ƴ���
	int f;//����ֵ
	Node* father;
	Node(int x, int y)  //�������ڵ�ĳ�ʼ������ʼ����ֹ�ڵ㡣Node����
	{
		this->x = x;
		this->y = y;
		this->g = 0;
		this->h = 0;
		this->f = 0;
		this->father = NULL;
	}
	Node(int x, int y, Node* father)
	{
		this->x = x;
		this->y = y;
		this->g = 0;
		this->h = 0;
		this->f = 0;
		this->father = father;
	}
}Node;


class Astar{
public:
	Astar();
	~Astar();
	void search(Node* startPos, Node* endPos);

	void checkPoit(int x, int y, Node* father, int g);
	void NextStep(Node* currentPoint);
	int isContains(vector<Node*>* Nodelist, int x, int y);
	void countGHF(Node* sNode, Node* eNode, int g);
	static bool compare(Node* n1, Node* n2);
	bool unWalk(int x, int y);
	void printPath(Node* current);
	void printMap();  // 7*9
	vector<Node*> openList;
	vector<Node*> closeList;
	Node *startPos;
	Node *endPos;
	static const int WeightW = 10;// ����������
	static const int WeightWH = 14;//��б���������
	static const int row = 6; // 0-6
	static const int col = 8; // 0-8
};
#endif