/*****************************13-4.c *******************************/
#pragma warning(disable:4996)
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int deleteSpace(string &str)
{
	string::iterator first = str.begin();
	string::iterator last = str.end();
	int count = 0;
	for (string::iterator itor = str.begin(); itor < str.end(); itor++)
	{
		while (*itor == ' ')
		{
			itor++;
			count++;
			if (*itor != ' ')
			{
				first = itor - count;
				last = itor - 1;
				str.erase(first, last);
				count = 0;
				//����ʹ������position��count��ɾ��
			}
		}
	}
	return 1;
}

int reverseStr(string &str, int begin, int end)   //�ַ�����β�ַ��ߵ�˳��
{
	char tmp;
	if (str.empty())
	{
		return 0;
	}
	if (begin<end)
	{
		tmp = str[begin];  //�ռ临�Ӷ�ΪO(1)
		str[begin] = str[end];
		str[end] = tmp;
		reverseStr(str, begin + 1, end - 1);
	}
	return 1;
}


int reverseWords(string &str)
{
	int wordBegin = 0, wordEnd = 0, len = 0;
	if (str.empty())
	{
		return 0;
	}
	len = str.size();
	reverseStr(str, 0, len - 1);
	while (wordEnd<len)
	{
		if (str[wordEnd] != ' ') {
			wordBegin = wordEnd;
			while (str[wordEnd] != ' ' && str[wordEnd] != '\0' && wordEnd<len)
			{
				wordEnd++;
			}
			wordEnd--;
			reverseStr(str, wordBegin, wordEnd);  //�Կո�Ϊ�з֣���ÿ���ո�֮������ַ������е���
		}
		wordEnd++;
	}
	return 1;
}


int main()
{
	string str;
	getline(cin, str);
	if (str[0] == ' ' || str[str.size()] == ' ')
	{
		str.erase(0, 1);
		str.erase(str.size() - 1);
	}
	// �޳�����ո�
	deleteSpace(str);
	reverseWords(str);
	cout << str << endl;
	system("Pause");
	return 0;
}