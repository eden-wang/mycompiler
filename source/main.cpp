//把界符1，关键字2，运算符3导入字符串数组中  还有数字4 标识符5
//注意：为了方便性，现将字符串"//","/*"，"*/"放入Operators.txt中；并且在对应的函数中进行相关的操作；
#include "a_word.h"
#include "a.h"
#define MaxLength 100000
int main()
{


	string FileStream;
	struct group_Main *Head=NULL;//设置头结点；头结点没有数据
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
        SwitchToAnalysisStream(Head,Stream_Arr,Stream_Len);//用一些简单的符号表示程序代码流
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
