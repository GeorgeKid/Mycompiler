# include<stdio.h>
#include <iostream>  
#include <stdlib.h>
#include <string.h>
#include <list>   
using namespace std;

typedef struct tab{
	char* name;//标识符名字 
	char* object;//种类：const, var, procedure, function 
	char* type;//具体类型：integer, char, array; procedure和function无type
	int ref;//为数组时记录数组信息向量表atab中的位置,指针值，其余一向为0 
	int lev;//表示该标识符所在分程序的静态层次，主程序为1, 逐层加1
	int normal;//当标识符为变量形参时填入0, 形参或其他变量应填入1
	int adr;//var类型的应该填入运行栈中的相对地址，const类型应该填入相应的数值，procedure和function应该填入口地址 
	int link;//指向上一个标识符在tab中的位置 
}Tab;

typedef list<Tab> Tablist;
typedef list<Tab>::iterator TablistItrator;

int main()
{
	//初始化 
	Tab word("a","var","char",0,1,1,0,0);
	//存入list表后面
	Tablist.posh_back(word);
	
	return 0;
	
}

