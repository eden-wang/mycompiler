#ifndef A_H_INCLUDED
#define A_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <IOMANIP> //cout��ʽ����
#include <stdlib.h>
#include <sstream> //�ַ��������
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
//�����еĶ�Ԫ��Ķ���
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
//��ʾ�����еĶ�Ԫ��Ķ���
struct group_StatementTable
{
    int ID;
    int MainTableID;
    string value;
	struct group_StatementTable *next;
};
struct Merger//��ʾ���ǹؼ��ֺ�������Ķ�Ԫ�飬��һ�����ַ������飬�ڶ����������ʵ�ʳ���
{
	string Data[BigMax];
	int length;
};


void GetFileStream(string  Path,string &FileStream);//�õ��ļ���
struct group_Main *Sort(string FileStream);//����
group_StatementTable *BuildStatementLink(group_Main *&Head);//ͨ���Ѿ�������Ĵʷ��࣬�Ե�һ�γ��ֵı���������������ʾ������
void WriteToFileForUser(struct group_Main *Head);//�ѷ���Ľ��д���ļ�Words_User.txt��
void WriteToFileForUser(struct group_StatementTable *StatementHead);//�ѷ���Ľ��д���ļ�Words_User_Statement.txt��
void DeleteLink(struct group_Main *Head);//ɾ����̬����Ľ��
void DeleteLink(struct group_StatementTable *StatementHead);//ɾ����̬����Ľ��
template <class T>
string NumberToString(T v);//������ת�����ַ���
void SwitchToAnalysisStream(group_Main * Head,vector<Stream_Analysis> &Stream_Arr,int &Stream_Len);//��һЩ�򵥵ķ��ű�ʾ���������





#endif // A_H_INCLUDED
