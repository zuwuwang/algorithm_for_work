#include <stdio.h>
#include <Windows.h>

int a[101], n;

void swap(int left,int right)
{
	int temp;
	temp = a[right];
	a[right] = a[left];
	a[left] = temp;
}
void quickSort(int left,int right)
{
	int pivot,i,j;
	if (left > right)
		return;
	pivot = a[left];
	i = left;
	j = right;
	// 核心比较算法
	while (i != j)
	{
		while (a[j] >= pivot && i < j)
			j--;
		while (a[i] <= pivot && i < j)
			i++;
		if (i < j)
		{
			swap(i, j);
		}
	}	
	swap(left, i);
	// 递归调用
	quickSort(left, i - 1);  // 左边子序列排序
	quickSort(i+1,right);
}

int main()
{
	int i, n;
	printf("输入元素总个数：");
	scanf("%d", &n);
	printf("依次输入各个元素的值");
	for (i = 1; i <= n; i++)
	{
		scanf("%d", &a[i]);
	}
	quickSort(1, n);

	for (i = 1; i <= n; i++)
	{
		printf("%d\t", a[i]);
	}
	system("pause");
	return 0;
}