#include <iostream>

using namespace std;

// lengthΪԭʼ�ַ�������������
void replaceSpace(char *str, int length)
{
	// ��ʼʱ������������
	if (str == NULL || length < 0)
		return;
	// ����Ϸ�����ʼ��һ��
	int i=0;
	int spaceNum=0;
	int charNum=0;
	while (str[i] != '\0')
	{
		charNum++;
		if (str[i] == ' ')
			spaceNum++;
		i++;
	}
	int originalLength = charNum;
	int newLength = spaceNum * 2 + originalLength;
	if (newLength > length)
		return;
	while (originalLength>=0 && newLength>originalLength)
	{
		if (str[originalLength] == ' ')
		{
			str[newLength--] = '0';
			str[newLength--] = '2';
			str[newLength--] = '%';
		}
		else
		{
			str[newLength--] = str[originalLength];
		}
		originalLength--;
	}
}

void main()
{
	char a[100] = "we are happy."; // �ӿո��\0�ܹ�14���ַ���
	replaceSpace(a, 100);
	printf("%c", a);
	system("pause");
	return;
}