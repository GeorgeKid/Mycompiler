#include<stdio.h>
#include <stdlib.h>
#include <string.h>

//�������� 
void error();
void expression();
void consty();
void term();
void factor();
void statement();
//�������� 
typedef struct word{
	char* name;//��ʶ������ 
	int adr;//var���͵�Ӧ����������ջ�е���Ե�ַ��const����Ӧ��������Ӧ����ֵ��procedure��functionӦ�������(begin)��ַ 
	int object;//���ࣺconst 0, var 1, procedure 2, function 3 
	int type;//�������ͣ�integer 0, char 1, array 1; procedure��function��type -1
	
	int index;//Ϊ����ʱ����¼�����±꣬��Ϊ���飬��¼Ϊ-1 
	int lev;//��ʾ�ñ�ʶ�����ڷֳ���ľ�̬��Σ�������Ϊ1, ����1
	
}Word;

//��Ԫʽ 
typedef struct op{
	char* operation;//���������
	char* result;//���
	char* data1;
	char* data2; 
}Op;

typedef struct array{
	char* type;//integer��char 
	int length;//���鳤�� 
	
}Array; 

//�����Ķ���
int wordNum=0;//��ʵ�ʱ����
int symNum=0;//ȡ�ʵ�ʱ����
char sym[100];//�﷨����ʱ����ȡÿ����
int maxNum=10;//����0���β������Ĵ���
int T=0;//�Ĵ����ı��ֵ 

Word tab[1000];//�������ű� 
int tabIndex=0;

Array atab[100];//���������
int atabIndex=0; 

Op ops[100];//������Ԫʽ��
int opIndex=0;

char ch;//���ʱ��
char token[1000];//���ʱ��
int adr=0;//Ĭ�Ͽ�ʼʱ�ĵ�ַ��0 
int lev=1;//Ĭ��������Ϊ1 

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

   FILE *fp1;//�����ļ���ָ�룬���ڴ򿪶�ȡ���ļ�
   FILE *fp2;//�����ļ���ָ�룬���ڴ�д�������ļ�

   char text[1024];//����һ���ַ������飬���ڴ洢��ȡ���ַ�

   fp1 = fopen("C:\\Users\\George\\Desktop\\Compile\\13061146_test1.txt","r");//ֻ����ʽ���ļ�a.txt
   fp2 = fopen("C:\\Users\\George\\Desktop\\Compile\\13061146_compileResult.txt","w");//д��ʽ���ļ�a.txt

   while(fgets(text,1024,fp1)!=NULL)//���ж�ȡfp1��ָ���ļ��е����ݵ�text��
   {


        for(i=0;i<strlen(text);i++)
        {

            while(isSpaceOrNewlineOrTab(text[i])) i++;//�������ַ�

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
			else if(text[i]==39)//����
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
    	fprintf(fp2,"%d %s %s\n", i,&symbols[i],&values[i]);
	}


   fclose(fp1);//�ر��ļ�a.txt
   fclose(fp2);//�ر��ļ�b.txt

}

int createTab(int start)//��¼�µ�ǰ��εķ��ű�������������ʼ��λ�� 
{
	int i=0,t=0,num=0;
	int sign=0;
	char* temp;
	i=start;
	while(((strcmp(values[i],"end")==0&&strcmp(values[i+1],".")==0)||(strcmp(values[i],"end")==0&&strcmp(values[i+1],";")==0))!=1)
	{
		if(i<wordNum)
		{
		
		if(strcmp(values[i],"const")==0)
		{
			while(strcmp(values[i],";")!=0)
			{
				i++;
				if(strcmp(symbols[i],"IDSY")==0&&strcmp(symbols[i+1],"EQUSY")==0)
				{
					tab[tabIndex].object=0;//���ࣺconst 0, var 1, procedure 2, function 3
					tab[tabIndex].name=values[i];//��ʶ������
					i=i+2;
					if(strcmp(values[i],"'")==0)
					{
						tab[tabIndex].type=1;//type��integer 0, char 1, array 2; procedure��function��type -1
						tab[tabIndex].adr=atoi(values[i+1]);//��ʶ����ֵ	
					}else if(strcmp(values[i],"+")==0||strcmp(values[i],"-")==0){
						tab[tabIndex].type=0;
						temp=values[i];
						strcat(temp,values[i+1]);
						tab[tabIndex].adr=atoi(temp);//��ʶ����ֵ 
						
					}else{
						tab[tabIndex].type=0;
						tab[tabIndex].adr=atoi(values[i]);//��ʶ����ֵ
					}
					
					tab[tabIndex].lev=lev;
			    	tab[tabIndex].index=-1;
			    	
			    	tabIndex++;
					
				}
				
			}
			
				
				
		}
		else if(strcmp(values[i],"var")==0)
		{
			while(strcmp(values[i],"function")!=0&&strcmp(values[i],"procedure")!=0&&strcmp(values[i],"begin")!=0)
			{
				i++;
				if(strcmp(symbols[i],"IDSY")==0)
				{
				
					tab[tabIndex].object=1;//���ࣺconst 0, var 1, procedure 2, function 3 
					tab[tabIndex].name=values[i];//��ʶ������
					t=i;
					while(strcmp(values[t],"integer")!=0&&strcmp(values[t],"char")!=0&&strcmp(values[t],"array")!=0)
					{
						t++;
					}
					if(strcmp(values[t],"integer")==0)
					{
						tab[tabIndex].type=0;//type��integer 0, char 1, array 2;
						tab[tabIndex].adr=adr;//�ӵ�һ����ֵΪ0�����ϼ�4 
						tab[tabIndex].index=-1;
						tab[tabIndex].lev=lev;
			        	adr=adr+4;
			        	tabIndex++;
			        
					}
					else if(strcmp(values[t],"char")==0)
					{
						tab[tabIndex].type=1;
						tab[tabIndex].adr=adr;//�ӵ�һ����ֵΪ0�����ϼ�4 
						tab[tabIndex].index=-1;
						tab[tabIndex].lev=lev;
			        	adr=adr+4;
			        	tabIndex++;
			        
					}
					else if(strcmp(values[t],"array")==0)
					{
						tab[tabIndex].type=2;//array 2
						tab[tabIndex].lev=lev;
						while(strcmp(symbols[t],"INTSY")!=0)
						{
							t++;
						}
						
						tab[tabIndex].adr=adr;
						
						num=atoi(values[t]);
						
						adr=adr+4*num;
						
						atab[atabIndex].length=num;
						tab[tabIndex].index=atabIndex;
						
						while(strcmp(values[t],"integer")!=0&&strcmp(values[t],"char")!=0)
						{
							t++;
						}
					
						atab[atabIndex].type=values[t];
						
						atabIndex++;
						tabIndex++;
					}
					
				}
				
			}
			i--;//��һ��function��procedure. 
		}
		else if(strcmp(values[i],"function")==0)
		{
			i++;
			tab[tabIndex].name=values[i];//��¼���������� 
			tab[tabIndex].object=3;//3��ʾfunction
			t=i;
			while(strcmp(values[t],"begin")!=0&&strcmp(values[t],"const")!=0&&strcmp(values[t],"var")!=0&&strcmp(values[t],"procedure")!=0&&strcmp(values[t],"function")!=0)
			{
				t++;
			}
			if(strcmp(values[t-2],"integer")==0)
			{
				tab[tabIndex].type=0;
			}
			else if(strcmp(values[t-2],"char")==0)
			{
				tab[tabIndex].type=1;
			}
			
			tab[tabIndex].adr=i+1;//��¼����function�Ĳ����б�"("��λ�á� 
			tab[tabIndex].lev=lev;
			tab[tabIndex].index=-1;
			tabIndex++;
			
			sign=1;
			while(sign!=0)
			{
				i++;
				if(strcmp(values[i],"function")==0||strcmp(values[i],"procedure")==0)
				{
					sign++;
					
				}else if(strcmp(values[i],"end")==0&&strcmp(values[i+1],";")==0)
				{
					sign--;
				}
			}
			
		}
		else if(strcmp(values[i],"procedure")==0)
		{
			i++;
			tab[tabIndex].name=values[i];//��¼���������� 
			tab[tabIndex].object=2;//2��ʾprocedure
			tab[tabIndex].adr=i+1;//��¼����function�Ĳ����б�"("��λ�á� 
			tab[tabIndex].lev=lev;
			tab[tabIndex].index=-1;
			tabIndex++;
			
	
			sign=1;
			while(sign!=0)
			{
				i++;
				if(strcmp(values[i],"function")==0||strcmp(values[i],"procedure")==0)
				{
					sign++;
				}else if(strcmp(values[i],"end")==0&&strcmp(values[i+1],";")==0)
				{
					sign--;
				}
			}
			
		}else if(strcmp(values[i],"begin")==0)
		{
			return i;
		}
		
		i++;
	}
	else{
		break;
	}
	}
	
	return i;
	
} 

void getsym()
{
	strcpy(sym,values[symNum]);
	symNum++;
}

void deleteTab(int lev)//��һ�����ʱ�������һ��ı��� 
{
	int i=0,num=tabIndex;
	
	for(i=0;i<num;i++)
	{
		if(tab[i].lev==lev)
		{
			tab[i].name[0]='\0';
			tab[i].adr=0;
			tab[i].object=0;
			tab[i].index=0;
			tab[i].type=0;
			tab[i].lev=0;
			tabIndex--;
		}
	}
}

void condition()//����
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
	//printf("this is a condition\n");
}

void error()//������
{
	printf("Test %s %d",&sym,symNum);
	printf("syntex error!\n");//��ʱ��ͳ����
}

void expression()
{
    int i=0;
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		printf("the operation is %s\n",sym);
		getsym();
	}
	term();
	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
		{
			printf("the operation is %s\n",sym);
			getsym();
			term();
		}else{
			break;
		}

	}


	//printf("this is a expression.\n");
}

void conEle()//�����Ԫ��
{
	consty();
	if(strcmp(sym,":")!=0) error();
	else{
		getsym();
		statement();
	}
}

void consty()//����
{
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		getsym();
	}

	getsym();

	//printf("this is a const\n");
}

void parameter()//ʵ�ڲ�����
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
	if(strcmp(sym,")")!=0) error();
	else getsym();
	
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

	//printf("this is a string.\n");
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

	//printf("this is a term.\n");
}

void factor()
{
	if(find(sym)==0||find(sym)==1)
	{
		printf("the word is %s\n",sym);
		getsym();
		if(strcmp(sym,"[")==0)
		{
			getsym();
			expression();
			if(strcmp(sym,"]")!=0) error();
			else getsym();
		}
	}
	else if(strcmp(sym,"(")==0)
	{
		getsym();
		
		expression();
		
		if(strcmp(sym,")")!=0) error();
		else getsym();
	}
	else if(find(sym)==3)
	{
		getsym();
		if(strcmp(sym,"(")==0)
		{
			getsym();
			parameter();//ʵ�ڲ�����
		
			//printf("this is a function statement.\n");
			
		}
	}
	else{//���޷������� 
	    printf("the word is %s\n",sym);
		getsym();
	}

	//printf("this is a factor.\n");

}

int find (char* sym)
{
	int i=0;
	for(i=0;i<tabIndex;i++)
	{
		if(strcmp(tab[i].name,sym)==0)
		{
			return tab[i].object;
		}
	}
	
	return -1;
}

void statement()
{                                                                     
	int i=0;//���ڼ���
    
    if(find(sym)==0||find(sym)==1||find(sym)==3)//��ֵ���  0 const, 1 var, 3 function ok
    {
    	getsym();
		if(strcmp(sym,":=")==0){
			getsym();
			expression();
		}
		else if(strcmp(sym,"[")==0){
			getsym();
			
			expression();
			
			if(strcmp(sym,"]")!=0) error();
			else{
				getsym();
				
				if(strcmp(sym,":=")!=0) error();
				else{
					getsym();
					expression();
				}

			}
			
		} 
		//printf("this is a assignment statement.\n");
	}
	else if(find(sym)==2)//���̵������  ok
	{
		getsym();
		
		if(strcmp(sym,"(")==0)
		{
			getsym();
			parameter();//ʵ�ڲ�����
			//printf("this is a procedure statement.\n");
			
		}
		else error();
		 
	}
	else if(strcmp(sym,"if")==0){//������� ok
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

		//printf("this is a if statement.\n");

	}
	else if(strcmp(sym,"case")==0){//������ ok
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
		//printf("this is a case statement.\n");
	}
	else if(strcmp(sym,"for")==0){//forѭ����� ok
		getsym();
		
		if(find(sym)==0||find(sym)==1)
		{
			getsym();
		}else error();
		
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
		//printf("this is a for statement.\n");
	}
	else if(strcmp(sym,"begin")==0){//�������  ok
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
     	
     	if(strcmp(sym,"end")!=0) error();
     	else getsym();

     	//printf("this is a fuhe statement.\n");
	}


	else if(strcmp(sym,"read")==0)//����� ok
	{
		getsym();
		if(strcmp(sym,"(")!=0) error();
		else{
			getsym();
			if(find(sym)==0||find(sym)==1)
			{
				getsym();
			}else error();
			for(i=0;i<maxNum;i++)
			{
				if(strcmp(sym,",")==0)
				{
					getsym();
					if(find(sym)==0||find(sym)==1)
					{
						getsym();
					}else error();
				}else{
					break;
				}
			}
		    if(strcmp(sym,")")!=0) error();
     	    else getsym();
		}
		//printf("this is a read statement.\n");

	}
	else if(strcmp(sym,"write")==0)//д���  ok
	{
		getsym();
		if(strcmp(sym,"(")!=0) error();
		else{
			getsym();
			if(string())//���ַ����趨Ϊint���ͣ��ȴ������з���ֵ
			{
				if(strcmp(sym,",")==0)
				{
					getsym();
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
		//printf("this is a write statement.\n");

	}
	else{//�����  ok
	   if(strcmp(sym,"end")==0||strcmp(sym,";")==0)
	   {
	   	
	   }else error();
	}

}

int main()
{
//	int i=0,start=0;
//	createSym();
//	start=createTab(0);
//	//deleteTab(1);
//	for(i=0;i<tabIndex;i++)
//	{
//		printf("%s %d %d %d %d %d\n",tab[i].name,tab[i].adr,tab[i].object,tab[i].type,tab[i].lev,tab[i].index);
//	}
    int i=0,start=0;
    createSym();
    start=createTab(0);
    symNum=start;
	getsym();
	statement();
    
	
	
}

