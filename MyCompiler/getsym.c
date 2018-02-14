#include <stdio.h>
#include <stdlib.h>

 int wordNum=0;
 char ch;
 char token[1000];

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

main()
{
   int i=0,index=0,j=0;
   token[0]='\0';
   FILE *fp1;//定义文件流指针，用于打开读取的文件
   FILE *fp2;//定义文件流指针，用于打开写操作的文件

   char text[1024];//定义一个字符串数组，用于存储读取的字符

   fp1 = fopen("C:\\Users\\George\\Desktop\\Compile\\13061146_test.txt","r");//只读方式打开文件a.txt
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


        //puts(text);//输出到屏幕
        //fputs(text,fp2);//将内容写到fp2所指向文件中
   }

    for(i=0;i<wordNum;i++)
    {
    	fprintf(fp2,"%d %s %s\n", (i+1),&symbols[i],&values[i]);
	}


   fclose(fp1);//关闭文件a.txt
   fclose(fp2);//关闭文件b.txt




}
