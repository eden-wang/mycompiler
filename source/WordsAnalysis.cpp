#include "a_word.h"
#include "a.h"
#include <math.h>

////输入文件路径
string InputFilePath()
{
	string Path;
	cout<<"Input filename which is under current path:"<<endl;
	cin>>Path;
    return Path;
}

//得到文件流
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
	file.open(filename);//第一个参数不支持string类，但支持字符数组
	if(!file.is_open())//检测文件是否打开
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
	delete[] filename;// 释放申请的内存
	file.close();

}



//把界符导入字符串Delimiters中
void	ReadFromFiles(string & Delimiters)
{
    ifstream file;//把界符导入字符串中
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
//关键字导入字符串数组A.Data[]中
void	ReadFromFiles(Merger & A,int MaxLength,string filename)
{
    ifstream file2;//在读取第二个文件的时候使用相同的流对象file读取是错误的
	//把关键字导入字符串数组Keys.Data[]中

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



//插入函数,把结构体链在尾部
void Insert(group_Main *&Head,group_Main *&Nail,group_Main *a)//注意指针如果在函数体中也需要改变的话必须是引用的形式
{
	if (NULL==Head)
	{
	    a->ID=1;
	    a->SubStatementTableID=0;//初始化为零
	    Head=a;
		a->next=NULL;
		Nail=a;
	}
	else
	{
	    a->ID=Nail->ID+1;
	    a->SubStatementTableID=0;//初始化为零
        Nail->next=a;
		a->next=NULL;
		Nail=a;
	}
}

//申请一个结点并根据参数对结点赋值和插入链表中
void ApplyAndInsert(group_Main *&Head,group_Main *&Nail,string value,int type,int row)
{
	group_Main *a=new group_Main;
	a->value=value;a->type=type;
	a->row=row;
	Insert(Head,Nail,a);//插入函数,链在尾部

}
//和关键字比较，返回1说明是关键字；否则是标识符
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
//判断是否是关键字并把结果链在尾部; 第一个已经是字母
void JudgeFromKeysAndIde(string FileStream,int &pos,int StreamLength,Merger Keys,group_Main *&Head,group_Main *&Nail,int &row)
{
    int count=0;
	string t="";
	do
	{
		t=t+FileStream[pos++];    //string 类重载了+,可以串接一个字符
		count++;
	} while ((isalpha(FileStream[pos])||isdigit(FileStream[pos])||'_'==FileStream[pos])&&count<IdentifierMax&&pos<StreamLength);//标识符最长IdentifierMax
	if (count>IdentifierMax)//太长
	{
		while(FileStream[pos]!=' '&&FileStream[pos]!='\n'&&pos<StreamLength)//最多记录IdentifierMax位
		{
			pos++;
		}
		ApplyAndInsert(Head,Nail,t,Identifier,row);
	}
	else
	{
		if(isMatch(t,Keys))//和关键字比较，返回1说明是关键字；否则是标识符
		{
			ApplyAndInsert(Head,Nail,t,Key,row);
		}
		else
		{
			ApplyAndInsert(Head,Nail,t,Identifier,row);
		}
	}
}
 //报错
void ReportError(int type,int row)
{

}
//删去无用的字符;
void FilterUselessChar(string FileStream,int &pos)
{

}
//拼数   只需要返回数字所组成的字符串
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
            //报错
            ReportError(Number,row);
            //删去无用的字符;
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
		if(isdigit(FileStream[pos])||FileStream[pos]=='+'&&pos<FileStream.length())////乘以10的整数倍
		{
		    if(FileStream[pos]=='+') pos++;
            if(!isdigit(FileStream[pos]))
            {
            //报错
                ReportError(Number,row);
            //删去无用的字符;
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
		else if(FileStream[pos]=='-'&&pos<FileStream.length())//乘以0.1的整数倍
        {
            pos++;
            if(!isdigit(FileStream[pos]))
            {
            //报错
                ReportError(Number,row);
            //删去无用的字符;
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
        else//error;e后无数
        {
            //报错
            ReportError(Number,row);
            //删去无用的字符;
            FilterUselessChar(FileStream,pos);
        }
	}
    return NumberToString(a);

}

//判断第i个是否是边界符
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



//找到最大匹配的运算符并返回
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

#define Horizontal_table 9 //制表符
//分类
struct group_Main *Sort(string FileStream)
{
	struct group_Main *Head=NULL,*Nail=NULL;
    string Delimiters;
	Merger Keys,Operators;
	int row=1;
	ReadFromFiles(Delimiters);//把界符导入字符串Delimiters中
	ReadFromFiles(Keys,BigMax,"Keys.txt");//关键字导入字符串数组Keys.Data[]中
	ReadFromFiles(Operators,BigMax,"Operators.txt");//运算符导入字符串数组Operators.Data[]中
	int pos=0;
	while (pos<FileStream.length())
    {

		if (FileStream[pos]==' '||FileStream[pos]=='\r'||FileStream[pos]==Horizontal_table)   //排除空格、换行、制表符
		{
		    pos++;
		}
		else if (FileStream[pos]=='\n')//回车符用来标记行数
		{
			pos++;
			row++;
		}
		else
		{
            if (isalpha(FileStream[pos])) //是字母
            {
				JudgeFromKeysAndIde(FileStream,pos,FileStream.length(),Keys,Head,Nail,row);//判断是否是关键字并把这个单词链在链表尾部
			}
			else if (isdigit(FileStream[pos]))//是数字
			{
				ApplyAndInsert(Head,Nail,MakeupNumber(FileStream,pos,row),Number,row);
			}
			else
			{
				string s;
				if(isDelimiter(FileStream,pos,Delimiters)) //判断是否是边界符 ,边界符只是一个字符
				{
					char c[2];
					c[1]='\0';c[0]=FileStream[pos-1];//如果是边界符，那么这时FileStream[pos]应该是边界符的下一个字符
				    ApplyAndInsert(Head,Nail,c,Delimiter,row);
				}
				else if(isOperator(FileStream,pos,Operators,s)) //运算符  如果是运算符，则找到最大匹配的运算符并存入s中 注意：本程序中将"//","/*"，"*/"
				{	                                             //作为运算符进行处理；
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
//将数字转换成字符串
template <class T>
string NumberToString(T v)
{
     ostringstream os;
     os<<v;
     return os.str();
}

//创建空格
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


//向文件中写入提示信息,也即数字1--5代表什么类型并写入value ..type ...row等提示信息
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
    Notice=Del+CreatSpace(5)+K+CreatSpace(5)+Ope+CreatSpace(5)+Num+CreatSpace(5)+Ide+'\n'+t+'\n';//注意：每个系统对行结尾符可能不一样，但由于是给用户的，不需要重新读入内存，所以无影响
 //   cout<<Notice<<endl;
 //   file.write(reinterpret_cast<char *>(&Notice),sizeof Notice);//为何使用这个函数文件开头会出现一些乱码？
    for(int i=0;i<Notice.length();i++)
    {
        file.put(Notice[i]);
    }
}

//写入数据
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


//把分类的结果写入文件Words_User.txt;
void WriteToFileForUser(struct group_Main *Head)
{
	if (NULL!=Head)
	{
		ofstream file;
		file.open("Words_User.txt");//文本输出
		if(!file.is_open())//检测文件是否打开
		{
			cout<<"Can not creat file "<<"Words_User.txt"<<endl;
			return;
		}
        PutAnnouncementForUser(file);//向文件中写入提示信息,也即数字1--5代表什么类型并写入一行“value     type       row   ”等提示信息
		PutDataForUser(file,Head);
		file.close();
	}

}
void DeleteLink(struct group_Main *Head)//删除动态申请的结点
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





