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
	
	int index;//Ϊ����ʱ����¼�����±꣬��Ϊ���飬��¼Ϊ-1;Ϊ�����͹��̵���ʱ����¼����������±� 
	int lev;//��ʾ�ñ�ʶ�����ڷֳ���ľ�̬��Σ�������Ϊ1, ����1
	
}Word;

//��Ԫʽ 
typedef struct op{
	char operation[50];//���������
	char result[50];//���
	char data1[50];
	char data2[50]; 
}Op;

typedef struct array{
	char* type;//integer��char 
	int length;//���鳤�� 
	
}Array; 

typedef struct formol{
	int num;//�������� 
	char* type[10];//�������� 
	char* name[10];//�������� 
}Formol;

typedef struct activity{
	char funProName[50];//��ǰִ�к�������
	char name[10][50];//ʵ������ 
	int startAddr;//������ʼִ�е�λ�ã����ڵݹ���� 
	int retAddr;
	int lev;
}Act;

//�����Ķ���
int wordNum=0;//��ʵ�ʱ����
int symNum=0;//ȡ�ʵ�ʱ����
char sym[100];//�﷨����ʱ����ȡÿ����
int maxNum=10;//����0���β������Ĵ���
int T=0;//�Ĵ����ı��ֵ
int Label=0;//label�ı�ǩ�� 
char str2[25];//Tתstr����ʱ���� 
char str1[25]="T";
char str3[50];

//���庯�����ص�ַ��ջ
int retAddr[50];
 
int stay=0;//case

Word tab[1000];//�������ű� 
int tabIndex=0;

Array atab[100];//���������
int atabIndex=0;

Formol fortab[100];//����ÿһ�������Ĳ����б�
int fortabIndex=0; 
 
Act acts[10];//�����ջ 
int actIndex=0;

Op ops[100];//������Ԫʽ��
int opIndex=0;

char ch;//���ʱ��
char token[1000];//���ʱ��
int adr=0;//Ĭ�Ͽ�ʼʱ�ĵ�ַ��0 
int lev=0;//Ĭ��������Ϊ1 

char *reserves[]={"const","var","array","integer","char","procedure","function","for","if","then"
 ,"else","case","of","downto","to","do","begin","end","read","write"};

char symbols [600][20];
char values [600][20];

//��ջ 
void intoAct()
{
	int i=0,j=0;
	for(i=actIndex;i>=0;i--)
	{
		strcpy(acts[i].funProName,acts[i-1].funProName);
		acts[i-1].funProName[0]='\0';
		
		acts[i].retAddr=acts[i-1].retAddr;
		acts[i-1].retAddr=0;
		
		acts[i].lev=acts[i-1].lev;
		acts[i-1].lev=0;
		
		acts[i].startAddr=acts[i-1].startAddr;
		acts[i-1].startAddr=0;
		
		for(j=0;j<10;j++)
		{
			strcpy(acts[i].name[j],acts[i-1].name[j]);
			acts[i-1].name[j][0]='\0';
		}
	}
}

//��ջ
void outAct()
{
	int i=0,j=0;
	for(i=0;i>actIndex;i++)
	{
		acts[i].funProName[0]='\0';
		strcpy(acts[i].funProName,acts[i+1].funProName);
		
		acts[i].retAddr=0;
		acts[i].retAddr=acts[i-1].retAddr;
		
		acts[i].lev=0;
		acts[i].lev=acts[i+1].lev;
		
		acts[i].startAddr=0;
		acts[i].startAddr=acts[i+1].startAddr;
		
		for(j=0;j<10;j++)
		{
			acts[i].name[j][0]='\0';
			strcpy(acts[i].name[j],acts[i+1].name[j]);
		
		}
	}
} 

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
	int i=0,t=0,num=0,j=0,k=0;
	int sign=0;
	char* temp;
	i=start;
	
	lev=acts[0].lev;
	
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
		else if(strcmp(values[i],"(")==0)//���ں������β� , index������¼�ڼ����β�
		{
			j=0; 
			while(strcmp(values[i],")")!=0)
			{
				
				i++;
				if(strcmp(symbols[i],"IDSY")==0)
				{
				
					tab[tabIndex].object=4;//���ࣺconst 0, var 1, procedure 2, function 3, parameter 4
					tab[tabIndex].name=values[i];//��ʶ������
					t=i;
					while(strcmp(values[t],"integer")!=0&&strcmp(values[t],"char")!=0)
					{
						t++;
					}
					if(strcmp(values[t],"integer")==0)
					{
						tab[tabIndex].type=0;//type��integer 0, char 1, array 2;
						tab[tabIndex].adr=adr;//�ӵ�һ����ֵΪ0�����ϼ�4 
						tab[tabIndex].index=j;
						tab[tabIndex].lev=lev;
			        	adr=adr+4;
			        	tabIndex++;
			        	j++;
			        
					}
					else if(strcmp(values[t],"char")==0)
					{
						tab[tabIndex].type=1;
						tab[tabIndex].adr=adr;//�ӵ�һ����ֵΪ0�����ϼ�4 
						tab[tabIndex].index=j;
						tab[tabIndex].lev=lev;
			        	adr=adr+4;
			        	tabIndex++;
			        	j++;
			        
					}
					
					
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
			
			k=i;
			while(strcmp(values[k],")")!=0)
			{
				k++;
				if(strcmp(symbols[k],"IDSY")==0)
				{
					fortab[fortabIndex].name[j]=values[k];
					t=k;
					while(strcmp(values[t],":")!=0) t++;
					fortab[fortabIndex].type[j]=values[t+1];
					j++;
				}
			}
			fortab[fortabIndex].num=j;
			fortabIndex++;
			
			
			
			tab[tabIndex].adr=i+1;//��¼����function�Ĳ����б�"("��λ�á� 
			tab[tabIndex].lev=lev;
			tab[tabIndex].index=fortabIndex;
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
			
			k=i;
			while(strcmp(values[k],")")!=0)
			{
				k++;
				if(strcmp(symbols[k],"IDSY")==0)
				{
					fortab[fortabIndex].name[j]=values[k];
					t=k;
					while(strcmp(values[t],":")!=0) t++;
					fortab[fortabIndex].type[j]=values[t+1];
					j++;
				}
			}
			fortab[fortabIndex].num=j;
			fortabIndex++;
			
			
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
	char data1[50];
	char data2[50];
	char op[50];
	
	expression();
	strcpy(data1,ops[opIndex-1].result);
	if(strcmp(sym,">")==0||strcmp(sym,">=")==0||strcmp(sym,"<")==0||strcmp(sym,"<=")==0||strcmp(sym,"=")==0||strcmp(sym,"<>")==0)
	{
		strcpy(op,sym);
		getsym();
		expression();
		
		strcpy(ops[opIndex].data1,data1);
		strcpy(ops[opIndex].data2,ops[opIndex-1].result);
		strcpy(ops[opIndex].operation,"MINUS");
		itoa(T,str2,10);
	    strcat(str3,str1);
	    strcat(str3,str2);
	    strcpy(ops[opIndex].result,str3);
	    str3[0]='\0';
		T++;
	    opIndex++;
	
	    strcpy(ops[opIndex].result,ops[opIndex-1].result);
	    strcpy(ops[opIndex].operation,"CON");
	    strcpy(ops[opIndex].data1,op);
	    strcpy(ops[opIndex].data2,"0");
	    opIndex++;
		
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
    char data1[50];
    char op[50];
    
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		
	    if(strcmp(sym,"-")==0)
		{
			strcpy(data1,"0");
			strcpy(op,"MINUS");
		}
		getsym();
		term();
		strcpy(ops[opIndex].data1,data1);
		strcpy(ops[opIndex].data2,ops[opIndex-1].result);
		strcpy(ops[opIndex].operation,op);
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
		str3[0]='\0';
		T++;
	    opIndex++;
		
	}else{
		term();
	} 
	
	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
		{
			strcpy(data1,ops[opIndex-1].result);
			if(strcmp(sym,"+")==0)
			{
				strcpy(op,"ADD");
				
			}else if(strcmp(sym,"-")==0)
			{
				strcpy(op,"MINUS");
			}
			getsym();
			term();
			strcpy(ops[opIndex].data1,data1);
	    	strcpy(ops[opIndex].data2,ops[opIndex-1].result);
	    	strcpy(ops[opIndex].operation,op);
	    	itoa(T,str2,10);
	    	strcat(str3,str1);
	    	strcat(str3,str2);
	    	strcpy(ops[opIndex].result,str3);
	    	str3[0]='\0';
	    	T++;
	        opIndex++;
		}else{
			break;
		}

	}



}

void conEle()//�����Ԫ��
{
	consty();
	if(strcmp(sym,":")!=0) error();
	else{
	    
		strcpy(ops[opIndex].data1,ops[stay].result);
		strcpy(ops[opIndex].data2,ops[opIndex-1].result);
		strcpy(ops[opIndex].operation,"MINUS");
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
		str3[0]='\0';
		T++;
	    opIndex++;
		
		strcpy(ops[opIndex].result,ops[opIndex-1].result);
	    strcpy(ops[opIndex].operation,"CON");
	    strcpy(ops[opIndex].data1,"=");
	    strcpy(ops[opIndex].data2,"0");
	    opIndex++;
		
		strcpy(ops[opIndex].operation,"LAB");
		itoa(Label,str2,10);
		strcpy(ops[opIndex].result,str2);
		Label++;
		opIndex++;
		
		getsym();
		statement();
		
	}
}

void conEle1()//�����Ԫ��
{
	consty();
	if(strcmp(sym,":")!=0) error();
	else{
	    
		strcpy(ops[opIndex].data1,ops[stay].result);
		strcpy(ops[opIndex].data2,ops[opIndex-1].result);
		strcpy(ops[opIndex].operation,"MINUS");
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
		str3[0]='\0';
		T++;
	    opIndex++;
		
		strcpy(ops[opIndex].result,ops[opIndex-1].result);
	    strcpy(ops[opIndex].operation,"CON");
	    strcpy(ops[opIndex].data1,"=");
	    strcpy(ops[opIndex].data2,"0");
	    opIndex++;
			
		getsym();
		statement();
		
	}
}

void consty()//����
{
	if(strcmp(sym,"+")==0||strcmp(sym,"-")==0)
	{
		if(strcmp(sym,"-")==0)
		{
		
			
			getsym();
			
			strcpy(ops[opIndex].operation,"BE");
			strcpy(ops[opIndex].data1,sym);
			itoa(T,str2,10);
	    	strcat(str3,str1);
	    	strcat(str3,str2);
	    	strcpy(ops[opIndex].result,str3);
	    	str3[0]='\0';
	    	T++;
	        opIndex++;
			
			strcpy(ops[opIndex].data1,ops[opIndex-1].result);
			strcpy(ops[opIndex].operation,"MINUS");
			strcpy(ops[opIndex].data2,sym);
			itoa(T,str2,10);
	    	strcat(str3,str1);
	    	strcat(str3,str2);
	    	strcpy(ops[opIndex].result,str3);
	    	str3[0]='\0';
	    	T++;
	        opIndex++;
		}
	
		     
	}else
	{
			
			strcpy(ops[opIndex].operation,"BE");
			strcpy(ops[opIndex].data1,sym);
			itoa(T,str2,10);
	    	strcat(str3,str1);
	    	strcat(str3,str2);
	    	strcpy(ops[opIndex].result,str3);
	    	str3[0]='\0';
	    	T++;
	        opIndex++;
	}

	getsym();

}

void parameter()//ʵ�ڲ�����
{
	int i=0,j=0;
	expression();
	strcpy(acts[0].name[j],ops[opIndex-1].result);
	j++;
	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,",")==0)
		{
			getsym();
			expression();
			strcpy(acts[0].name[j],ops[opIndex-1].result);
	        j++;
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
		strcpy(ops[opIndex].operation,"BE");
		strcpy(ops[opIndex].data1,sym);
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
	    str3[0]='\0';
		T++;
		opIndex++;
		
		strcpy(ops[opIndex].operation,"WR");
		strcpy(ops[opIndex].result,ops[opIndex-1].result);
		opIndex++;
		
		getsym();
		return 1;
	}else{
		return 0;
	}

	
}

void term()
{
	int i=0;
	char data1[50];
	char op[50];
	factor();

	for(i=0;i<maxNum;i++)
	{
		if(strcmp(sym,"*")==0||strcmp(sym,"/")==0)
		{
			strcpy(data1,ops[opIndex-1].result);
	        if(strcmp(sym,"*")==0)
	        {
	        	strcpy(op,"MUL");
			}
			else if(strcmp(sym,"/")==0)
			{
				strcpy(op,"DIV");
			}
			getsym();
			factor();
			strcpy(ops[opIndex].data1,data1);
	    	strcpy(ops[opIndex].data2,ops[opIndex-1].result);
	    	strcpy(ops[opIndex].operation,op);
	    	itoa(T,str2,10);
	        strcat(str3,str1);
	    	strcat(str3,str2);
	    	strcpy(ops[opIndex].result,str3);
	    	str3[0]='\0';
		    T++;
	    	opIndex++;
		}else{
			break;
		}
	}

	
}

void factor()
{
	int j=0,k=0,start=0;
	char data1[50];
	
	if(find(sym)==0||find(sym)==1||find(sym)==4)
	{
		strcpy(ops[opIndex].operation,"BE");
		if(find(sym)==4)
		{
			j=position(sym);
			k=tab[j].index;
			if(tab[j].lev==lev)
			{
				strcpy(ops[opIndex].data1,acts[0].name[k]);
				
			}else if(tab[j].lev==(lev-1))
			{
				strcpy(ops[opIndex].data1,acts[1].name[k]);
			}
		    
		 
		}else{
			strcpy(ops[opIndex].data1,sym);
		}
		
		strcpy(ops[opIndex].data2,"");//""��ʾû�� 
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
	    str3[0]='\0';
		T++;
		opIndex++;
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
	else if(find(sym)==3)//����������� 
	{
		start=position(sym);
		start=tab[start].adr;
		if(strcmp(sym,acts[0].funProName)==0)
		{
			intoAct();
			lev=lev+1;
			strcpy(acts[0].funProName,sym); 
			getsym();
			if(strcmp(sym,"(")==0)
			{
				getsym();
				parameter();//ʵ�ڲ�����
				
				acts[0].retAddr=symNum;
				acts[0].lev=acts[1].lev+1;
				acts[0].startAddr=opIndex;
				actIndex++;
				
				start=createTab(start);
				symNum=start;
				strcpy(sym,values[symNum]);
				symNum++;
				
				//��ת��Ԫʽ 
				strcpy(ops[opIndex].operation,"GOTO");
				itoa(acts[0].startAddr,str2,10);
				strcpy(ops[opIndex].data1,str2);
				opIndex++;
			
			}
     	}else{
     		
     		intoAct();
			lev=lev+1;	
			strcpy(acts[0].funProName,sym); 
			getsym();
			if(strcmp(sym,"(")==0)
			{
				getsym();
				parameter();//ʵ�ڲ�����
				
				acts[0].retAddr=symNum;
				acts[0].lev=acts[1].lev+1;
				acts[0].startAddr=opIndex;
				actIndex++;
				
				start=createTab(start);
				symNum=start;
				strcpy(sym,values[symNum]);
				symNum++;
				
				
				
				statement();
				
				deleteTab(lev);
				lev=lev-1;
				
				symNum=acts[0].retAddr;
				strcpy(sym,values[symNum-1]);
				
				//��ջ
				outAct(); 
			}
		 }
     	
	
			
		
	}
	else{//�޷������� 
	    strcpy(ops[opIndex].operation,"BE");
		strcpy(ops[opIndex].data1,sym);
		strcpy(ops[opIndex].data2,"");
		itoa(T,str2,10);
		strcat(str3,str1);
		strcat(str3,str2);
		strcpy(ops[opIndex].result,str3);
		str3[0]='\0';
		T++;
		opIndex++;
		getsym();
	}

	
   
}

int find (char* sym)//�����ű����Ƿ���ڸñ�ʶ�� , �Ӻ���ǰ���� 
{
	int i=tabIndex-1;
	for(i=tabIndex-1;i>=0;i--)
	{
		if(strcmp(tab[i].name,sym)==0)
		{
			return tab[i].object;
		}
	}
	
	return -1;
}

int position(char* sym)//�ҵ���ʶ���ڷ��ű��е�λ�� 
{
	int i=tabIndex-1;
	for(i=tabIndex-1;i>=0;i--)
	{
		if(strcmp(tab[i].name,sym)==0)
		{
			return i;
		}
	}
	
	return -1;
}

void statement()
{                                                                     
	int i=0,j=0,start=0,sig=0;//���ڼ���
	int k=0;//���ڼ��� 
	int num=0;
	int tempOp=0;//���ڼ�¼JUM����תֵ 
    char data1[50];
    char data2[50];
    char op[50];
    
    if(find(sym)==0||find(sym)==1||find(sym)==3||find(sym)==4)//��ֵ���  0 const, 1 var, 3 function 
    {
    	if(find(sym)==4)
    	{
    		j=position(sym);
    		j=tab[j].index;
			strcpy(data1,acts[0].name[j]); 
    		
		}else{
			strcpy(data1,sym);
		}
    	
    	getsym();
		if(strcmp(sym,":=")==0){
		
			getsym();
			if(find(sym)==3)
			{
			  sig=1;
			  strcpy(data2,sym);
			}
			expression();
			strcpy(ops[opIndex].result,data1);
			if(sig==1)
			{
				strcpy(ops[opIndex].data1,data2);
				sig=0;
			}else{
				
				strcpy(ops[opIndex].data1,ops[opIndex-1].result);
			}
			
			strcpy(ops[opIndex].operation,"BE");
			opIndex++;
			
		}
		else if(strcmp(sym,"[")==0){
			getsym();
			
			expression();
		    strcpy(data2,ops[opIndex-1].result);
		    
			if(strcmp(sym,"]")!=0) error();
			else{
				getsym();
				
				if(strcmp(sym,":=")!=0) error();
				else{
					getsym();
					expression();
					strcpy(ops[opIndex].result,data1);
					strcpy(ops[opIndex].data1,data2);
					strcpy(ops[opIndex].data2,ops[opIndex-1].result);
					strcpy(ops[opIndex].operation,"ABE");
					opIndex++;
				}

			}
			
		} 
	
	}
	else if(find(sym)==2)//���̵������ 
	{
		intoAct();
		lev=lev+1;
		start=position(sym);
		start=tab[start].adr;
		strcpy(acts[0].funProName,sym);
		getsym();
		
		if(strcmp(sym,"(")==0)
		{
			getsym();
			parameter();//ʵ�ڲ�����
			
		}
	 
	
		acts[0].retAddr=symNum;
		acts[0].lev=acts[1].lev+1;
		acts[0].startAddr=opIndex;
		actIndex++;
		
		
		start=createTab(start);
		symNum=start;
		strcpy(sym,values[symNum]);
		symNum++;
	
		statement();
		
		//ɾ�����ű� 
		deleteTab(lev);
		lev=lev-1;
		
		//�ظ���ǰλ�� 
		symNum=acts[0].retAddr;
		strcpy(sym,values[symNum-1]);
		//��ջ
		outAct(); 
		
		 
	}
	else if(strcmp(sym,"if")==0){//�������  label else�� 0, if�ṹ���� 1 , 
		getsym();
		condition();
		
		if(strcmp(sym,"then")!=0) error();
		else{
			getsym();
			statement();
			
			if(strcmp(sym,"else")==0)
			{
				strcpy(ops[opIndex].operation,"JUM");
		        tempOp=opIndex;
		        opIndex++;
			}
			
			if(strcmp(sym,"else")==0){
				strcpy(ops[opIndex].operation,"LAB");
				itoa(Label,str2,10);
				strcpy(ops[opIndex].result,str2);
				Label++;
				opIndex++;
				getsym();
				statement();
				
			}
			
			strcpy(ops[opIndex].operation,"LAB");
			itoa(Label,str2,10);
			strcpy(ops[opIndex].result,str2);
			strcpy(ops[tempOp].result,str2);
			Label++;
			opIndex++;
			
		}

	}
	else if(strcmp(sym,"case")==0){//������
	 
		getsym();
		
		expression();
		
		stay=opIndex-1;
		strcpy(data1,ops[opIndex-1].result);
		
		if(strcmp(sym,"of")!=0) error();
		else{
			getsym();
			conEle1();
			
			strcpy(ops[opIndex].operation,"JUM");
		    tempOp=opIndex;
			opIndex++;
			
			k=1;
			for(i=0;i<maxNum;i++)
			{
				if(strcmp(sym,";")==0)
				{
					getsym();
					conEle();
					
					strcpy(ops[opIndex].operation,"JUM");
		        	k++;
		        	opIndex++;
				}
				else{
					break;
				}
			}
			if(strcmp(sym,"end")!=0) error();
			else getsym();
			
			
		}
		
		strcpy(ops[opIndex].operation,"LAB");
		itoa(Label,str2,10);
		strcpy(ops[opIndex].result,str2);
		for(i=0;i<k;i++)
		{
			strcpy(ops[tempOp+i].result,str2);
		}
		Label++;
		opIndex++;
	
	}
	else if(strcmp(sym,"for")==0){//forѭ����� 
		getsym();
		
		
		if(find(sym)==0||find(sym)==1)
		{
			strcpy(data1,sym);
			getsym();
			
		}else error();
		
		if(strcmp(sym,":=")!=0) error();
		else{
			getsym();
			expression();
			strcpy(ops[opIndex].result,data1);
			strcpy(ops[opIndex].data1,ops[opIndex-1].result);
			strcpy(ops[opIndex].operation,"BE");
			opIndex++;
			
			
			if(strcmp(sym,"downto")!=0&&strcmp(sym,"to")!=0) error();
			else{
				getsym();
				expression();
				if(strcmp(sym,"downto")==0)
				{
				   strcpy(ops[opIndex].data1,data1);
				   strcpy(ops[opIndex].data2,ops[opIndex-1].result);
			    }else{
			       strcpy(ops[opIndex].data2,data1);
				   strcpy(ops[opIndex].data1,ops[opIndex-1].result);
				}
			    
			    strcpy(ops[opIndex].operation,"MINUS");
			    itoa(T,str2,10);
                strcat(str3,str1);
                strcat(str3,str2);
                strcpy(ops[opIndex].result,str3);
                str3[0]='\0';
	            T++;
                opIndex++;
			
			    num=opIndex;
				strcpy(ops[opIndex].result,ops[opIndex-1].result);
	            strcpy(ops[opIndex].operation,"CON");
	            strcpy(ops[opIndex].data1,">=");
	            strcpy(ops[opIndex].data2,"0");
	            opIndex++;
				
				strcpy(data1,ops[opIndex-1].result);
				
				strcpy(ops[opIndex].operation,"LAB");
				itoa(opIndex,str2,10);
				strcpy(ops[opIndex].result,str2);
				strcpy(ops[opIndex].data1,"0");
				opIndex++;
				
				
				
				
				if(strcmp(sym,"do")!=0) error();
				else{
					getsym();
					statement();
					
					strcpy(ops[opIndex].operation,"MINUS");
			     	strcpy(ops[opIndex].data1,data1);
			    	strcpy(ops[opIndex].data2,"1");
			    	strcpy(ops[opIndex].result,data1);
			    	opIndex++;
					
					strcpy(ops[opIndex].operation,"GOTO");
					itoa(num,str2,10);
			    	strcpy(ops[opIndex].result,str2);
					opIndex++;
				}
				
				strcpy(ops[opIndex].operation,"LAB");
				itoa(opIndex,str2,10);
				strcpy(ops[opIndex].result,str2);
				strcpy(ops[opIndex].data1,"1");
				opIndex++;
				
			}
		}
		//printf("this is a for statement.\n");
	}
	else if(strcmp(sym,"begin")==0){//�������  
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
				strcpy(ops[opIndex].operation,"BE");
				strcpy(ops[opIndex].data1,sym);
				itoa(T,str2,10);
				strcat(str3,str1);
				strcat(str3,str2);
				strcpy(ops[opIndex].result,str3);
			    str3[0]='\0';
				T++;
				opIndex++;
				
				strcpy(ops[opIndex].operation,"RE");
				strcpy(ops[opIndex].result,ops[opIndex-1].result);
				opIndex++;
				
				getsym();
				
				
			}else error();
			for(i=0;i<maxNum;i++)
			{
				if(strcmp(sym,",")==0)
				{
					getsym();
					if(find(sym)==0||find(sym)==1)
					{
						strcpy(ops[opIndex].operation,"BE");
						strcpy(ops[opIndex].data1,sym);
						itoa(T,str2,10);
						strcat(str3,str1);
						strcat(str3,str2);
						strcpy(ops[opIndex].result,str3);
					    str3[0]='\0';
						T++;
						opIndex++;
						
						strcpy(ops[opIndex].operation,"RE");
						strcpy(ops[opIndex].result,ops[opIndex-1].result);
						opIndex++;
						
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
			if(find(sym)==-1&&string())//���ַ����趨Ϊint���ͣ��ȴ������з���ֵ
			{
				if(strcmp(sym,",")==0)
				{
					getsym();
					expression();
					
					strcpy(ops[opIndex].operation,"WR");
			    	strcpy(ops[opIndex].result,ops[opIndex-1].result);
			    	opIndex++;
					
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
				
				strcpy(ops[opIndex].operation,"WR");
			    strcpy(ops[opIndex].result,ops[opIndex-1].result);
			    opIndex++;
				
				if(strcmp(sym,")")!=0) error();
				else getsym();
			}
		}
		//printf("this is a write statement.\n");

	}
	else{//�����  
	   if(strcmp(sym,"end")==0||strcmp(sym,";")==0)
	   {
	   	
	   }else error();
	}

}
 
int main()
{

    int i=0,start=0;
    char c[50]="a";
    i=c[0];
    
//   

    printf("%d",i);
    
	return 0;
	
}

