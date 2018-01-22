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

struct R //һά���飺��ʾ����ʽ�ұ�
{
    R();
    string A[Max];
    int Len;
};
struct CreatExp //��ʾ����ʽ
{
    char Left;
	R Right;
	struct CreatExp *next;
};
struct Table
{
    Table();//Ĭ�Ϲ��캯��
    char PriRelaTable[Ter_Max][Ter_Max];//Ԫ��Ϊ'='��'<'��'>'���ֱ��ʾ��ȡ�С�ڡ����ڣ���Ӧ���С�����һ��ת���������硰void��-->2

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

//����place,codebegin���Եķ��ս��
struct NoTer_HavePlace
{
    int codebegin;
    string c;
    string TrueValue;
    string place;
};
//��Ԫʽ���Ա�
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
    CreatExp *GramHead;//����ʽ��ͷ
    char Begin;//��ʼ��
    string VT[Ter_Max];//�ս��,����#��,���ᳬ��Ter_Max��
    int VTLen;
    string VN;//���ս��;
    Table T;//������ȹ�ϵ��
    bool F[NoTer_Max][Ter_Max];//bool����
    int line;
    int col;
    ErrorArr EArr;
    List_of_Four_Elements FourElementsArray;////��Ԫʽ����
public:
    Gram();//Ĭ�Ϲ��캯��
	void InitGram(string filename);//��filename�е��ķ�������ʼ��
	void InitiaOthMem();//���ݵõ����ķ���ʼ��������Ա������˵VT[],VN,VTLen
	void PrintGram();//����ķ�
	void PrintVNVT();//������е��ռ����ͷ��ս��
	bool isExistVT(string t);////�ж��Ƿ������VT
	void AddToVT(string t);//��t��ӵ�VT��
	bool isExistVN(char c);////�ж��Ƿ������VN
	void AddToVN(char c);//��c��ӵ�VN��
	void GetBoolArr(int d);//d=1���first bool����;d=-1���last bool����;
	int ReturnVTXiaBiao(string T);//���ظ��ս����Ӧ���±ꣻ
	int ReturnVNXiaBiao(char No);//���ظ÷��ս����Ӧ���±ꣻ
	void My_Div(string S,R &Right);//������ʽ�Ҳ����ַ����ָ���ս���ͷ��ս�������
	void PrintBoolArr();//���bool����
	void GetFirst(char c,string Set[],int &len);//����c��first��������Set��
	int Dir(int d,CreatExp *p);//��������1��ʾ��first������0��-1��ʾ��last�����ز���ʽ�Ҳ��ĳ���
	void GetLast(char c,string Set[Ter_Max],int &len);//����c��last��������Set��
	void GetAnalysisTab();//�õ�������ȱ�
	void PrintAnalysisTab();//���������ȱ�
	NoTer_HavePlace GuiYue(stack<NoTer_HavePlace> S,stack<NoTer_HavePlace> g,stack<int> &Link,ofstream &File);//��Լ����g�����ַ�������ʽ���ز���ʽ�ұߵķ��ս��
	bool JudgeSen(vector<Stream_Analysis> Sentence,int Stream_Len);//���������Ƿ�Ϸ�,������#�Ž���
	bool isExistEArr(int l,int h,int row);//�Ƿ��Ѿ����
	void RememberRow(vector<Stream_Analysis>Sentence,int pos);//��ס�����λ��
    void SwitchToNextAvalialeSen(vector<Stream_Analysis>Sentence,int &pos,int Stream_Len);//�ҵ���һ�����ܿ��Թ�Լ�ľ���
    void PrintErrorRow();//��������λ��
    void InitTempCodeBeginAndPlace(stack<NoTer_HavePlace>S,stack<NoTer_HavePlace> g,stack<int> &Link,int RightLen,NoTer_HavePlace &Temp,ofstream &File);//���ݲ���ʽ���͵Ĳ�ͬ�����صķ��ս����Place����Ҳ��һ��
};


#endif
