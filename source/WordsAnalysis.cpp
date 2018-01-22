#include "a_word.h"
#include "a.h"
#include <math.h>

////�����ļ�·��
string InputFilePath()
{
	string Path;
	cout<<"Input filename which is under current path:"<<endl;
	cin>>Path;
    return Path;
}

//�õ��ļ���
void GetFileStream(string Path,string &FileStream)
{
    ifstream file;
	int n=Path.length();
	char *filename=new char[n+1];
	filename[n]='\0';
	for (int i=0;i<n;i++)
	{
		filename[i]=Path[i];
	}
	file.open(filename);//��һ��������֧��string�࣬��֧���ַ�����
	if(!file.is_open())//����ļ��Ƿ��
	{
		cout<<"Can not open file "<<Path<<endl;
		return;
	}
	char c;
	file.get(c);
	while (!file.eof())
	{
		FileStream+=c;
		file.get(c);
	}
	delete[] filename;// �ͷ�������ڴ�
	file.close();

}



//�ѽ�������ַ���Delimiters��
void	ReadFromFiles(string & Delimiters)
{
    ifstream file;//�ѽ�������ַ�����
	file.open("Delimiters.txt",ios::in);
	if (!file.is_open())
	{
		cout<<"Can not open file Delimiters.txt"<<endl;
		return;
	}
    char c;
	while (!file.eof())
	{
		file.get(c);
		if (c!=' '&&c!='\n')
		{
			Delimiters+=c;
		}
	}
	file.close();

}
//�ؼ��ֵ����ַ�������A.Data[]��
void	ReadFromFiles(Merger & A,int MaxLength,string filename)
{
    ifstream file2;//�ڶ�ȡ�ڶ����ļ���ʱ��ʹ����ͬ��������file��ȡ�Ǵ����
	//�ѹؼ��ֵ����ַ�������Keys.Data[]��

	file2.open(filename.c_str(),ios::in);
	if (!file2.is_open())
	{
		cout<<"Can not open file "<<filename<<endl;
		return;
	}
	char k[20];
	k[19]='\0';
	A.length=0;
	while (!file2.eof()&&A.length<=MaxLength)
	{
		file2.getline(k,20);
		A.Data[A.length++]=k;
	}
	file2.close();

}



//���뺯��,�ѽṹ������β��
void Insert(group_Main *&Head,group_Main *&Nail,group_Main *a)//ע��ָ������ں�������Ҳ��Ҫ�ı�Ļ����������õ���ʽ
{
	if (NULL==Head)
	{
	    a->ID=1;
	    a->SubStatementTableID=0;//��ʼ��Ϊ��
	    Head=a;
		a->next=NULL;
		Nail=a;
	}
	else
	{
	    a->ID=Nail->ID+1;
	    a->SubStatementTableID=0;//��ʼ��Ϊ��
        Nail->next=a;
		a->next=NULL;
		Nail=a;
	}
}

//����һ����㲢���ݲ����Խ�㸳ֵ�Ͳ���������
void ApplyAndInsert(group_Main *&Head,group_Main *&Nail,string value,int type,int row)
{
	group_Main *a=new group_Main;
	a->value=value;a->type=type;
	a->row=row;
	Insert(Head,Nail,a);//���뺯��,����β��

}
//�͹ؼ��ֱȽϣ�����1˵���ǹؼ��֣������Ǳ�ʶ��
int isMatch(string t,Merger Keys)
{
	int flag=0;
	for (int i=0;i<Keys.length;i++)
	{
		if (t==Keys.Data[i])
		{
			flag=1;
			break;
		}
	}
	return flag;
}
//�ж��Ƿ��ǹؼ��ֲ��ѽ������β��; ��һ���Ѿ�����ĸ
void JudgeFromKeysAndIde(string FileStream,int &pos,int StreamLength,Merger Keys,group_Main *&Head,group_Main *&Nail,int &row)
{
    int count=0;
	string t="";
	do
	{
		t=t+FileStream[pos++];    //string ��������+,���Դ���һ���ַ�
		count++;
	} while ((isalpha(FileStream[pos])||isdigit(FileStream[pos])||'_'==FileStream[pos])&&count<IdentifierMax&&pos<StreamLength);//��ʶ���IdentifierMax
	if (count>IdentifierMax)//̫��
	{
		while(FileStream[pos]!=' '&&FileStream[pos]!='\n'&&pos<StreamLength)//����¼IdentifierMaxλ
		{
			pos++;
		}
		ApplyAndInsert(Head,Nail,t,Identifier,row);
	}
	else
	{
		if(isMatch(t,Keys))//�͹ؼ��ֱȽϣ�����1˵���ǹؼ��֣������Ǳ�ʶ��
		{
			ApplyAndInsert(Head,Nail,t,Key,row);
		}
		else
		{
			ApplyAndInsert(Head,Nail,t,Identifier,row);
		}
	}
}
 //����
void ReportError(int type,int row)
{

}
//ɾȥ���õ��ַ�;
void FilterUselessChar(string FileStream,int &pos)
{

}
//ƴ��   ֻ��Ҫ������������ɵ��ַ���
string MakeupNumber(string FileStream,int &pos,int row)
{

    double a=0;
    while (isdigit(FileStream[pos])&&pos<FileStream.length())
    {
        a=a*10+FileStream[pos]-48;
		pos++;
    }
    if(FileStream[pos]=='.')
    {
        double b=0;
        pos++;
        int count=0;
        if(!isdigit(FileStream[pos]))
        {
            //����
            ReportError(Number,row);
            //ɾȥ���õ��ַ�;
            FilterUselessChar(FileStream,pos);
        }
        else
        {
            while(isdigit(FileStream[pos])&&pos<FileStream.length())
            {
                count++;
                b=b+(FileStream[pos]-48)*pow(0.1,count);//
                pos++;
            }
            a=a+b;
        }
    }
    if(FileStream[pos]=='e')
	{

		int count=0;
		char c;
		pos++;
		if(isdigit(FileStream[pos])||FileStream[pos]=='+'&&pos<FileStream.length())////����10��������
		{
		    if(FileStream[pos]=='+') pos++;
            if(!isdigit(FileStream[pos]))
            {
            //����
                ReportError(Number,row);
            //ɾȥ���õ��ַ�;
                FilterUselessChar(FileStream,pos);
            }
            else
            {
                while (isdigit(FileStream[pos])&&pos<FileStream.length())
                {
                    count=count*10+FileStream[pos]-48;//
                    pos++;
                }
                for(int i=0;i<count;i++)
                    a=a*10;
            }
		}
		else if(FileStream[pos]=='-'&&pos<FileStream.length())//����0.1��������
        {
            pos++;
            if(!isdigit(FileStream[pos]))
            {
            //����
                ReportError(Number,row);
            //ɾȥ���õ��ַ�;
                FilterUselessChar(FileStream,pos);
            }
            else
            {

                while (isdigit(FileStream[pos])&&pos<FileStream.length())
                {
                    count=count*10+FileStream[pos]-48;//
                    pos++;
                }
                for(int i=0;i<count;i++)
                    a=a*0.1;
            }
        }
        else//error;e������
        {
            //����
            ReportError(Number,row);
            //ɾȥ���õ��ַ�;
            FilterUselessChar(FileStream,pos);
        }
	}
    return NumberToString(a);

}

//�жϵ�i���Ƿ��Ǳ߽��
bool isDelimiter(string FileStream,int &pos,string Delimiters)
{
	for (int i=0;i<Delimiters.length();i++)
	{
		if (FileStream[pos]==Delimiters[i])
		{
			pos++;
			return true;
		}
	}

	return false;
}



//�ҵ����ƥ��������������
bool isOperator(string FileStream,int &pos,Merger  Operators,string &s)
{
  	s="";
	string t;
	int count=0;
	for (int i=0;i<Operators.length;i++)
	{
		int j=0;
        int k=pos;
        t="";
		while(FileStream[k]==(Operators.Data[i])[j]&&k<FileStream.length()&&j<(Operators.Data[i]).length())
		{
			t+=FileStream[k];
			k++;
			j++;
		}
		if (s.length()<t.length())
		{
			s=t;
		}
	}
	if (s.length()!=0)
	{
		pos+=s.length();
		return true;
	}
	else
	{
        return false;
	}
}

#define Horizontal_table 9 //�Ʊ��
//����
struct group_Main *Sort(string FileStream)
{
	struct group_Main *Head=NULL,*Nail=NULL;
    string Delimiters;
	Merger Keys,Operators;
	int row=1;
	ReadFromFiles(Delimiters);//�ѽ�������ַ���Delimiters��
	ReadFromFiles(Keys,BigMax,"Keys.txt");//�ؼ��ֵ����ַ�������Keys.Data[]��
	ReadFromFiles(Operators,BigMax,"Operators.txt");//����������ַ�������Operators.Data[]��
	int pos=0;
	while (pos<FileStream.length())
    {

		if (FileStream[pos]==' '||FileStream[pos]=='\r'||FileStream[pos]==Horizontal_table)   //�ų��ո񡢻��С��Ʊ��
		{
		    pos++;
		}
		else if (FileStream[pos]=='\n')//�س��������������
		{
			pos++;
			row++;
		}
		else
		{
            if (isalpha(FileStream[pos])) //����ĸ
            {
				JudgeFromKeysAndIde(FileStream,pos,FileStream.length(),Keys,Head,Nail,row);//�ж��Ƿ��ǹؼ��ֲ������������������β��
			}
			else if (isdigit(FileStream[pos]))//������
			{
				ApplyAndInsert(Head,Nail,MakeupNumber(FileStream,pos,row),Number,row);
			}
			else
			{
				string s;
				if(isDelimiter(FileStream,pos,Delimiters)) //�ж��Ƿ��Ǳ߽�� ,�߽��ֻ��һ���ַ�
				{
					char c[2];
					c[1]='\0';c[0]=FileStream[pos-1];//����Ǳ߽������ô��ʱFileStream[pos]Ӧ���Ǳ߽������һ���ַ�
				    ApplyAndInsert(Head,Nail,c,Delimiter,row);
				}
				else if(isOperator(FileStream,pos,Operators,s)) //�����  ���������������ҵ����ƥ��������������s�� ע�⣺�������н�"//","/*"��"*/"
				{	                                             //��Ϊ��������д���
					if ("//"==s)
					{
						while (FileStream[pos]!='\n'&&pos<FileStream.length())
						{
							pos++;
						}
					}
					else if ("/*"==s)
					{
						while(1)
						{
							while (FileStream[pos]!='*'&&pos<FileStream.length())
							{
                                if ('\n'==FileStream[pos])
                                {
									row++;
                                }
								pos++;
							}
							if (isOperator(FileStream,pos,Operators,s))
							{
								if ("*/"==s)
								{
									break;
								}
							}
						}
					}
					else
					{
						ApplyAndInsert(Head,Nail,s,Operator,row);
					}
				}
				else// # ,....
				{
				    string t;
					t=FileStream[pos++];
					ApplyAndInsert(Head,Nail,t,Key,row);
				}
			}

		}


	}

/*	group_Main *p=Head;
	while (p!=NULL)
	{
		cout<<"value "<<setw(15)<<setiosflags(ios_base::left)<<p->value<<"  type="<<setw(1)<<p->type<<"  row="<<setw(5)<<p->row<<endl;
		p=p->next;
	}
*/	return Head;

}
//������ת�����ַ���
template <class T>
string NumberToString(T v)
{
     ostringstream os;
     os<<v;
     return os.str();
}

//�����ո�
string CreatSpace(int n)
{
    if(n<=0) return string("");
    string s;
    char *t=new char[n+1];
    t[n]='\0';
    for(int i=0;i<n;i++)
        t[i]=' ';
    s=t;
    delete[] t;
    return s;
}


//���ļ���д����ʾ��Ϣ,Ҳ������1--5����ʲô���Ͳ�д��value ..type ...row����ʾ��Ϣ
void PutAnnouncementForUser(ofstream &file)
{
    string Notice;
    string Del;
    Del=NumberToString(Delimiter);Del.append(".Delimiter");
    string K;
    K=NumberToString(Key);K.append(".Key");
    string Ope;
    Ope=NumberToString(Operator);Ope.append(".Operator");
    string Num;
    Num=NumberToString(Number);Num.append(".Number");
    string Ide;
    Ide=NumberToString(Identifier);Ide.append(".Identifier");
    string t;
    t="ID"+CreatSpace(SmallSpaceNumber-2)+"value"+CreatSpace(MaxSpaceNumber-5)+"type"+CreatSpace(SmallSpaceNumber-4)+"row"+CreatSpace(SmallSpaceNumber-3)+"StatementID";
    Notice=Del+CreatSpace(5)+K+CreatSpace(5)+Ope+CreatSpace(5)+Num+CreatSpace(5)+Ide+'\n'+t+'\n';//ע�⣺ÿ��ϵͳ���н�β�����ܲ�һ�����������Ǹ��û��ģ�����Ҫ���¶����ڴ棬������Ӱ��
 //   cout<<Notice<<endl;
 //   file.write(reinterpret_cast<char *>(&Notice),sizeof Notice);//Ϊ��ʹ����������ļ���ͷ�����һЩ���룿
    for(int i=0;i<Notice.length();i++)
    {
        file.put(Notice[i]);
    }
}

//д������
void PutDataForUser(ofstream &file,group_Main *Head)
{
    group_Main *p=Head;
    string t;
    while(NULL!=p)
    {
        t=NumberToString(p->ID)+CreatSpace(SmallSpaceNumber-NumberToString(p->ID).length())+p->value+CreatSpace(MaxSpaceNumber-(p->value).length())+NumberToString(p->type)+CreatSpace(SmallSpaceNumber-1)+NumberToString(p->row)+CreatSpace(SmallSpaceNumber-NumberToString(p->row).length())+NumberToString(p->SubStatementTableID)+'\n';
        for(int i=0;i<t.length();i++)
        {
            file.put(t[i]);
        }
            p=p->next;
    }
}


//�ѷ���Ľ��д���ļ�Words_User.txt;
void WriteToFileForUser(struct group_Main *Head)
{
	if (NULL!=Head)
	{
		ofstream file;
		file.open("Words_User.txt");//�ı����
		if(!file.is_open())//����ļ��Ƿ��
		{
			cout<<"Can not creat file "<<"Words_User.txt"<<endl;
			return;
		}
        PutAnnouncementForUser(file);//���ļ���д����ʾ��Ϣ,Ҳ������1--5����ʲô���Ͳ�д��һ�С�value     type       row   ������ʾ��Ϣ
		PutDataForUser(file,Head);
		file.close();
	}

}
void DeleteLink(struct group_Main *Head)//ɾ����̬����Ľ��
{
	if (NULL!=Head)
	{
		group_Main *p=Head;
		Head=Head->next;
		while (Head!=NULL)
		{
			delete p;
			p=Head;
			Head=Head->next;
		}
		delete p;
	}
}





