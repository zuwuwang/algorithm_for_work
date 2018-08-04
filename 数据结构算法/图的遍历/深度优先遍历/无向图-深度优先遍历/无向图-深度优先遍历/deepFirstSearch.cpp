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
	printf("������ڵ����ͱ������Կո�ָ���");
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
		printf("�������໥���ӵıߣ�a b���ո�ָ�,a,b��ֵҪ��nС��");
		scanf_s("%d %d", &a, &b);
		array[a][b] = 1;
		array[b][a] = 1;
	}
	int curNode;
	printf("�����봥���Ķ��㣺");
	scanf_s("%d", &curNode);
	book[curNode] = 1;
	printf("����ڵ�ķ���˳��");
	printf("%d", curNode);
	dfs(curNode);
	system("pause");
}