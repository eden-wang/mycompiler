#ifndef A_H
#define A_H
#include <string>
#include <iomanip>
#include <vector>
#include <stack>
#define Ter_Max 100
#define NoTer_Max 30
#define Max 1000
#define OvError -1
using namespace std;

struct R //一维数组：表示产生式右边
{
    R();
    string A[Max];
    int Len;
};
struct CreatExp //表示产生式
{
    char Left;
	R Right;
	struct CreatExp *next;
};
struct Table
{
    Table();//默认构造函数
    char PriRelaTable[Ter_Max][Ter_Max];//元素为'='、'<'、'>'，分别表示相等、小于、大于；对应的行、列有一个转换函数，如“void”-->2

};
struct Ele_Stack
{
    char NoTer;
    string Ter;
};
struct ErrorArr
{
    int E[Max];
    int Capacity;
    int Len;
    public:
    ErrorArr(){Len=0;Capacity=Max;}
};

//具有place,codebegin属性的非终结符
struct NoTer_HavePlace
{
    int codebegin;
    string c;
    string TrueValue;
    string place;
};
//四元式线性表
#define MaxNumber_FourElements 10000
#define MaxNumber_Aid 100
struct List_of_Four_Elements
{
    string List[MaxNumber_FourElements];
    int RealNumber;
};
class Gram
{
private:
    CreatExp *GramHead;//产生式的头
    char Begin;//开始符
    string VT[Ter_Max];//终结符,包括#号,不会超过Ter_Max个
    int VTLen;
    string VN;//非终结符;
    Table T;//算符优先关系表
    bool F[NoTer_Max][Ter_Max];//bool数组
    int line;
    int col;
    ErrorArr EArr;
    List_of_Four_Elements FourElementsArray;////四元式数组
public:
    Gram();//默认构造函数
	void InitGram(string filename);//将filename中的文法拿来初始化
	void InitiaOthMem();//根据得到的文法初始化其他成员，比如说VT[],VN,VTLen
	void PrintGram();//输出文法
	void PrintVNVT();//输出所有的终极符和非终结符
	bool isExistVT(string t);////判断是否存在于VT
	void AddToVT(string t);//将t添加到VT中
	bool isExistVN(char c);////判断是否存在于VN
	void AddToVN(char c);//将c添加到VN中
	void GetBoolArr(int d);//d=1求解first bool数组;d=-1求解last bool数组;
	int ReturnVTXiaBiao(string T);//返回该终结符对应的下标；
	int ReturnVNXiaBiao(char No);//返回该非终结符对应的下标；
	void My_Div(string S,R &Right);//将产生式右部的字符串分割成终结符和非终结符的组合
	void PrintBoolArr();//输出bool数组
	void GetFirst(char c,string Set[],int &len);//计算c的first集并存在Set中
	int Dir(int d,CreatExp *p);//方向函数，1表示求first，返回0；-1表示求last，返回产生式右部的长度
	void GetLast(char c,string Set[Ter_Max],int &len);//计算c的last集并存在Set中
	void GetAnalysisTab();//得到算符优先表
	void PrintAnalysisTab();//输出算符优先表
	NoTer_HavePlace GuiYue(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,ofstream &File);//规约句型g，以字符串的形式返回产生式右边的非终结符
	bool JudgeSen(vector<Stream_Analysis> Sentence,int Stream_Len);//分析句子是否合法,句子以#号结束
	bool isExistEArr(int l,int h,int row);//是否已经标记
	void RememberRow(vector<Stream_Analysis>Sentence,int pos);//记住错误的位置
    void SwitchToNextAvalialeSen(vector<Stream_Analysis>Sentence,int &pos,int Stream_Len);//找到下一个可能可以规约的句子
    void PrintErrorRow();//输出错误的位置
    void InitTempCodeBeginAndPlace(stack<NoTer_HavePlace>S,stack<NoTer_HavePlace> g,stack<int> &Link,int RightLen,NoTer_HavePlace &Temp,ofstream &File);//根据产生式类型的不同，返回的非终结符的Place属性也不一样
};


#endif
