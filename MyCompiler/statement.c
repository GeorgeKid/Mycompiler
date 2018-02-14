#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//函数声明 
void error();
void expression();
void consty();
void term();
void factor();
void statement();

 int wordNum=0;//存词的时候用
 int symNum=0;//取词的时候用
 char sym[100];//语法分析时用于取每个词
 int maxNum=10;//处理0或多次不超过的次数

 char ch;//存词时用
 char token[1000];//存词时用

 char *reserves[]={"const","var","array","integer","char","procedure","function","for","if","then"
 ,"else","case","of","downto","to","do","begin","end","read","write"};

 char symbols [600][20];
 char values [600][20];

int isSpaceOrNewlineOrTab(char ch)
{
    if(ch==' '||ch=='\n'||ch=='\t')
        return 1;
    else
        return 0;
}

int isLetter(char ch)
{
    if((ch>64&&ch<91)||(ch>96&&ch<123))
        return 1;
    else
        return 0;
}

int isDigit(char ch)
{

    if(ch>47&&ch<58)
        return 1;
    else
        return 0;
}

int transNum(char* s)
{
    int i=0,t=0,result=0;

    if(strlen(s)>0)
    {
          for(i=0; i<=strlen(s)-1;i++)
        {
            result=result*10+(s[i]-48);
        }
    }

    return result;
}

void clearToken(char* token)
{

    memset( token, '\0', sizeof(token) );
    token[0]='\0';
}

int reserver(char* s)
{
    int i=0,length=0;

    length=sizeof(reserves)/sizeof(reserves[0]);

    for(i=0;i<length;i++)
    {
        if(strcmp(s,reserves[i])==0)
        {
            return 1;
        }
    }

    return 0;


}


void createSym()
{
   int i=0,index=0,j=0;
   token[0]='\0';

   FILE *fp1;//定义文件流指针，用于打开读取的文件
   FILE *fp2;//定义文件流指针，用于打开写操作的文件

   char text[1024];//定义一个字符串数组，用于存储读取的字符

   fp1 = fopen("C:\\Users\\George\\Desktop\\Compile\\13061146_test1.txt","r");//只读方式打开文件a.txt
   fp2 = fopen("C:\\Users\\George\\Desktop\\Compile\\13061146_compileResult.txt","w");//写方式打开文件a.txt

   while(fgets(text,1024,fp1)!=NULL)//逐行读取fp1所指向文件中的内容到text中
   {


        for(i=0;i<strlen(text);i++)
        {

            while(isSpaceOrNewlineOrTab(text[i])) i++;//跳过空字符

            if(isLetter(text[i]))
            {
                while(isLetter(text[i])||isDigit(text[i]))
                {
                    token[index]=text[i];
                    i++;
                    index++;
                }
                token[index]='\0';
                strcpy(values[wordNum],token);

                if(reserver(token))
                    strcpy(symbols[wordNum],"RESERVE");
                else
                    strcpy(symbols[wordNum],"IDSY");

                wordNum++;
                index=0;
                clearToken(token);
                i--;//back one step

            }
            else if(isDigit(text[i]))
            {

                while(isDigit(text[i]))
                {

                    token[index]=text[i];
                    i++;
                    index++;
                }
                token[index]='\0';
                strcpy(values[wordNum],token);
                strcpy(symbols[wordNum],"INTSY");

                wordNum++;
                index=0;
                clearToken(token);
                i--;//back one step

            }
            else if (text[i]==':')
            {
        	token[index]=text[i];
        	 index++;
        	 i++;
        	if(text[i]=='=')
        	{
        		token[index]='=';
        		i++;
        		index++;
        		strcpy(symbols[wordNum],"ASSIGNSY");

			}else{
				strcpy(symbols[wordNum],"COLONSY");
			}
             token[index]='\0';
			strcpy(values[wordNum],token);
			 wordNum++;
             index=0;
             clearToken(token);
             i--;//back one step

			}
			else if(text[i]=='=')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"EQUSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='+')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"PLUSSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='-')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"MINUSSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='*')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"STARSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='/')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"DIVISY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='(')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"LPARSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]==')')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"RPARSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]==',')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"COMMASY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]==';')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"SEMISY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='|')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"SHU");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='[')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"LZHONG");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]==']')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"RZHONG");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='.')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"ENDSY");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]==39)//引号
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"YIN");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='{')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"LDA");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='}')
			{
				token[index]=text[i];
				index++;
                token[index]='\0';
				strcpy(symbols[wordNum],"RDA");
				strcpy(values[wordNum],token);
				wordNum++;
	            index=0;
	            clearToken(token);
			}
			else if(text[i]=='<')
			{
				token[index]=text[i];
				i++;
				index++;

				if(text[i]=='=')
				{
					token[index]=text[i];
					i++;
					index++;
                    token[index]='\0';
					strcpy(symbols[wordNum],"XIAODENG");
				}else if(text[i]=='>')
				{
					token[index]=text[i];
					i++;
					index++;
                    token[index]='\0';
					strcpy(symbols[wordNum],"NOTEQUSY");
				}else{
                    token[index]='\0';
					strcpy(symbols[wordNum],"XIAO");
				}

				strcpy(values[wordNum],token);

				wordNum++;
	            index=0;
	            clearToken(token);
	            i--;
			}
			else if(text[i]=='>')
			{
				token[index]=text[i];
				i++;
				index++;

				if(text[i]=='=')
				{
					token[index]=text[i];
					i++;
					index++;
					token[index]='\0';
					strcpy(symbols[wordNum],"DADENG");

				}else{
				    token[index]='\0';
					strcpy(symbols[wordNum],"DA");
				}

				strcpy(values[wordNum],token);

				wordNum++;
	            index=0;
	            clearToken(token);
	            i--;
			}


        }


   }

    for(i=0;i<wordNum;i++)
    {
    	fprintf(fp2,"%d %s %s\n", (i+1),&symbols[i],&values[i]);
	}


   fclose(fp1);//关闭文件a.txt
   fclose(fp2);//关闭文件b.txt

}

void getsym()
{
	strcpy(sym,values[symNum]);
	symNum++;
}

void condition()//条件
{
	expression();
	if(strcmp(sym,">")==0||strcmp(sym,">=")==0||strcmp(sym,"<")==0||strcmp(sym,"<=")==0||strcmp(sym,"=")==0||strcmp(sym,"<>")==0)
	{
		getsym();
		expression();

	}else{
		error();
		getsym();
	}
	printf("this is a condition\n");
}

void error()//错误处理
{
	printf("Test %s %d",&sym,symNum);
	printf("syntex error!\n");//暂时笼统处理
}

void expression()
{
    int i=0;
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		getsym();
	}
	term();
	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
		{
			getsym();
			term();
		}else{
			break;
		}

	}


	printf("this is a expression.\n");
}

void conEle()//情况表元素
{
	consty();
	if(strcmp(sym,":")!=0) error();
	else{
		getsym();
		statement();
	}
}

void consty()//常量
{
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		getsym();
	}

	getsym();

	printf("this is a const\n");
}

void var()//标识符
{
   getsym();//暂时不做处理
}

void parameter()//实在参数表
{
	int i=0;
	expression();
	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,",")==0)
		{
			getsym();
			expression();
			
		}else{
			break;
		}
	}
	
}

int string()
{
	if(sym[0]==32||sym[0]==33||(sym[0]>35&&sym[0]<126))
	{
		getsym();
		return 1;
	}else{
		return 0;
	}

	printf("this is a string.\n");
}

void term()
{
	int i=0;
	factor();

	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,"*")==0||strcmp(sym,"/")==0)
		{
			getsym();
			factor();
		}else{
			break;
		}
	}

	printf("this is a term.\n");
}

void factor()
{
	if(strcmp(sym,"(")==0)
	{
		getsym();
		//printf("testfact1 %s %d",&sym,symNum);
		expression();
		//printf("testfact2 %s %d",&sym,symNum);
		if(strcmp(sym,")")!=0) error();
		else getsym();
	}

	else{
		//printf("testfact1 %s %d\n",&sym,symNum);
		var();
		//printf("testfact2 %s %d\n",&sym,symNum);
		if(strcmp(sym,"[")==0)
		{
			getsym();
			expression();
			if(strcmp(sym,"]")!=0) error();
			else getsym();
		}
		else if(strcmp(sym,"(")==0)
		{
			getsym();
			parameter();//实在参数表
			//printf("testfactRight %s %d\n",&sym,symNum);
			if(strcmp(sym,")")!=0) error();
			else getsym();
		}
	

	}

	printf("this is a factor.\n");

}

void statement()
{                                                                     
	int i=0;//用于计数

	if(strcmp(sym,"if")==0){//条件语句 
		getsym();
		condition();
		if(strcmp(sym,"then")!=0) error();
		else{
			getsym();
			statement();
			if(strcmp(sym,"else")==0){
				getsym();
				statement();
			}
		}

		printf("this is a if statement.\n");

	}
	else if(strcmp(sym,"case")==0){//情况语句 
		getsym();
		//printf("testcase1 %s %d",&sym,symNum);
		expression();
		//printf("testcase2 %s %d",&sym,symNum);
		if(strcmp(sym,"of")!=0) error();
		else{
			getsym();
			conEle();
			for(i=0;i<maxNum;i++)
			{
				if(strcmp(sym,";")==0)
				{
					getsym();
					conEle();
				}
				else{
					break;
				}
			}
			if(strcmp(sym,"end")!=0) error();
			else getsym();
		}
		printf("this is a case statement.\n");
	}
	else if(strcmp(sym,"for")==0){//for循环语句 
		getsym();
		var();//标识符处理
		if(strcmp(sym,":=")!=0) error();
		else{
			getsym();
			expression();
			if(strcmp(sym,"downto")!=0&&strcmp(sym,"to")!=0) error();
			else{
				getsym();
				expression();
				if(strcmp(sym,"do")!=0) error();
				else{
					getsym();
					statement();
				}
			}
		}
		printf("this is a for statement.\n");
	}
	else if(strcmp(sym,"begin")==0){//复合语句 
		getsym();
        statement();
		for(i=0;i<maxNum;i++)
		{
			if(strcmp(sym,";")==0)
			{
				getsym();
				statement();
			}
			else{
				break;
			}
     	}
     	
     	//printf("testfuhe %s %d",&sym,symNum);
     	if(strcmp(sym,"end")!=0) error();
     	else getsym();

     	if(strcmp(sym,";")!=0) error();
     	else getsym();
        
     	printf("this is a fuhe statement.\n");
	}


	else if(strcmp(sym,"read")==0)//读语句 
	{
		getsym();
		if(strcmp(sym,"(")!=0) error();
		else{
			getsym();
			var();//标识符分析
			for(i=0;i<maxNum;i++)
			{
				if(strcmp(sym,",")==0)
				{
					getsym();
					var();
				}else{
					break;
				}
			}
		    if(strcmp(sym,")")!=0) error();
     	    else getsym();
		}
		printf("this is a read statement.\n");

	}
	else if(strcmp(sym,"write")==0)//写语句 
	{
		getsym();
		if(strcmp(sym,"(")!=0) error();
		else{
			getsym();
			if(string())//将字符串设定为int类型，既处理又有返回值
			{
				if(strcmp(sym,",")==0)
				{
					expression();
					if(strcmp(sym,")")!=0) error();
					else getsym();
				}
				else if(strcmp(sym,")")==0)
				{
					getsym();
				}
				else error();
			}else{
				expression();
				if(strcmp(sym,")")!=0) error();
				else getsym();
			}
		}
		printf("this is a write statement.\n");

	}

	else{
	    if(strcmp(sym,"end")==0)
	    {
	    	var();//标识符处理
		}
		
		
		if(strcmp(sym,":=")==0){
			getsym();
			
			expression();
			printf("this is a assignment statement.\n");

		}
		else if(strcmp(sym,"[")==0){
			getsym();
			//printf("testassign1 %s %d",&sym,symNum);
			expression();
			//printf("testassign2 %s %d",&sym,symNum);
			if(strcmp(sym,"]")!=0) error();
			else{
				getsym();
				//printf("testassign3 %s %d",&sym,symNum);
				if(strcmp(sym,":=")!=0) error();
				else{
					getsym();
					expression();
				}

			}

			printf("this is a assignment statement.\n");
		}
		else if(strcmp(sym,"(")==0){
			getsym();
			parameter();//实在参数表
			if(strcmp(sym,")")!=0) error();
			else getsym();
			printf("this is a procedure statement.\n");
		}


	}






}




void main()
{
	createSym();
	getsym();
	while(symNum<=wordNum)
	{
		if(strcmp(sym,"const")==0)
		{
			getsym();
			while(strcmp(sym,";")!=0) getsym();
			getsym();
			printf("this is a constdeclaration.\n");
		}
		else if(strcmp(sym,"var")==0)
		{
			getsym();

			while(strcmp(sym,";")!=0) getsym();
			getsym();
			while(strcmp(sym,";")!=0) getsym();
			getsym();
			while(strcmp(sym,";")!=0) getsym();
			getsym();

			printf("this is a vardeclaration.\n");
		}
		else if(strcmp(sym,"function")==0)
		{
			getsym();
			while(strcmp(sym,";")!=0) getsym();
			getsym();
			printf("this is a fundeclaration.\n");
		}
		else if(strcmp(sym,"procedure")==0)
		{
			getsym();
			while(strcmp(sym,";")!=0) getsym();
			getsym();
			printf("this is a prodeclaration.\n");
		}
		else{
			statement();
		}
    }
}







