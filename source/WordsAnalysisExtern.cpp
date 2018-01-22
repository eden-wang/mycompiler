#include "a_word.h"
#include "a.h"
#define DataTypeNumber 15
//�ж�s�Ƿ���һ�������
bool isDataType(string s)
{
    string A[DataTypeNumber]= {"int","double","short","bool","signed","long","enum","static","unsigned","char","float",
                               "string","ifstream","ofstream","void"
                              };//���������ֻ�ǻ������������ͣ��������û��Զ��������
    for(int i=0; i<DataTypeNumber; i++)
        if(A[i]==s)
        {
            return true;
        }
    return false;
}
//����һ���ڵ��ű�ʶ������������
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
//ͨ���Ѿ�������Ĵʷ��࣬�Ե�һ�γ��ֵı���������������ʶ������
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
        if(isDataType(pr->value))//��ʱ����int a.���ܴ���int a,b;����int a=0;����struct group a;
        {
            //          cout<<"22 "<<endl;
            if(p->type==Identifier)
            {
                //               cout<<33<<endl;
                ApplyAndInsert(StatementHead,StatementNail,p);//����һ���ڵ��ű�ʶ������������
            }
        }
        p=p->next;
        pr=pr->next;
    }
    return StatementHead;
}
extern string CreatSpace(int n);
//���ļ���д����ʾ��Ϣд��һ�С�ID   value     IDInMainTable   ������ʾ��Ϣ
void PutAnnouncementForUser_State(ofstream &file)
{
    string t;
    t="ID"+CreatSpace(MaxSpaceNumber-2)+"value"+CreatSpace(MaxSpaceNumber-5)+"IDInMainTable"+CreatSpace(SmallSpaceNumber-13)+"\n";

//   cout<<Notice<<endl;
//   file.write(reinterpret_cast<char *>(&Notice),sizeof Notice);//Ϊ��ʹ����������ļ���ͷ�����һЩ���룿
    for(int i=0; i<t.length(); i++)
    {
        file.put(t[i]);
    }
}

//д������
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
//�ѷ���Ľ��д���ļ�Words_User_Statement.txt��
void WriteToFileForUser(struct group_StatementTable *StatementHead)
{
    if (NULL!=StatementHead)
    {
        ofstream file;
        file.open("Words_User_Statement.txt");//�ı����
        if(!file.is_open())//����ļ��Ƿ��
        {
            cout<<"Can not creat file "<<"Words_User_Statement.txt"<<endl;
            return;
        }
        PutAnnouncementForUser_State(file);//���ļ���д����ʾ��Ϣд��һ�С�ID   value     IDInMainTable   ������ʾ��Ϣ
        PutDataForUser(file,StatementHead);
        file.close();
    }


}
void DeleteLink(struct group_StatementTable *StatementHead)//ɾ����̬����Ľ��
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
void Initia(string &FalseValue,group_Main *p)//�������Ը�ֵ
{
    switch(p->type)
    {
        case 5:FalseValue="i";break;
        case 4:FalseValue="id";break;
        default:FalseValue=p->value;
    }
}
void SwitchToAnalysisStream(group_Main * Head,vector<Stream_Analysis> &Stream_Arr,int &Stream_Len)//��һЩ�򵥵ķ��ű�ʾ���������
{
    group_Main *p=Head;
    int i=0;
    while(p!=NULL&&i<Stream_Arr.size())
    {
        Stream_Arr[i].TrueValue=p->value;
        Initia(Stream_Arr[i].FalseValue,p);//�������Ը�ֵ
        Stream_Arr[i].row=p->row;
        p=p->next;
        i++;
    }
    Stream_Len=i;
}
