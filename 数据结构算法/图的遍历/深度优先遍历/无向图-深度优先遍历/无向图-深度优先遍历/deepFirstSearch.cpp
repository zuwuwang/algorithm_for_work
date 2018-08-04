#include <stdio.h>
#include <Windows.h>

int book[101], sum, n, array[101][101];

void dfs(int curNode)
{
	for (int i = 1; i <= n; i++)
	{
		if (array[curNode][i] == 1 && book[i] == 0)
		{
			printf("%d",i);
			book[i] = 1;
			dfs(i);
		}
	}
	return;
}

int main()
{
	int m, a, b;
	printf("请输入节点数和边数，以空格分隔：");
	scanf_s("%d %d", &n,&m);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (i == j)
				array[i][j] = 0;
			else
				array[i][j] = 99999;
		}
	}
	
	for (int i = 1; i <= m; i++)
	{
		printf("请输入相互连接的边：a b，空格分隔,a,b的值要比n小：");
		scanf_s("%d %d", &a, &b);
		array[a][b] = 1;
		array[b][a] = 1;
	}
	int curNode;
	printf("请输入触发的顶点：");
	scanf_s("%d", &curNode);
	book[curNode] = 1;
	printf("输出节点的访问顺序：");
	printf("%d", curNode);
	dfs(curNode);
	system("pause");
}