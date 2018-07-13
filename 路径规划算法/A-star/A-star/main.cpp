#include "Astar.h"
#include <tchar.h>
#include <Windows.h>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	Astar astar;
	Node *startPos = new Node(5, 1);
	Node *endPos = new Node(3, 8);
	astar.search(startPos, endPos);
	system("pause");
	return 0;
}