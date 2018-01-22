#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
#include "a_word.h"
#include "a.h"

using namespace std;
// Table��Ĭ�Ϲ��캯����
R::R()
{
    Len=0;
}
Table::Table()
{
    for(int i=0; i<Ter_Max; i++)
        for(int j=0; j<Ter_Max; j++)
            PriRelaTable[i][j]='&';//&��ʾ�޷��Ƚ�

}
Gram::Gram()//Ĭ�Ϲ��캯��
{
    GramHead=NULL;
    Begin='0';
    for(int i=0; i<Ter_Max; i++)
        VT[i]="";
    VTLen=0;
    VN="";
    line=0;
    col=0;
    FourElementsArray.RealNumber=0;
}
void Gram::InitGram(string filename)//��filename�е��ķ�������ʼ��
{
    // system("pause");
    ifstream file(filename.c_str());
    if(!file.is_open())
    {
        cout<<"Can't not open "<<filename<<endl;
        exit(0);
    }
    string line;
    CreatExp *Nail=NULL;
    getline(file,line);
    //system("pause");
    if(!file.eof())
    {
        if(line.length()<4) cout<<"Grammar is wrong!"<<endl;
        else
        {
            Begin=line[0];//��ʼ��
            CreatExp *p=new CreatExp;
            GramHead=p;
            Nail=p;
            p->Left=line[0];
            My_Div(line.substr(3),p->Right);//������ʽ�Ҳ����ַ����ָ���ս���ͷ��ս�������
            p->next=NULL;
        }
    }
    while(getline(file,line))
    {
        if(line.length()<4) cout<<"Grammar is wrong!"<<endl;
        else
        {
            CreatExp *p=new CreatExp;
            Nail->next=p;
            Nail=p;
            p->Left=line[0];
            My_Div(line.substr(3),p->Right);//������ʽ�Ҳ����ַ����ָ���ս���ͷ��ս�������
            p->next=NULL;
        }
    }
    file.close();
    InitiaOthMem();//���ݵõ����ķ���ʼ��������Ա
}

void Gram::PrintGram()//����ķ�
{
    if(GramHead==NULL)
    {
        cout<<"The Grammars are empty!"<<endl;
        return;
    }
    CreatExp *p=GramHead;
    while(NULL!=p)
    {
        cout<<p->Left<<"->";//��������ø�
        for(int i=0; i<p->Right.Len; i++)
            cout<<p->Right.A[i];
        cout<<endl;
        p=p->next;
    }
}
bool Gram::isExistVT(string t)////�ж��Ƿ������VT
{
    for(int i=0; i<VTLen; i++)
        if(t==VT[i])
        {
            return true;
        }
    return 0;

}
void Gram::AddToVT(string t)//��t��ӵ�VT��
{
    if(Ter_Max==VTLen)
        cout<<"�ս������ "<<Ter_Max<<"��"<<endl;
    else
    {
        VTLen++;
        VT[VTLen-1]=t;
    }
}
bool Gram::isExistVN(char c)////�ж��Ƿ������VN
{
    for(unsigned int i=0; i<VN.length(); i++)
        if(c==VN[i])
        {
            return true;
        }
    return 0;

}
void Gram::AddToVN(char c)//��c��ӵ�VN��
{
    VN+=c;
}
void Gram::InitiaOthMem()//���ݵõ����ķ���ʼ��������Ա
{
    if(NULL==GramHead)
    {
        for(int i=0; i<Ter_Max; i++)
            VT[i]="";
        VN="";
    }
    else
    {
        AddToVN(Begin);//�ѿ�ʼ����ӽ�ȥ
        AddToVT("#");
        CreatExp *p=GramHead;
        while(p!=NULL)
        {
            for(int i=0; i<p->Right.Len; i++)
            {
                if(p->Right.A[i].length()!=0&&p->Right.A[i][0]>='A'&&p->Right.A[i][0]<='Z')
                {
                    if(!isExistVN(p->Right.A[i][0]))
                        AddToVN(p->Right.A[i][0]);
                }
                else if(p->Right.A[i].length()>=1)
                {
                    if(!isExistVT(p->Right.A[i]))
                        AddToVT(p->Right.A[i]);
                }
                else
                {
                }//�����
            }
            p=p->next;
        }
    }
}
void Gram::PrintVNVT()//������е��ռ����ͷ��ս��
{
    cout<<"�ս����"<<endl;
    for( int i=0; i<VTLen; i++)
        cout<<VT[i]<<" ";
    cout<<endl;
    cout<<"���ս����"<<endl;
    for(unsigned int j=0; j<VN.length(); j++)
        cout<<VN[j]<<" ";
    cout<<endl;
}

int Gram::ReturnVTXiaBiao(string T)//���ظ��ս����Ӧ���±ꣻ
{
    for(int i=0; i<VTLen; i++)
        if(T==VT[i])
            return i;
    return OvError;//û���ҵ�
}
int Gram::ReturnVNXiaBiao(char N)//���ظ÷��ս����Ӧ���±ꣻ
{
    for(unsigned int i=0; i<VN.length(); i++)
        if(N==VN[i])
            return i;
    return OvError;//û���ҵ�
}

int Gram::Dir(int d,CreatExp *p)//��������1��ʾ��first������0��-1��ʾ��last�����ز���ʽ*p���ұߵ��±�
{
    if(1==d)
    {
        return 0;
    }
    else if(-1==d)
    {
        return p->Right.Len-1;
    }
    else
    {
        cout<<"Dir is error!"<<endl;
        exit(0);
    }

}
void Gram::GetBoolArr(int d)////d=1���first bool����;d=-1���last bool����;
{
    stack<Ele_Stack> S;
    line=VN.length();
    col=VTLen;
    for(int i=0; i<line; i++)
        for(int j=0; j<col; j++)
            F[i][j]=false;
    CreatExp *p=GramHead;
    Ele_Stack Ele;
    //��ʼ��ջ
    while(p!=NULL)
    {
        bool a=isExistVT(p->Right.A[Dir(d,p)]);//first:A->a....(last��A->....a)
        bool b=p->Right.Len>=2&&isExistVN(p->Right.A[Dir(d,p)][0])&&isExistVT(p->Right.A[Dir(d,p)+d]);//first:A->Ba...(last��A->...aB)
        int VNXiaBiao=ReturnVNXiaBiao(p->Left);
        if(OvError==VNXiaBiao)
        {
            cout<<"bool Array OvError1"<<p->Left<<" "<<p->Right.A[0]<<endl;
            return;
        }

        if(a)
        {
            Ele.NoTer=p->Left;
            Ele.Ter=p->Right.A[Dir(d,p)];
            int VTXiaBiao=ReturnVTXiaBiao(p->Right.A[Dir(d,p)]);
            if(OvError==VTXiaBiao)
            {
                cout<<"bool Array OvError2"<<endl;
                return;
            }
            if(false==F[VNXiaBiao][VTXiaBiao])
            {
                F[VNXiaBiao][VTXiaBiao]=true;
                S.push(Ele);
            }
        }
        if(b)
        {
            Ele.NoTer=p->Left;
            Ele.Ter=p->Right.A[Dir(d,p)+d];
            int VTXiaBiao=ReturnVTXiaBiao(p->Right.A[Dir(d,p)+d]);
            if(OvError==VTXiaBiao)
            {
                cout<<"bool Array OvError3"<<endl;
                return;
            }
            if(false==F[VNXiaBiao][VTXiaBiao])
            {
                F[VNXiaBiao][VTXiaBiao]=true;
                S.push(Ele);
            }
        }
        p=p->next;
    }
    while(!S.empty())
    {
        Ele=S.top();
        S.pop();
        p=GramHead;
        //first:(B,a)��ջ���Բ���ʽ�Ҳ�ɨ�裬�����A->B... �ѣ�A��a��ѹ��ջ��
        //last��(B,a)��ջ���Բ���ʽ�Ҳ�ɨ�裬�����A->...B �ѣ�A��a��ѹ��ջ��
        while(p!=NULL)
        {
            if(Ele.NoTer==p->Right.A[Dir(d,p)][0])//�Ƿ��ս��
            {
                int VTXiaBiao=ReturnVTXiaBiao(Ele.Ter);
                if(OvError==VTXiaBiao)
                {
                    cout<<"bool Array OvError4"<<endl;
                    return;
                }
                int VNXiaBiao=ReturnVNXiaBiao(p->Left);
                if(OvError==VNXiaBiao)
                {
                    cout<<"bool Array OvError5"<<endl;
                    return;
                }
                Ele_Stack E;
                E.NoTer=p->Left;
                E.Ter=Ele.Ter;
                if(false==F[VNXiaBiao][VTXiaBiao])
                {
                    F[VNXiaBiao][VTXiaBiao]=true;
                    S.push(E);
                }
            }
            p=p->next;
        }
    }

}

void Gram::PrintBoolArr()//���bool����
{
    cout<<"  ";
    for(int k=0; k<VTLen; k++)
        cout<<VT[k];
    cout<<endl;
    for(int i=0; i<line; i++)
    {
        cout<<VN[i]<<" ";
        for(int j=0; j<col; j++)
            cout<<setw(1)<<F[i][j];
        cout<<endl;
    }
}

void Gram::GetFirst(char c,string Set[Ter_Max],int &len)//����c��first��������Set��
{
    GetBoolArr(1);//���first bool����;
    len=0;
    int VNXiaBiao=ReturnVNXiaBiao(c);
    if(OvError==VNXiaBiao)
    {
        cout<<"GetFirst OvError"<<endl;
        return;
    }
    for(int i=0; i<col; i++)
        if(1==F[VNXiaBiao][i])
        {
            Set[len++]=VT[i];
        }
}
void Gram::GetLast(char c,string Set[Ter_Max],int &len)//����c��last��������Set��
{
    GetBoolArr(-1);//���last bool����;
    len=0;
    int VNXiaBiao=ReturnVNXiaBiao(c);
    if(OvError==VNXiaBiao)
    {
        cout<<"GetFirst OvError"<<endl;
        return;
    }
    for(int i=0; i<col; i++)
        if(1==F[VNXiaBiao][i])
        {
            Set[len++]=VT[i];
        }
}
void Gram::GetAnalysisTab()//�õ�������ȱ�
{
    CreatExp *p=GramHead;
    //ɨ��һ���ķ���Ѱ��=,<,>��ϵ
    while(NULL!=p)
    {
        for(int i=0; i<p->Right.Len-1; i++)
        {
            bool a=isExistVT(p->Right.A[i])&&isExistVT(p->Right.A[i+1]);//A->...ab...
            bool b=p->Right.Len>=3&&isExistVT(p->Right.A[i])&&isExistVN(p->Right.A[i+1][0])&&isExistVT(p->Right.A[i+2]);//A->...aBb...
            bool c=isExistVT(p->Right.A[i])&&isExistVN(p->Right.A[i+1][0]);//A->...aB...
            bool d=isExistVN(p->Right.A[i][0])&&isExistVT(p->Right.A[i+1]);//A->...Ba...
            if(a)
            {
                int VTXiaBiao_Line=ReturnVTXiaBiao(p->Right.A[i]);
                int VTXiaBiao_Col=ReturnVTXiaBiao(p->Right.A[i+1]);
                if(OvError==VTXiaBiao_Line||OvError==VTXiaBiao_Col)
                {
                    cout<<"OvError1_="<<endl;
                    return;
                }
                if('&'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]||'='==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col])
                {
                    T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]='=';
                }
                else
                {
                    cout<<"Relation: "<<T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]<<" and "<<" = "<<endl;
                    cout<<"It is not OPG_1"<<endl;
                    exit(0);
                }
            }
            if(b)
            {
                int VTXiaBiao_Line=ReturnVTXiaBiao(p->Right.A[i]);
                int VTXiaBiao_Col=ReturnVTXiaBiao(p->Right.A[i+2]);
                if(OvError==VTXiaBiao_Line||OvError==VTXiaBiao_Col)
                {
                    cout<<"OvError2_="<<endl;
                    return;
                }
                if('&'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]||'='==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col])
                {
                    T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]='=';
                }
                else
                {
                    cout<<"Relation: "<<T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]<<" and "<<" = "<<endl;
                    cout<<"It is not OPG_2"<<endl;
                    exit(0);
                }
            }
            if(c)
            {
                string Set[Ter_Max];
                int len;
                GetFirst(p->Right.A[i+1][0],Set,len);
                for(int j=0; j<len; j++)
                {
                    int VTXiaBiao_Line=ReturnVTXiaBiao(p->Right.A[i]);
                    int VTXiaBiao_Col=ReturnVTXiaBiao(Set[j]);
                    if(OvError==VTXiaBiao_Line||OvError==VTXiaBiao_Col)
                    {
                        cout<<"OvError_<"<<endl;
                        return;
                    }
                    if('&'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]||'<'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col])
                    {
                        T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]='<';
                    }
                    else
                    {
                        cout<<"error:"<<p->Left<<"->"<<p->Right.A[0]<<endl;
                        cout<<p->Right.A[i]<<" "<<Set[j]<<" ";
                        cout<<"Relation: "<<T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]<<" and "<<" < "<<endl;
                        cout<<"It is not OPG_3"<<endl;
                        exit(0);
                    }

                }

            }
            if(d)
            {
                string Set[Ter_Max];
                int len;
                GetLast(p->Right.A[i][0],Set,len);
                for(int j=0; j<len; j++)
                {
                    int VTXiaBiao_Line=ReturnVTXiaBiao(Set[j]);
                    int VTXiaBiao_Col=ReturnVTXiaBiao(p->Right.A[i+1]);
                    if(OvError==VTXiaBiao_Line||OvError==VTXiaBiao_Col)
                    {
                        cout<<"OvError_>"<<endl;
                        return;
                    }
                    if('&'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]||'>'==T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col])
                    {
                        T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]='>';
                    }
                    else
                    {
                        cout<<"error:"<<p->Left<<"->"<<p->Right.A[0]<<endl;
                        cout<<Set[j]<<p->Right.A[i+1]<<" ";
                        cout<<"Relation: "<<T.PriRelaTable[VTXiaBiao_Line][VTXiaBiao_Col]<<" and "<<" > "<<endl;
                        cout<<"It is not OPG_4"<<endl;
                        exit(0);
                    }
                }
            }
        }
        p=p->next;
    }

}
#define Width 5
void Gram::PrintAnalysisTab()//���������ȱ�
{
    for(int t=0; t<Width; t++)
        cout<<" ";
    for(int k=0; k<VTLen; k++)
        cout<<setw(Width)<<VT[k];
    cout<<endl;
    for(int i=0; i<VTLen; i++)
    {
        cout<<setw(Width)<<VT[i];
        for(int j=0; j<VTLen; j++)
            if('&'!=T.PriRelaTable[i][j])
                cout<<setw(Width)<<T.PriRelaTable[i][j];
            else   cout<<setw(Width)<<'&';
        cout<<endl;
    }

}
//���ջ�е�����
void PrintStack(stack<NoTer_HavePlace> g)
{
    stack<NoTer_HavePlace> SS=g;
    while(!SS.empty())
    {
        cout<<(SS.top()).c;
        SS.pop();
    }

}
//���ʣ����ӵ�����
void PrintRemainSen(int current_pos,vector<Stream_Analysis> Sentence,int Stream_Len)
{

    for(int j=current_pos; j<Stream_Len; j++)
        cout<<Sentence[j].FalseValue;
}

string ToStringRemainSen(int current_pos,vector<Stream_Analysis> Sentence,int Stream_Len)
{
    string  t="";
    for(int j=current_pos; j<Stream_Len; j++)
        t+=Sentence[j].FalseValue;
    return t;
}

stack<NoTer_HavePlace> ReserveStack(stack<NoTer_HavePlace> S)//����ջ
{
    stack<NoTer_HavePlace> SS;
    while(!S.empty())
    {
        SS.push(S.top());
        S.pop();
    }
    return SS;

}
//��ջS�е������4��Ԫ���γ��ַ���������
string PopNElementToString(stack<NoTer_HavePlace> S,int n)
{
    string temp="";
    while(!S.empty()&&n>0)
    {
        temp+=(S.top()).c;
        S.pop();
        n--;
    }
    return temp;
}
//��ջ�����µõ�S�е�һ��N
NoTer_HavePlace GetN(stack<NoTer_HavePlace> S)
{
    while(!S.empty())
    {
        if((S.top().c)=="N")
            return S.top();
        S.pop();
    }
    NoTer_HavePlace  Temp;
    Temp.codebegin=-1;
    Temp.c="";
    Temp.TrueValue="";
    Temp.place="";
    return Temp;

}
void WriteToFile(string s,ofstream &File);//���ַ���sд��File��ָ����ļ���
bool Gram::JudgeSen(vector<Stream_Analysis> Sentence,int Stream_Len)//���������Ƿ�Ϸ�,������#�Ž���
{

    //Ϊ�������ջ����ջ������ı���
    ofstream FileStack("InOutStack.txt");
    if(!FileStack.is_open())
    {
        cout<<"Can not open InOutStack.txt"<<endl;
        return false;
    }
    //*****************//
    ofstream File("FourElementsExp.txt");
    if(!File.is_open())
    {
        cout<<"Can not open FourElementsExp.txt"<<endl;
        return false;
    }
    stack<NoTer_HavePlace> S;
    NoTer_HavePlace Ele;
    Ele.codebegin=0;
    Ele.c="#";Ele.TrueValue="#";Ele.place="#";
    S.push(Ele);
    NoTer_HavePlace t;//��Ŵ�ջ�������ߵĵ�һ���ս��
    stack<NoTer_HavePlace> g;//����Ҫ��Լ�ľ���
    NoTer_HavePlace a;//���ջ���Ƿ��ս������a���棻
    int flag;//flag=1���Sջ���Ƿ��ս��
    stack<int> Link;//�����Ҫ��������ĵ�ַ
    for(int i=0; i<Stream_Len;)
    {
        /* cout<<"S_begin:";
        PrintStack(ReserveStack(S));
        PrintStack(g);
        cout<<"       ";
        PrintRemainSen(i,Sentence,Stream_Len);
        cout<<endl;
       */
       string TempFile=PopNElementToString(ReserveStack(S),1000)+PopNElementToString(g,1000);
       TempFile+="      "+ToStringRemainSen(i,Sentence,Stream_Len)+"\n";
       WriteToFile(TempFile,FileStack);
       //�ж�if�Ŀ��������Ƿ��Ѿ���Լ���
        if(")N(if"==PopNElementToString(S,4))
        {
            NoTer_HavePlace TempN=GetN(S);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+TempN.place + " true goto "+NumberToString(FourElementsArray.RealNumber+2);
            FourElementsArray.RealNumber++;
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+"goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;
        }
        //�ж�while�Ŀ��������Ƿ��Ѿ���Լ���
        else if(")N(while"==PopNElementToString(S,4))
        {
            NoTer_HavePlace TempN=GetN(S);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+TempN.place + " true goto "+NumberToString(FourElementsArray.RealNumber+2);
            FourElementsArray.RealNumber++;
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+"goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;
        }
        //�ж�for�Ŀ��������Ƿ��Ѿ���Լ���
        else if(";N;N(for"==PopNElementToString(S,6))
        {
            stack<NoTer_HavePlace> TempS=S;
            TempS.pop();
            NoTer_HavePlace TempN=GetN(TempS);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+TempN.place + " true goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+"goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;

        }
        //�ж�for�������Ƿ��Ѿ���Լ���
        else if(")N;N;N(for"==PopNElementToString(S,8))
        {
            stack<NoTer_HavePlace> TempS=S;
            TempS.pop();TempS.pop();TempS.pop();
            NoTer_HavePlace TempN=GetN(TempS);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+ "goto "+NumberToString(TempN.codebegin);
            FourElementsArray.RealNumber++;
        }
        else
        {

        }
        //�жϽ���
        flag=0;
        a.c="";
        if(isExistVT((S.top()).c))
        {
            t=S.top();
        }
        else
        {
            flag=1;
            a=S.top();
            g.push(a);
            S.pop();
            if(isExistVT((S.top()).c))
            {
                t=S.top();
            }
            else
            {
                cout<<"error1!"<<endl;
            }
        }
         int VTXiaBiao_t=ReturnVTXiaBiao(t.c);
        int Sentence_cur=ReturnVTXiaBiao(Sentence[i].FalseValue);
        if(OvError==VTXiaBiao_t||OvError==Sentence_cur)
        {
            cout<<"OvError"<<endl;
            File.close();
            return 0;
        }

        //��Ҫ��Լ
        if('>'==T.PriRelaTable[VTXiaBiao_t][Sentence_cur])
        {

            g.push(S.top());
            S.pop();
            NoTer_HavePlace t2;//�洢Ҫ����ջ������������һ���ս��
            int VTXiaBiao_t2;
            int Pre_XiaBiao=VTXiaBiao_t;
            //�������ض���
            while(1)
            {
                if(isExistVT((S.top()).c))
                {
                    t2=S.top();

                }
                else
                {
                    g.push(S.top());
                    S.pop();
                    if(isExistVT((S.top()).c))
                        t2=S.top();
                    else
                    {
                        cout<<"error2!"<<endl;
                    }
                }

                VTXiaBiao_t2=ReturnVTXiaBiao(t2.c);
                if('>'==T.PriRelaTable[VTXiaBiao_t2][Pre_XiaBiao]||'='==T.PriRelaTable[VTXiaBiao_t2][Pre_XiaBiao])
                {
                    Pre_XiaBiao=VTXiaBiao_t2;
                    g.push(S.top());
                    S.pop();

                }
                else break;
            }
            //���ҵ������ض���
            //��ʼ��Լ��
           //�˿�S���Ѿ���������Ҫ��Լ��Ԫ��
            NoTer_HavePlace N=GuiYue(S,g,Link,File);
            if(" "==N.c)
                continue;
            else  S.push(N);
        }
        else if('<'==T.PriRelaTable[VTXiaBiao_t][Sentence_cur])
        {

            if(1==flag) S.push(a);
            NoTer_HavePlace Ele;
            Ele.codebegin=FourElementsArray.RealNumber;//��Ԫʽ�����realnumber��ʾ���ǲ���ʽ�ĸ���
            Ele.c=Sentence[i].FalseValue;
            Ele.TrueValue=Sentence[i].TrueValue;
            Ele.place="";
            S.push(Ele);
            i++;
        }
        else if('='==T.PriRelaTable[VTXiaBiao_t][Sentence_cur])
        {
            if(1==flag) S.push(a);
            if("#"==Sentence[i].FalseValue)
            {
                break;
            }
            else
            {
                NoTer_HavePlace Ele;
                Ele.codebegin=FourElementsArray.RealNumber;//��Ԫʽ�����realnumber��ʾ���ǲ���ʽ�ĸ���
                Ele.c=Sentence[i].FalseValue;
                Ele.TrueValue=Sentence[i].TrueValue;
                Ele.place="";
                S.push(Ele);
                i++;
            }

        }
        else//û���ҵ��κεĹ�ϵ; ����
        {
            cout<<(S.top()).TrueValue<<endl;
            cout<<Sentence[i].FalseValue<<endl;
            if("#"!=t.c)
                RememberRow(Sentence,i);//��סλ��
            i++;
        }
        //��մ���Լ�ľ���
        while(!g.empty())
        {
            g.pop();
        }
    }
   //Ϊ��Ԫʽ������һ��
    FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+":return ";
    FourElementsArray.RealNumber++;
    cout<<EArr.Len<<endl;
    PrintErrorRow();
    for(int i=0;i<FourElementsArray.RealNumber;i++)
        WriteToFile(FourElementsArray.List[i],File);
    if(EArr.Len!=0)
    {
       File.close();
       return false;
    }
    File.close();
    return true;
}

NoTer_HavePlace Gram::GuiYue(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,ofstream &File)//��Լ����g���Ծ���place���Եķ��ս������ʽ���ز���ʽ�ұߵķ��ս��
{
    //�˿�S���Ѿ���������Ҫ��Լ��Ԫ��
    //���ս�з��ռ����Ŀ�ʼλ��
 /*   stack<NoTer_HavePlace> TempS=S;
    cout<<"reverse:"<<endl;
    while(!TempS.empty())
    {
        if((TempS.top()).c=="N")
            cout<<(TempS.top()).codebegin<<endl;
        TempS.pop();
    }
    system("pause");
    TempS=g;
    while(!TempS.empty())
    {
        if((TempS.top()).c=="N")
            cout<<(TempS.top()).codebegin<<endl;
        TempS.pop();
    }
    system("pause");
  */  //*****************�������*****************//
 //   stack<NoTer_HavePlace> SS=g;
    CreatExp *p=GramHead;
    int i;
    while(NULL!=p)
      {

        stack<NoTer_HavePlace> SS=g;
        for(i=0; i<p->Right.Len;)
        {
            if(isExistVT(p->Right.A[i])&&!SS.empty()&&isExistVT((SS.top()).c)&&p->Right.A[i]==(SS.top()).c)
            {
                i++;
                SS.pop();
           }
            else if(!isExistVT(p->Right.A[i])&&!SS.empty()&&!isExistVT((SS.top()).c))
            {
                i++;
                SS.pop();
            }
            else
            {

                break;
            }
        }

        if(i==p->Right.Len&&SS.empty())//ƥ��ɹ�
        {

            NoTer_HavePlace  Temp;
            Temp.c="N";Temp.TrueValue="N";
            InitTempCodeBeginAndPlace(S,g,Link,p->Right.Len,Temp,File);
            return Temp;
        }

        p=p->next;
    }
    NoTer_HavePlace  Temp;
    Temp.codebegin=-1;
    Temp.c="";
    Temp.TrueValue="";
    Temp.place="";
    return Temp;

}
extern void	ReadFromFiles(Merger & A,int MaxLength,string filename);
extern bool isOperator(string FileStream,int &pos,Merger  Operators,string &s);
void Gram::My_Div(string S,R &Right)
{
    Merger Operators;
    ReadFromFiles(Operators,BigMax,"Operators.txt");//����������ַ�������Operators.Data[]��
    int i=0;
    while(i<S.length())
    {
        string s="";
        if(S[i]>=0&&S[i]<=32) i++;//�������ţ�����˵�ո񣬻س�ʲô��
        else if(S[i]>='a'&&S[i]<='z')//Сд��ĸ,��ĸ�������ɵ��ս��
        {
            string t="";
            while(S[i]>='a'&&S[i]<='z')
            {
                t+=S[i];
                i++;
            }
            Right.A[Right.Len++]=t;

        }
        else if(isOperator(S,i,Operators,s))//�������
        {
            Right.A[Right.Len++]=s;

        }
        else  //��д��ĸ�����ĸ��ɵķ���������ս��
        {
            Right.A[Right.Len++]=S[i];
            i++;
        }


    }
}

void Gram::RememberRow(vector<Stream_Analysis>Sentence,int pos)//��ס�����λ��;   ���Եü�¼�����λ��
{
    if("#"==Sentence[pos].FalseValue) return;
    system("pause");
    if(0==EArr.Len)
    {

     EArr.E[0]=Sentence[pos].row;
     EArr.Len++;
    }
    else
    {
        bool a=isExistEArr(0,EArr.Len-1,Sentence[pos].row);
        if(!a)
        {
            int i;
            if(EArr.Len==EArr.Capacity) {cout<<"the errorarray is full!"<<endl;return;}
            for(i=EArr.Len-1; i>=0; i--)
                if(EArr.E[i]>Sentence[pos].row)
                {
                    EArr.E[i+1]=EArr.E[i];
                }
                else break;
            EArr.E[i+1]=Sentence[pos].row;
            EArr.Len++;
        }
        else {}
    }
}
void Gram::SwitchToNextAvalialeSen(vector<Stream_Analysis>Sentence,int &pos,int Stream_Len)//�ҵ���һ�����ܿ��Թ�Լ�ľ���
{
    while(pos<Stream_Len)
    {
        if(","==Sentence[pos].FalseValue||";"==Sentence[pos].FalseValue)
            break;
        pos++;
    }
    if(pos<Stream_Len)
        pos++;

}

void Gram::PrintErrorRow()//��������λ��

{
    cout<<"Part of Wrong Rows(It not the accurate position):";
    for(int i=0; i<EArr.Len; i++)
        cout<<EArr.E[i]<<" ";
    cout<<endl;
}

bool Gram::isExistEArr(int l,int h,int row)//�Ƿ��Ѿ����
{
    if(l==h)
    {
        if(EArr.E[l]==row)
            return true;
        else return false;
    }
    else if(l<h)
    {

       int middle=(l+h)/2;
       if(EArr.E[middle]>row)
           return isExistEArr(l,middle-1,row);
       else if(row==EArr.E[middle]) return false;
       else return isExistEArr(middle+1,h,row);
    }
    else
          return false;
}


string FormIntoString(stack<NoTer_HavePlace> g) //��g�е�Ԫ�ص�Place���Դ��ӳ�һ���ַ���
{
    string s="";
    while(!g.empty())
    {
        if("N"==(g.top()).c) //���ս��
            s+=(g.top()).place;
        else if("i"==(g.top()).c) s+=(g.top()).TrueValue; //����
        else     s+=(g.top()).c; //�����
        g.pop();
    }
    return s;

}
//��g�е�FalseValue���Դ��ӳ�һ���ַ���
string C_FormIntoString(stack<NoTer_HavePlace> g)
{
    string s="";
    while(!g.empty())
    {
       s+=(g.top()).c; //�����
       g.pop();
    }
    return s;

}

void WriteToFile(string s,ofstream &File)       //���ַ���sд���ļ���
{
    for(int i=0;i<s.length();i++)
        File.put(s[i]);
    File.put('\n');
}
void Gram::InitTempCodeBeginAndPlace(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,int RightLen,NoTer_HavePlace &Temp,ofstream &File)//���ݾ��͵Ĳ�ͬ�����صķ��ս����Place����Ҳ��һ��
{

    static int count=0;
    stack<NoTer_HavePlace> SS=g;
    //��ջ�е�Ԫ��ת�����ַ�������
    //���ݾ��ͷ��ز�ͬ��Place����
    string C_TempG=C_FormIntoString(g);
    if("N"==(g.top()).c&&3==RightLen)//����N+N��N||N,�����ܰ������ű��ʽ
    {
         stack<NoTer_HavePlace> TempG=g;
         NoTer_HavePlace codebeginT=TempG.top();
         TempG.pop();
         Temp.codebegin=codebeginT.codebegin;
         if((TempG.top()).c==",") //��ֹ����N,N��������ķ���
         {
             Temp.place="";
             return;
         }
         string t="T";
         char InterChar[10];
         InterChar[9]='\0';
         itoa(count,InterChar,10);
         t=t+InterChar;
         count++;
         string s=t+"="+FormIntoString(g);//�����ص��ַ���
         s=NumberToString(FourElementsArray.RealNumber)+":"+s;
         FourElementsArray.List[FourElementsArray.RealNumber]=s;
         FourElementsArray.RealNumber++;
         Temp.place=t;
    }
    else if("N"==(g.top()).c&&1==RightLen)//����N
    {
        Temp.codebegin=(g.top()).codebegin;
        Temp.place="N";

    }
    else if("!"==(g.top()).c&&2==RightLen)//����!N
    {

         string t="T";
         char InterChar[10];
         InterChar[9]='\0';
         itoa(count,InterChar,10);
         t=t+InterChar;
         count++;
         string s=t+"="+FormIntoString(g);//�����ص��ַ���
         s=NumberToString(FourElementsArray.RealNumber)+":"+s;
         FourElementsArray.RealNumber++;
       FourElementsArray.List[FourElementsArray.RealNumber]=s;
         g.pop();
         Temp.codebegin=(g.top()).codebegin;
         Temp.place="N";

    }
    else if("("==(g.top()).c&&3==RightLen)//����(N)
    {
        g.pop();
        Temp.place=(g.top()).place;
        Temp.codebegin=(g.top()).codebegin;
    }
    else if("i"==(g.top()).c&&1==RightLen)  //����i
    {
        Temp.place=(g.top()).TrueValue;
        Temp.codebegin=FourElementsArray.RealNumber;

    }
    else if("id"==(g.top()).c&&1==RightLen)    //����id
    {
        Temp.place=(g.top()).TrueValue;
        Temp.codebegin=FourElementsArray.RealNumber;

    }
    else if("i"==(g.top()).c&&3==RightLen)//����i=B
    {

         string s=FormIntoString(g);//�����ص��ַ���
         s=NumberToString(FourElementsArray.RealNumber)+":"+s;
         FourElementsArray.List[FourElementsArray.RealNumber]=s;
         Temp.place=(g.top()).TrueValue;
         g.pop();
         g.pop();
         Temp.codebegin=(g.top()).codebegin;
         FourElementsArray.RealNumber++;
    }
    else if("{}"==C_TempG)
    {
        Temp.place="";
        Temp.codebegin=FourElementsArray.RealNumber;
    }
    else if("{N;}"==C_TempG||"{N;N}"==C_TempG||"{N;N;}"==C_TempG||"{N;N;N}"==C_TempG)//������{...}�Ĵ���
    {
        g.pop();
        Temp.place=(g.top()).place;
        Temp.codebegin=(g.top()).codebegin;
    }
    else if("if(N)N"==C_TempG)//
    {
        g.pop();
        g.pop();
        Temp.place="";
        Temp.codebegin=(g.top()).codebegin;
        if(Link.top()<=FourElementsArray.RealNumber)
            {
                FourElementsArray.List[Link.top()]+=NumberToString(FourElementsArray.RealNumber);
                Link.pop();
            }

    }
    else if("while(N)N"==C_TempG)//
    {
        g.pop();
        g.pop();
        Temp.place="";
        Temp.codebegin=(g.top()).codebegin;
        FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+":"+"goto "+NumberToString((g.top()).codebegin);
        if(Link.top()<=FourElementsArray.RealNumber)
        {
            FourElementsArray.List[Link.top()]+=NumberToString(FourElementsArray.RealNumber+1);
            Link.pop();
        }
        FourElementsArray.RealNumber++;
    }
    else if("for(N;N;N)N"==C_TempG)
    {
        g.pop();g.pop();//2��pop�õ���俪ʼ��
        Temp.place="";
        Temp.codebegin=(g.top()).codebegin;
        g.pop();g.pop();g.pop();g.pop();//4��pop�õ�ջ��Ԫ��Ϊ���������ķ��ս��
        FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+":"+"goto "+NumberToString((g.top()).codebegin);
        g.pop();g.pop();//��ʾ{}��ʼ�����
        if(Link.top()<=FourElementsArray.RealNumber)
        {
            FourElementsArray.List[Link.top()]+=NumberToString(FourElementsArray.RealNumber+1);
            Link.pop();
        }
        if(Link.top()<=FourElementsArray.RealNumber)
        {
            FourElementsArray.List[Link.top()]+=NumberToString((g.top()).codebegin);
            Link.pop();
        }
        FourElementsArray.RealNumber++;
  }
    else
    {

    }


}

