#include "a_word.h"
#include "a.h"
#define DataTypeNumber 15
//判断s是否是一个运算符
bool isDataType(string s)
{
    string A[DataTypeNumber]= {"int","double","short","bool","signed","long","enum","static","unsigned","char","float",
                               "string","ifstream","ofstream","void"
                              };//处理的类型只是基本的数据类型，不包括用户自定义的类型
    for(int i=0; i<DataTypeNumber; i++)
        if(A[i]==s)
        {
            return true;
        }
    return false;
}
//申请一个节点存放标识符，建立链表；
void ApplyAndInsert(group_StatementTable *&StatementHead,group_StatementTable *&StatementNail,group_Main * &p)
{
    group_StatementTable *a=new group_StatementTable;
    if (NULL==StatementHead)
    {
        a->ID=1;
        a->MainTableID=p->ID;
        a->value=p->value;
        p->SubStatementTableID=a->ID;
        StatementHead=a;
        a->next=NULL;
        StatementNail=a;

    }
    else
    {
        a->ID=StatementNail->ID+1;
        a->MainTableID=p->ID;
        a->value=p->value;
        p->SubStatementTableID=a->ID;
        StatementNail->next=a;
        a->next=NULL;
        StatementNail=a;
    }
    /*   group_StatementTable *q=StatementHead;
    while (q!=NULL)
    {
    	cout<<"value "<<setw(15)<<setiosflags(ios_base::left)<<q->value<<"  ID"<<setw(5)<<q->ID<<endl;
    	q=q->next;
    }
    */

}
//通过已经处理过的词分类，对第一次出现的变量的声明建立标识符链表
group_StatementTable *BuildStatementLink(group_Main *&Head)
{
    if(NULL==Head) return NULL;
    group_Main *p=Head->next;
    group_Main *pr=Head;
    group_StatementTable *StatementHead=NULL;
    group_StatementTable *StatementNail=NULL;
    while(p!=NULL)
    {
//       cout<<"value "<<p->value<<endl;
        if(isDataType(pr->value))//暂时处理int a.不能处理int a,b;或者int a=0;或者struct group a;
        {
            //          cout<<"22 "<<endl;
            if(p->type==Identifier)
            {
                //               cout<<33<<endl;
                ApplyAndInsert(StatementHead,StatementNail,p);//申请一个节点存放标识符，建立链表；
            }
        }
        p=p->next;
        pr=pr->next;
    }
    return StatementHead;
}
extern string CreatSpace(int n);
//向文件中写入提示信息写入一行“ID   value     IDInMainTable   ”等提示信息
void PutAnnouncementForUser_State(ofstream &file)
{
    string t;
    t="ID"+CreatSpace(MaxSpaceNumber-2)+"value"+CreatSpace(MaxSpaceNumber-5)+"IDInMainTable"+CreatSpace(SmallSpaceNumber-13)+"\n";

//   cout<<Notice<<endl;
//   file.write(reinterpret_cast<char *>(&Notice),sizeof Notice);//为何使用这个函数文件开头会出现一些乱码？
    for(int i=0; i<t.length(); i++)
    {
        file.put(t[i]);
    }
}

//写入数据
void PutDataForUser(ofstream &file,group_StatementTable *StatementHead)
{
    group_StatementTable *p=StatementHead;
    string t;
    while(NULL!=p)
    {
        t=NumberToString(p->ID)+CreatSpace(MaxSpaceNumber-NumberToString(p->ID).length())+p->value+CreatSpace(MaxSpaceNumber-(p->value).length())+NumberToString(p->MainTableID)+"\n";
        for(int i=0; i<t.length(); i++)
        {
            file.put(t[i]);
        }
        p=p->next;
    }
}
//把分类的结果写入文件Words_User_Statement.txt中
void WriteToFileForUser(struct group_StatementTable *StatementHead)
{
    if (NULL!=StatementHead)
    {
        ofstream file;
        file.open("Words_User_Statement.txt");//文本输出
        if(!file.is_open())//检测文件是否打开
        {
            cout<<"Can not creat file "<<"Words_User_Statement.txt"<<endl;
            return;
        }
        PutAnnouncementForUser_State(file);//向文件中写入提示信息写入一行“ID   value     IDInMainTable   ”等提示信息
        PutDataForUser(file,StatementHead);
        file.close();
    }


}
void DeleteLink(struct group_StatementTable *StatementHead)//删除动态申请的结点
{
    if (NULL!=StatementHead)
    {
        group_StatementTable *p=StatementHead;
        StatementHead=StatementHead->next;
        while (StatementHead!=NULL)
        {
            delete p;
            p=StatementHead;
            StatementHead=StatementHead->next;
        }
        delete p;
    }
}
void Initia(string &FalseValue,group_Main *p)//给假属性赋值
{
    switch(p->type)
    {
        case 5:FalseValue="i";break;
        case 4:FalseValue="id";break;
        default:FalseValue=p->value;
    }
}
void SwitchToAnalysisStream(group_Main * Head,vector<Stream_Analysis> &Stream_Arr,int &Stream_Len)//用一些简单的符号表示程序代码流
{
    group_Main *p=Head;
    int i=0;
    while(p!=NULL&&i<Stream_Arr.size())
    {
        Stream_Arr[i].TrueValue=p->value;
        Initia(Stream_Arr[i].FalseValue,p);//给假属性赋值
        Stream_Arr[i].row=p->row;
        p=p->next;
        i++;
    }
    Stream_Len=i;
}
