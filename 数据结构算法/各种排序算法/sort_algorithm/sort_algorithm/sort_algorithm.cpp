/**********
*
*    �ο�����https://www.cnblogs.com/onepixel/articles/7674659.html
*
*************/
#include <iostream>

//ð������-1����ǰ���󣬴��������
// lenΪ���鳤��	
// ���鳤�ȿ���ͨ��sizeof����
// len = sizeof(arr) / sizeof(arr[0])

int* bubbleSort(int *arr,int len)  
{
	int i = 0, j = 0;
	for (i = 0; i < len - 1; i++)  //���ѭ������
	{
		for (j = 0; j < len - 1 - i; j++) //�ڲ�ÿ��ѭ���ȽϵĴ���
		{
			if (arr[j] > arr[j + 1]) //������ź���
			{
				int tmp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	return arr;
}

//ð������-2���Ӻ���ǰ�Ƚϣ������ð�ݵ�ͷ��
int* bubbleSort_2(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = len; j > i; j++) //�������������ʼ�Ƚ�
		{
			if (arr[j]>arr[j - 1])
			{
				int tmp = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	return arr;
}

// ð��������Ժ�����������������������Ϊ����
void TestBubbleSort(int*(buttleSort)(int* arr, int len))
{
	
}

// ѡ������
// ������δ����������Ѱ����������С��Ԫ�أ��ŵ����е���ʼλ��
// Ȼ����Ŵ�������Ѱ����������С��Ԫ��
// �Ȱѳ�ʼֵ���ó�Ϊ���ֵ������Сֵ
int* slectSort(int* arr, int len)
{
	int minIndex;
	for (int i = 0; i < len - 1; i++)
	{
		minIndex = i;
		for (int j = 1; j < len; j++)
		{
			if (arr[j]<arr[minIndex])
			{
				minIndex = j;
			}
		}
		//ÿ�αȽ���ɺ󽫵õ�����Сֵ�ŵ�arr[i]��
		int tmp = arr[i];
		arr[i] = arr[minIndex];
		arr[minIndex] = tmp;
	}
	return arr;
}


int mian()
{

	return 0;
}