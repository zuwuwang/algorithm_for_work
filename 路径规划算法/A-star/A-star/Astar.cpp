// #include "stdafx.h"
#include "Astar.h"
int map[101][101] =
{
	{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 0, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 1, 0 }
};
Astar::Astar()
{

}
Astar::~Astar()
{

}

void Astar::search(Node* startPos, Node* endPos)  //��֪��ʼ����ֹ�ڵ�������һλ��
{
	if (startPos->x < 0 || startPos->x > row || startPos->y < 0 || startPos->y >col ||
		endPos->x < 0 || endPos->x > row || endPos->y < 0 || endPos->y > col)
		return;
	Node* current;
	this->startPos = startPos;
	this->endPos = endPos;

	openList.push_back(startPos);//1����������open�У���Ϊ�ȴ�����Ľڵ�

	//��Ҫ����飬�ѿ�ʼ�Ľڵ����openlist��ʼ�����Աߵ�8���ڵ�
	//������곬����Χ����closelist��return 
	//����Ѿ�����openlist�ͶԱȵ�ǰ�ڵ㵽���������Ǹ��ڵ��Gֵ�͵�ǰ�ڵ㵽ԭ�����ڵ��Gֵ 
	//���ԭ����Gֵ�Ƚϴ󣨼����ҵ�·����gֵ�Ƚ�С�������¸�ֵGֵ ���ڵ� ��f 
	//������½ڵ� ���뵽openlist ֱ��opellistΪ�ջ��ҵ��յ�
	while (openList.size() > 0)
	{
		current = openList[0]; 

		if (current->x == endPos->x && current->y == endPos->y)//�����ǰ����Ŀ��㣬�������
		{
			cout << "find the path" << endl;
			printMap();
			printPath(current);
			openList.clear();
			closeList.clear();
			break;
		}
		//�����ǰ�㲻��Ŀ��㣬�Ͱ�����Ϊ��һ�����ڵ��������
		NextStep(current); //2����Χ��8�������open�У���ǰ�ڵ�close��
		closeList.push_back(current);
		openList.erase(openList.begin());
		sort(openList.begin(), openList.end(), compare);//3�����Զ���compare�������򣬴�open�еõ�fn��С�Ľڵ�
	}
}

void Astar::checkPoit(int x, int y, Node* father, int g)
{
	if (x < 0 || x > row || y < 0 || y > col)  // ����Ƿ�Խ��
		return;
	if (this->unWalk(x, y)) //���ڽڵ��ǲ���ͨ���ĵ㣬�˳�
		return;
	if (isContains(&closeList, x, y) != -1) // ���������ڽڵ��Ѿ�close�����˳�
		return;
	int index;
	if ((index = isContains(&openList, x, y)) != -1)  // ���ڽڵ���open�б���
	{
		Node *point = openList[index]; // �����ڽڵ���pointָ��
		if (point->g > father->g + g)  // �ж���·��(��father->g + g)λ�ô���g�Ƿ��С��ԭÿ��λ�ö���һ��g,pose->g��
		{
			point->father = father;
			point->g = father->g + g;
			point->f = point->g + point->h;
		}
	}
	else  //������ڵ㲻��open�б��У�����push_back��open�б��У�������g��h
	{
		Node * point = new Node(x, y, father);
		countGHF(point, endPos, g);//��ַ����
		openList.push_back(point);
	}
}

void Astar::NextStep(Node* current)
{
	// ��鵱ǰ�������ҿ��õ�դ�񣨲������ϰ����close�Ľڵ㣩
	// ǰ�������ĸ��ڵ�Ĵ���ֵΪ10��WeightW
	checkPoit(current->x - 1, current->y, current, WeightW);//��
	checkPoit(current->x + 1, current->y, current, WeightW);//��
	checkPoit(current->x, current->y + 1, current, WeightW);//��
	checkPoit(current->x, current->y - 1, current, WeightW);//��
	// �ĸ��ԽǵĴ���ֵΪ14��WeightWH
	checkPoit(current->x - 1, current->y + 1, current, WeightWH);//����
	checkPoit(current->x - 1, current->y - 1, current, WeightWH);//����
	checkPoit(current->x + 1, current->y - 1, current, WeightWH);//����
	checkPoit(current->x + 1, current->y + 1, current, WeightWH);//����
}

int Astar::isContains(vector<Node*>* Nodelist, int x, int y) // open����close�����ĵ�
{
	for (int i = 0; i < Nodelist->size(); i++)
	{
		if (Nodelist->at(i)->x == x && Nodelist->at(i)->y == y)
		{
			return i;  // ���ذ����ĵ������ֵ
		}
	}
	return -1;
}

void Astar::countGHF(Node* sNode, Node* eNode, int g)  //�β�Ϊ�����ڵ㣬��ǰ�ڵ�eNode����ʵ�ڵ�sNode���������ֵ
{
	int h = abs(sNode->x - eNode->x) + abs(sNode->y - eNode->y) * WeightW;//�޸ĺ����������
	printf("h is = %d\n", h);
	//h�ļ��㷽ʽӦ��������һ�֣��������h���������ֵ����Ȩ���ټ��Ϻ�����Ĳ�ֵ
	int dealt_x = abs(sNode->x - eNode->x);
	int dealt_y = abs(sNode->y - eNode->y);
    h = max(dealt_x, dealt_y) * WeightW;
	printf("new h is %d\n", h);
	int current_g = sNode->father->g + g; //��ǰ��g�����ڵ�Ĵ���g���ϵ�ǰ�ڵ㵽�丸�ڵ�Ĵ���
	int f = current_g + h;
	sNode->f = f;
	sNode->h = h;
	sNode->g = current_g;
}

bool Astar::compare(Node* n1, Node* n2)
{
	//printf("%d,%d",n1->f,n2->f);
	return n1->f < n2->f; //���ؽ�С���Ǹ�
}

bool Astar::unWalk(int x, int y)
{
	if (map[x][y] == 1)  // ����ͨ���ĵ��ֵΪ1
		return true;
	return false;
}

void Astar::printPath(Node* current)
{
	if (current->father != NULL)
		printPath(current->father);//�ݹ����
	printf("(%d,%d)", current->x, current->y);
}

void Astar::printMap()
{
	for (int i = 0; i <= row; i++){
		for (int j = 0; j <= col; j++){
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}