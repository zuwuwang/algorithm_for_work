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
	// ���ıȽ��㷨
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
	// �ݹ����
	quickSort(left, i - 1);  // �������������
	quickSort(i+1,right);
}

int main()
{
	int i, n;
	printf("����Ԫ���ܸ�����");
	scanf("%d", &n);
	printf("�����������Ԫ�ص�ֵ");
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