/**********
*
*    参考链接https://www.cnblogs.com/onepixel/articles/7674659.html
*
*************/
#include <iostream>

//冒泡排序-1，从前往后，大的数沉底
// len为数组长度	
// 数组长度可以通过sizeof来求
// len = sizeof(arr) / sizeof(arr[0])

int* bubbleSort(int *arr,int len)  
{
	int i = 0, j = 0;
	for (i = 0; i < len - 1; i++)  //外层循环次数
	{
		for (j = 0; j < len - 1 - i; j++) //内层每次循环比较的次数
		{
			if (arr[j] > arr[j + 1]) //大的数放后面
			{
				int tmp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	return arr;
}

//冒泡排序-2，从后往前比较，最大数冒泡到头部
int* bubbleSort_2(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = len; j > i; j++) //从最后两个数开始比较
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

// 冒泡排序测试函数，测试样例，函数名作为类型
void TestBubbleSort(int*(buttleSort)(int* arr, int len))
{
	
}

// 选择排序
// 首先在未排序序列中寻找最大或者最小的元素，放到序列的起始位置
// 然后接着从序列中寻找最大或者最小的元素
// 先把初始值设置成为最大值或者最小值
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
		//每次比较完成后将得到的最小值放到arr[i]处
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