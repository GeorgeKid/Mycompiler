# include<stdio.h>
#include <iostream>  
#include <stdlib.h>
#include <string.h>
#include <list>   
using namespace std;

typedef struct tab{
	char* name;//��ʶ������ 
	char* object;//���ࣺconst, var, procedure, function 
	char* type;//�������ͣ�integer, char, array; procedure��function��type
	int ref;//Ϊ����ʱ��¼������Ϣ������atab�е�λ��,ָ��ֵ������һ��Ϊ0 
	int lev;//��ʾ�ñ�ʶ�����ڷֳ���ľ�̬��Σ�������Ϊ1, ����1
	int normal;//����ʶ��Ϊ�����β�ʱ����0, �βλ���������Ӧ����1
	int adr;//var���͵�Ӧ����������ջ�е���Ե�ַ��const����Ӧ��������Ӧ����ֵ��procedure��functionӦ������ڵ�ַ 
	int link;//ָ����һ����ʶ����tab�е�λ�� 
}Tab;

typedef list<Tab> Tablist;
typedef list<Tab>::iterator TablistItrator;

int main()
{
	//��ʼ�� 
	Tab word("a","var","char",0,1,1,0,0);
	//����list�����
	Tablist.posh_back(word);
	
	return 0;
	
}

