//�ѽ��1���ؼ���2�������3�����ַ���������  ��������4 ��ʶ��5
//ע�⣺Ϊ�˷����ԣ��ֽ��ַ���"//","/*"��"*/"����Operators.txt�У������ڶ�Ӧ�ĺ����н�����صĲ�����
#include "a_word.h"
#include "a.h"
#define MaxLength 100000
int main()
{


	string FileStream;
	struct group_Main *Head=NULL;//����ͷ��㣻ͷ���û������
	vector<Stream_Analysis> Stream_Arr(MaxLength);
	int Stream_Len=0;
	group_StatementTable *StatementHead=NULL;
  	string Path;
	char c;
	do{

    	Path="Input.txt";
		FileStream="";
		GetFileStream(Path,FileStream);
		Head=Sort(FileStream);
		StatementHead=BuildStatementLink(Head);
        WriteToFileForUser(Head);
        WriteToFileForUser(StatementHead);
        SwitchToAnalysisStream(Head,Stream_Arr,Stream_Len);//��һЩ�򵥵ķ��ű�ʾ���������
        Gram G;
        G.InitGram("Test_Grammar.txt");
        G.GetAnalysisTab();
        if(G.JudgeSen(Stream_Arr,Stream_Len))
            cout<<"Legal"<<endl;
        else cout<<"Illegal"<<endl;
        DeleteLink(Head);
		DeleteLink(StatementHead);
        cout<<"Continue(Y/y)"<<endl;
		cin>>c;
	}while(c=='Y'||c=='y');
  return 0;
}