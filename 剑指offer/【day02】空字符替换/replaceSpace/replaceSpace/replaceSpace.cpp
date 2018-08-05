#include <iostream>

using namespace std;

// length为原始字符串的数组容量
void replaceSpace(char *str, int length)
{
	// 初始时候对输入做检查
	if (str == NULL || length < 0)
		return;
	// 输入合法，开始下一步
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
	char a[100] = "we are happy."; // 加空格和\0总共14个字符。
	replaceSpace(a, 100);
	printf("%c", a);
	system("pause");
	return;
}