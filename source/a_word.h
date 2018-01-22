#ifndef A_H_INCLUDED
#define A_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <IOMANIP> //cout格式控制
#include <stdlib.h>
#include <sstream> //字符串输出流
#include <vector>
#define Delimiter 1
#define Key 2
#define Operator 3
#define Number 4
#define Identifier 5
#define LittleMax 50
#define BigMax 100
#define IdentifierMax 20
#define MaxSpaceNumber 25
#define SmallSpaceNumber 10
using  namespace std;
//主表中的二元组的定义
struct group_Main
{
    int ID;
    int SubStatementTableID;
    string value;
	int type;
	int row;
	struct group_Main *next;
};
struct Stream_Analysis
{
    string TrueValue;
    string FalseValue;
    int row;

};
//标示符表中的二元组的定义
struct group_StatementTable
{
    int ID;
    int MainTableID;
    string value;
	struct group_StatementTable *next;
};
struct Merger//表示的是关键字和运算符的二元组，第一个是字符串数组，第二个是数组的实际长度
{
	string Data[BigMax];
	int length;
};


void GetFileStream(string  Path,string &FileStream);//得到文件流
struct group_Main *Sort(string FileStream);//分类
group_StatementTable *BuildStatementLink(group_Main *&Head);//通过已经处理过的词分类，对第一次出现的变量的声明建立标示符链表
void WriteToFileForUser(struct group_Main *Head);//把分类的结果写入文件Words_User.txt中
void WriteToFileForUser(struct group_StatementTable *StatementHead);//把分类的结果写入文件Words_User_Statement.txt中
void DeleteLink(struct group_Main *Head);//删除动态申请的结点
void DeleteLink(struct group_StatementTable *StatementHead);//删除动态申请的结点
template <class T>
string NumberToString(T v);//将数字转换成字符串
void SwitchToAnalysisStream(group_Main * Head,vector<Stream_Analysis> &Stream_Arr,int &Stream_Len);//用一些简单的符号表示程序代码流





#endif // A_H_INCLUDED
