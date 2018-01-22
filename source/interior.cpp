#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
#include "a_word.h"
#include "a.h"

using namespace std;
// Table的默认构造函数；
R::R()
{
    Len=0;
}
Table::Table()
{
    for(int i=0; i<Ter_Max; i++)
        for(int j=0; j<Ter_Max; j++)
            PriRelaTable[i][j]='&';//&表示无法比较

}
Gram::Gram()//默认构造函数
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
void Gram::InitGram(string filename)//将filename中的文法拿来初始化
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
            Begin=line[0];//开始符
            CreatExp *p=new CreatExp;
            GramHead=p;
            Nail=p;
            p->Left=line[0];
            My_Div(line.substr(3),p->Right);//将产生式右部的字符串分割成终结符和非终结符的组合
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
            My_Div(line.substr(3),p->Right);//将产生式右部的字符串分割成终结符和非终结符的组合
            p->next=NULL;
        }
    }
    file.close();
    InitiaOthMem();//根据得到的文法初始化其他成员
}

void Gram::PrintGram()//输出文法
{
    if(GramHead==NULL)
    {
        cout<<"The Grammars are empty!"<<endl;
        return;
    }
    CreatExp *p=GramHead;
    while(NULL!=p)
    {
        cout<<p->Left<<"->";//输出函数得改
        for(int i=0; i<p->Right.Len; i++)
            cout<<p->Right.A[i];
        cout<<endl;
        p=p->next;
    }
}
bool Gram::isExistVT(string t)////判断是否存在于VT
{
    for(int i=0; i<VTLen; i++)
        if(t==VT[i])
        {
            return true;
        }
    return 0;

}
void Gram::AddToVT(string t)//将t添加到VT中
{
    if(Ter_Max==VTLen)
        cout<<"终结符多于 "<<Ter_Max<<"个"<<endl;
    else
    {
        VTLen++;
        VT[VTLen-1]=t;
    }
}
bool Gram::isExistVN(char c)////判断是否存在于VN
{
    for(unsigned int i=0; i<VN.length(); i++)
        if(c==VN[i])
        {
            return true;
        }
    return 0;

}
void Gram::AddToVN(char c)//将c添加到VN中
{
    VN+=c;
}
void Gram::InitiaOthMem()//根据得到的文法初始化其他成员
{
    if(NULL==GramHead)
    {
        for(int i=0; i<Ter_Max; i++)
            VT[i]="";
        VN="";
    }
    else
    {
        AddToVN(Begin);//把开始符添加进去
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
                }//空语句
            }
            p=p->next;
        }
    }
}
void Gram::PrintVNVT()//输出所有的终极符和非终结符
{
    cout<<"终结符："<<endl;
    for( int i=0; i<VTLen; i++)
        cout<<VT[i]<<" ";
    cout<<endl;
    cout<<"非终结符："<<endl;
    for(unsigned int j=0; j<VN.length(); j++)
        cout<<VN[j]<<" ";
    cout<<endl;
}

int Gram::ReturnVTXiaBiao(string T)//返回该终结符对应的下标；
{
    for(int i=0; i<VTLen; i++)
        if(T==VT[i])
            return i;
    return OvError;//没有找到
}
int Gram::ReturnVNXiaBiao(char N)//返回该非终结符对应的下标；
{
    for(unsigned int i=0; i<VN.length(); i++)
        if(N==VN[i])
            return i;
    return OvError;//没有找到
}

int Gram::Dir(int d,CreatExp *p)//方向函数，1表示求first，返回0；-1表示求last，返回产生式*p最右边的下标
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
void Gram::GetBoolArr(int d)////d=1求解first bool数组;d=-1求解last bool数组;
{
    stack<Ele_Stack> S;
    line=VN.length();
    col=VTLen;
    for(int i=0; i<line; i++)
        for(int j=0; j<col; j++)
            F[i][j]=false;
    CreatExp *p=GramHead;
    Ele_Stack Ele;
    //初始化栈
    while(p!=NULL)
    {
        bool a=isExistVT(p->Right.A[Dir(d,p)]);//first:A->a....(last：A->....a)
        bool b=p->Right.Len>=2&&isExistVN(p->Right.A[Dir(d,p)][0])&&isExistVT(p->Right.A[Dir(d,p)+d]);//first:A->Ba...(last：A->...aB)
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
        //first:(B,a)出栈；对产生式右部扫描，如果有A->B... 把（A，a）压入栈中
        //last：(B,a)出栈；对产生式右部扫描，如果有A->...B 把（A，a）压入栈中
        while(p!=NULL)
        {
            if(Ele.NoTer==p->Right.A[Dir(d,p)][0])//是非终结符
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

void Gram::PrintBoolArr()//输出bool数组
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

void Gram::GetFirst(char c,string Set[Ter_Max],int &len)//计算c的first集并存在Set中
{
    GetBoolArr(1);//求解first bool数组;
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
void Gram::GetLast(char c,string Set[Ter_Max],int &len)//计算c的last集并存在Set中
{
    GetBoolArr(-1);//求解last bool数组;
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
void Gram::GetAnalysisTab()//得到算符优先表
{
    CreatExp *p=GramHead;
    //扫描一遍文法，寻找=,<,>关系
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
void Gram::PrintAnalysisTab()//输出算符优先表
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
//输出栈中的内容
void PrintStack(stack<NoTer_HavePlace> g)
{
    stack<NoTer_HavePlace> SS=g;
    while(!SS.empty())
    {
        cout<<(SS.top()).c;
        SS.pop();
    }

}
//输出剩余句子的内容
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

stack<NoTer_HavePlace> ReserveStack(stack<NoTer_HavePlace> S)//倒置栈
{
    stack<NoTer_HavePlace> SS;
    while(!S.empty())
    {
        SS.push(S.top());
        S.pop();
    }
    return SS;

}
//从栈S中弹出最多4个元素形成字符串并返回
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
//从栈顶向下得到S中第一个N
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
void WriteToFile(string s,ofstream &File);//将字符串s写入File所指向的文件中
bool Gram::JudgeSen(vector<Stream_Analysis> Sentence,int Stream_Len)//分析句子是否合法,句子以#号结束
{

    //为了输出出栈、进栈而定义的变量
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
    NoTer_HavePlace t;//存放从栈顶往下走的第一个终结符
    stack<NoTer_HavePlace> g;//可能要规约的句型
    NoTer_HavePlace a;//如果栈顶是非终结符，用a保存；
    int flag;//flag=1标记S栈顶是非终结符
    stack<int> Link;//标记需要回填的语句的地址
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
       //判断if的控制条件是否已经规约完成
        if(")N(if"==PopNElementToString(S,4))
        {
            NoTer_HavePlace TempN=GetN(S);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+TempN.place + " true goto "+NumberToString(FourElementsArray.RealNumber+2);
            FourElementsArray.RealNumber++;
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+"goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;
        }
        //判断while的控制条件是否已经规约完成
        else if(")N(while"==PopNElementToString(S,4))
        {
            NoTer_HavePlace TempN=GetN(S);
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+TempN.place + " true goto "+NumberToString(FourElementsArray.RealNumber+2);
            FourElementsArray.RealNumber++;
            FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+": "+"goto ";
            Link.push(FourElementsArray.RealNumber);
            FourElementsArray.RealNumber++;
        }
        //判断for的控制条件是否已经规约完成
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
        //判断for的增量是否已经规约完成
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
        //判断结束
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

        //需要规约
        if('>'==T.PriRelaTable[VTXiaBiao_t][Sentence_cur])
        {

            g.push(S.top());
            S.pop();
            NoTer_HavePlace t2;//存储要沿着栈从上往下找下一个终结符
            int VTXiaBiao_t2;
            int Pre_XiaBiao=VTXiaBiao_t;
            //找最左素短语
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
            //已找到最左素短语
            //开始规约。
           //此刻S中已经不包含将要规约的元素
            NoTer_HavePlace N=GuiYue(S,g,Link,File);
            if(" "==N.c)
                continue;
            else  S.push(N);
        }
        else if('<'==T.PriRelaTable[VTXiaBiao_t][Sentence_cur])
        {

            if(1==flag) S.push(a);
            NoTer_HavePlace Ele;
            Ele.codebegin=FourElementsArray.RealNumber;//四元式数组的realnumber表示的是产生式的个数
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
                Ele.codebegin=FourElementsArray.RealNumber;//四元式数组的realnumber表示的是产生式的个数
                Ele.c=Sentence[i].FalseValue;
                Ele.TrueValue=Sentence[i].TrueValue;
                Ele.place="";
                S.push(Ele);
                i++;
            }

        }
        else//没有找到任何的关系; 报错
        {
            cout<<(S.top()).TrueValue<<endl;
            cout<<Sentence[i].FalseValue<<endl;
            if("#"!=t.c)
                RememberRow(Sentence,i);//记住位置
            i++;
        }
        //清空待规约的句子
        while(!g.empty())
        {
            g.pop();
        }
    }
   //为四元式添加最后一行
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

NoTer_HavePlace Gram::GuiYue(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,ofstream &File)//规约句型g，以具有place属性的非终结符的形式返回产生式右边的非终结符
{
    //此刻S中已经不包含将要规约的元素
    //输出战中非终极符的开始位置
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
  */  //*****************输出结束*****************//
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

        if(i==p->Right.Len&&SS.empty())//匹配成功
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
    ReadFromFiles(Operators,BigMax,"Operators.txt");//运算符导入字符串数组Operators.Data[]中
    int i=0;
    while(i<S.length())
    {
        string s="";
        if(S[i]>=0&&S[i]<=32) i++;//其他符号，比如说空格，回车什么的
        else if(S[i]>='a'&&S[i]<='z')//小写字母,字母串所构成的终结符
        {
            string t="";
            while(S[i]>='a'&&S[i]<='z')
            {
                t+=S[i];
                i++;
            }
            Right.A[Right.Len++]=t;

        }
        else if(isOperator(S,i,Operators,s))//是运算符
        {
            Right.A[Right.Len++]=s;

        }
        else  //大写字母或非字母组成的非运算符的终结符
        {
            Right.A[Right.Len++]=S[i];
            i++;
        }


    }
}

void Gram::RememberRow(vector<Stream_Analysis>Sentence,int pos)//记住错误的位置;   粗略得记录错误的位置
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
void Gram::SwitchToNextAvalialeSen(vector<Stream_Analysis>Sentence,int &pos,int Stream_Len)//找到下一个可能可以规约的句子
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

void Gram::PrintErrorRow()//输出错误的位置

{
    cout<<"Part of Wrong Rows(It not the accurate position):";
    for(int i=0; i<EArr.Len; i++)
        cout<<EArr.E[i]<<" ";
    cout<<endl;
}

bool Gram::isExistEArr(int l,int h,int row)//是否已经标记
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


string FormIntoString(stack<NoTer_HavePlace> g) //将g中的元素的Place属性串接成一个字符串
{
    string s="";
    while(!g.empty())
    {
        if("N"==(g.top()).c) //非终结符
            s+=(g.top()).place;
        else if("i"==(g.top()).c) s+=(g.top()).TrueValue; //变量
        else     s+=(g.top()).c; //运算符
        g.pop();
    }
    return s;

}
//将g中的FalseValue属性串接成一个字符串
string C_FormIntoString(stack<NoTer_HavePlace> g)
{
    string s="";
    while(!g.empty())
    {
       s+=(g.top()).c; //运算符
       g.pop();
    }
    return s;

}

void WriteToFile(string s,ofstream &File)       //将字符串s写入文件中
{
    for(int i=0;i<s.length();i++)
        File.put(s[i]);
    File.put('\n');
}
void Gram::InitTempCodeBeginAndPlace(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,int RightLen,NoTer_HavePlace &Temp,ofstream &File)//根据句型的不同，返回的非终结符的Place属性也不一样
{

    static int count=0;
    stack<NoTer_HavePlace> SS=g;
    //将栈中的元素转换成字符串数组
    //根据句型返回不同的Place属性
    string C_TempG=C_FormIntoString(g);
    if("N"==(g.top()).c&&3==RightLen)//形如N+N，N||N,但不能包含逗号表达式
    {
         stack<NoTer_HavePlace> TempG=g;
         NoTer_HavePlace codebeginT=TempG.top();
         TempG.pop();
         Temp.codebegin=codebeginT.codebegin;
         if((TempG.top()).c==",") //防止出现N,N这种情况的发生
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
         string s=t+"="+FormIntoString(g);//将返回的字符串
         s=NumberToString(FourElementsArray.RealNumber)+":"+s;
         FourElementsArray.List[FourElementsArray.RealNumber]=s;
         FourElementsArray.RealNumber++;
         Temp.place=t;
    }
    else if("N"==(g.top()).c&&1==RightLen)//形如N
    {
        Temp.codebegin=(g.top()).codebegin;
        Temp.place="N";

    }
    else if("!"==(g.top()).c&&2==RightLen)//形如!N
    {

         string t="T";
         char InterChar[10];
         InterChar[9]='\0';
         itoa(count,InterChar,10);
         t=t+InterChar;
         count++;
         string s=t+"="+FormIntoString(g);//将返回的字符串
         s=NumberToString(FourElementsArray.RealNumber)+":"+s;
         FourElementsArray.RealNumber++;
       FourElementsArray.List[FourElementsArray.RealNumber]=s;
         g.pop();
         Temp.codebegin=(g.top()).codebegin;
         Temp.place="N";

    }
    else if("("==(g.top()).c&&3==RightLen)//形如(N)
    {
        g.pop();
        Temp.place=(g.top()).place;
        Temp.codebegin=(g.top()).codebegin;
    }
    else if("i"==(g.top()).c&&1==RightLen)  //形如i
    {
        Temp.place=(g.top()).TrueValue;
        Temp.codebegin=FourElementsArray.RealNumber;

    }
    else if("id"==(g.top()).c&&1==RightLen)    //形如id
    {
        Temp.place=(g.top()).TrueValue;
        Temp.codebegin=FourElementsArray.RealNumber;

    }
    else if("i"==(g.top()).c&&3==RightLen)//形如i=B
    {

         string s=FormIntoString(g);//将返回的字符串
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
    else if("{N;}"==C_TempG||"{N;N}"==C_TempG||"{N;N;}"==C_TempG||"{N;N;N}"==C_TempG)//大括号{...}的处理
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
        g.pop();g.pop();//2个pop得到语句开始句
        Temp.place="";
        Temp.codebegin=(g.top()).codebegin;
        g.pop();g.pop();g.pop();g.pop();//4个pop得到栈顶元素为代表增量的非终结符
        FourElementsArray.List[FourElementsArray.RealNumber]=NumberToString(FourElementsArray.RealNumber)+":"+"goto "+NumberToString((g.top()).codebegin);
        g.pop();g.pop();//表示{}开始的语句
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

