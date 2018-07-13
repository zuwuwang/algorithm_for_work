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

void Astar::search(Node* startPos, Node* endPos)  //已知起始和终止节点搜索下一位置
{
	if (startPos->x < 0 || startPos->x > row || startPos->y < 0 || startPos->y >col ||
		endPos->x < 0 || endPos->x > row || endPos->y < 0 || endPos->y > col)
		return;
	Node* current;
	this->startPos = startPos;
	this->endPos = endPos;

	openList.push_back(startPos);//1、将起点放入open中，即为等待处理的节点

	//主要是这块，把开始的节点放入openlist后开始查找旁边的8个节点
	//如果坐标超长范围或在closelist就return 
	//如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值 
	//如果原来的G值比较大（即新找到路径的g值比较小）则重新赋值G值 父节点 和f 
	//如果是新节点 加入到openlist 直到opellist为空或找到终点
	while (openList.size() > 0)
	{
		current = openList[0]; 

		if (current->x == endPos->x && current->y == endPos->y)//如果当前点是目标点，搜索完成
		{
			cout << "find the path" << endl;
			printMap();
			printPath(current);
			openList.clear();
			closeList.clear();
			break;
		}
		//如果当前点不是目标点，就把其作为下一个父节点接着搜索
		NextStep(current); //2、周围的8个点放入open中，当前节点close掉
		closeList.push_back(current);
		openList.erase(openList.begin());
		sort(openList.begin(), openList.end(), compare);//3、按自定义compare方法排序，从open中得到fn最小的节点
	}
}

void Astar::checkPoit(int x, int y, Node* father, int g)
{
	if (x < 0 || x > row || y < 0 || y > col)  // 检查是否越界
		return;
	if (this->unWalk(x, y)) //相邻节点是不可通过的点，退出
		return;
	if (isContains(&closeList, x, y) != -1) // 如果这个相邻节点已经close掉，退出
		return;
	int index;
	if ((index = isContains(&openList, x, y)) != -1)  // 相邻节点在open列表里
	{
		Node *point = openList[index]; // 设相邻节点由point指向
		if (point->g > father->g + g)  // 判断新路径(由father->g + g)位置处的g是否更小（原每个位置都有一个g,pose->g）
		{
			point->father = father;
			point->g = father->g + g;
			point->f = point->g + point->h;
		}
	}
	else  //如果相邻点不在open列表中，则将其push_back到open列表中，并计算g、h
	{
		Node * point = new Node(x, y, father);
		countGHF(point, endPos, g);//地址传递
		openList.push_back(point);
	}
}

void Astar::NextStep(Node* current)
{
	// 检查当前点相邻且空置的栅格（不包括障碍物和close的节点）
	// 前后左右四个节点的带价值为10，WeightW
	checkPoit(current->x - 1, current->y, current, WeightW);//左
	checkPoit(current->x + 1, current->y, current, WeightW);//右
	checkPoit(current->x, current->y + 1, current, WeightW);//上
	checkPoit(current->x, current->y - 1, current, WeightW);//下
	// 四个对角的代价值为14，WeightWH
	checkPoit(current->x - 1, current->y + 1, current, WeightWH);//左上
	checkPoit(current->x - 1, current->y - 1, current, WeightWH);//左下
	checkPoit(current->x + 1, current->y - 1, current, WeightWH);//右下
	checkPoit(current->x + 1, current->y + 1, current, WeightWH);//右上
}

int Astar::isContains(vector<Node*>* Nodelist, int x, int y) // open或者close包含的点
{
	for (int i = 0; i < Nodelist->size(); i++)
	{
		if (Nodelist->at(i)->x == x && Nodelist->at(i)->y == y)
		{
			return i;  // 返回包含的点的索引值
		}
	}
	return -1;
}

void Astar::countGHF(Node* sNode, Node* eNode, int g)  //形参为两个节点，当前节点eNode和其实节点sNode，计算代价值
{
	int h = abs(sNode->x - eNode->x) + abs(sNode->y - eNode->y) * WeightW;//修改后的启发函数
	printf("h is = %d\n", h);
	//h的计算方式应该是下面一种，上面计算h是纵坐标差值乘以权重再加上横坐标的差值
	int dealt_x = abs(sNode->x - eNode->x);
	int dealt_y = abs(sNode->y - eNode->y);
    h = max(dealt_x, dealt_y) * WeightW;
	printf("new h is %d\n", h);
	int current_g = sNode->father->g + g; //当前的g，父节点的代价g加上当前节点到其父节点的代价
	int f = current_g + h;
	sNode->f = f;
	sNode->h = h;
	sNode->g = current_g;
}

bool Astar::compare(Node* n1, Node* n2)
{
	//printf("%d,%d",n1->f,n2->f);
	return n1->f < n2->f; //返回较小的那个
}

bool Astar::unWalk(int x, int y)
{
	if (map[x][y] == 1)  // 不可通过的点的值为1
		return true;
	return false;
}

void Astar::printPath(Node* current)
{
	if (current->father != NULL)
		printPath(current->father);//递归调用
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