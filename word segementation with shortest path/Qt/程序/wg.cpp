#include "wg.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include<algorithm>
#include<cmath>
#include <map>
#include<QFile>
#include <vector>
#include <QTextCodec>
#include <QString>
#define INF 0x3f3f3f3f

wg::wg(){

}
typedef struct Table{//DAGÿ��������洢����Ϣ
    int num;//Դ�㵽�ý�����·�������
    double length;//Դ�㵽�ýڵ�����·��
    int pre[2];//pre[0]��ʾ���ýڵ�����·����ǰ�����±꣬pre[1]��ʾ��ǰ���ж�Ӧ��ǰn��·�������
}Info;
typedef struct Keyword{//TF-IDF�Ż�ʱ��Ҫ�Ĺؼ��ʣ����𷽱��¼���޸�������г��ֵĴʵ�Ĵ�
    string key;
    int i;//��ӦDAG�е��±�
    int j;
}Keyword;
typedef struct Word{//ÿ���ʵĽṹ�壬��Ȩ�غʹ�����������
    double weight;
    string kind = "unknown";
}Word;
vector<Keyword>keywords;//�洢�ؼ��ʵ�����
vector<string> stopwords;//�����������ͣ�ʱ�
map<string,Word>dict;//�洢�ʵ������������ͨ�������ִ��ҵ���Ӧ�ֵ��Ȩ��
void GetDict(){//�����ʵ�Ͳ��ɷִʱ�
//    ifstream infile("E:\\works\\c++\\word_segment_final\\dict-gb2312.txt");
//    ifstream specialwords("E:\\works\\c++\\word_segment_final\\specialwords.txt");
//    ifstream stop("E:\\works\\c++\\word_segment_final\\stopwords.txt");
    ifstream infile("dict-gb2312.txt");
    ifstream specialwords("specialwords.txt");
    ifstream stop("stopwords.txt");

    string s,cn,kind;
    Word c;
    const char * w;
    unsigned long long pos;
    while(getline(infile,s)){//�����ַ����ָ�
        pos = s.find(" ");
        cn = s.substr(0,pos);
        s.erase(0,pos+1);
        pos = s.find(" ");
        w = s.substr(0,pos).c_str();
        c.weight = atoi(w);
        kind = s.substr(pos+1);
        c.kind = kind;
        dict.insert(pair<string,Word>(cn,c));
    }
    while(getline(specialwords,s)){//�򴴽��õĴʵ������ͣ�ô�
        Word sp;
        sp.weight = 0;
        map<string,Word>::iterator iter;
        iter = dict.find(s);
        if(iter != dict.end())iter->second.weight = 0;//����ô��ڴʵ����У�����Ȩ���޸�Ϊ0
        else dict.insert(pair<string,Word>(s,sp));
    }
    while(getline(stop,s)){
        stopwords.push_back(s);
    }
}
Word SearchInDict(string &s){//�����ַ������Ҵʵ��ж�Ӧ��Ȩ��
    Word non;
    non.weight = INF;
    map<string,Word>::iterator iter;
    iter = dict.find(s);
    if(iter != dict.end())return iter->second;
    else return non;
}
vector<QString> Print(vector<vector<Info>> &tables,vector<vector<Word>> &G,string &s,int l,int n){//�Էִʽ���Ĵ�ӡ
    int k,i,j,t;
    vector<QString> qs;
    string as,ss,final;//as�洢���ԣ�ss�洢�ִʺ�����
    for(k = 0;k < n;k++){
        ss = s;
        final.clear();
        as.clear();
        t = l-1;
        cout<<"��"<<k+1<<"��·���ִʽ��Ϊ��";
        for(i = tables[l-1][k].pre[0],j = tables[l-1][k].pre[1];i != 0;i = tables[t][j].pre[0],j = tables[t][j].pre[1]){
            as = "-" + G[i][t].kind + as;//���Ա��
            ss.insert(2*i,"/");//������в��롰/�����ﵽ�ִʵ�Ŀ��
            t = i;
        }
        as = G[0][t].kind + as;
        final = ss+"\n���Ա�ǣ�"+as + "\n·�����ȣ�" + to_string(tables[l-1][k].length);
        QTextCodec* pCodec = QTextCodec::codecForName("gb2312");//���ı����ת���Ƚ�gb2312תUnicode��תUTF-8
        QString qstr = pCodec->toUnicode(final.c_str(), final.length());
        qs.push_back(qstr);
    }
    return qs;
}
vector<vector<Word>> BuildGraph(string &s){//���ݴʵ����ι���DAG�����ڽӾ�������ʾ��i->j�ߴ�����δ�i+1���ֿ�ʼ��j-i���ַ�
    int i,j,l;
    string ps;
    Word non;
    Keyword Key;
    non.weight = INF;
    l = s.length()/2+1;
    vector<vector<Word>>graph(l,vector<Word>(l,non));//�ڽӾ���ĳ�ʼ��
    for(i = 0;i < l;i++){
        for(j = i;j < l;j++){
            if(i == j)graph[i][i].weight = 0;
            else{
                ps = s.substr(2*i,2*(j-i));//ѡȡ�ַ������Ӵ������ڽӾ���
//                cout<<ps;
                graph[i][j].weight = SearchInDict(ps).weight;
                if(graph[i][j].weight < INF){//��¼�¹ؼ���
                    Key.i = i;
                    Key.j = j;
                    Key.key = ps;
                    keywords.push_back(Key);
                }
                graph[i][j].kind = SearchInDict(ps).kind;
//                cout<<SearchInDict(ps).weight<<" ";
//                cout<<SearchInDict(ps).kind<<endl;
            }
        }
    }
    return graph;
}
int cmp(const Info &a,Info &b){//�ȽϷ���ʹ��sort����������Info��·�����Ⱦ�iѡ�ĸ�����
    return a.length<b.length;
}
void NMin(vector<Info> &can,int n){//��ÿ�������Ϣ��������ֻ����Դ�㵽���ǰn�����·��
    vector<Info> newcan;
    sort(can.begin(),can.end(),cmp);//����
    if(can.size() > n)can.resize(n);
    for(int i = 0;i < can.size();i++){
        if(i!=0 && can[i].pre == can[i-1].pre)can.erase(can.begin()+i);//��������ظ�·������ɾ������һ��
            else can[i].num = i;
    }
}
vector<QString> Dijkstra(vector<vector<Word>> &G,int n,int l,string &s){//��Dijkstra�㷨����֮�Ͻ��иĽ�
    int k;
    vector<vector<Info>> tables(l);//��ά��table�������ÿ�����洢����Ϣ
    vector<Info> table(n);//table�洢ÿ��������Ϣ����ÿ��table���洢n�����·��
    tables[0].push_back({0,0,{0,0}});//��ʼ��
    for(int i = 1;i < l;i++){
        k = 0;
        for(int j = 0;j < i;j++){
            if(G[j][i].weight < INF){
                for(int q = 0;q < tables[j].size();q++){
                    double ll = tables[j][q].length + G[j][i].weight;//ÿһ������·���ǻ���Դ�㵽ǰ�������·����õ㵽ǰ��·�����
                    tables[i].push_back({k,ll,{j,q}});
                    k++;
                }
            }
        }
        NMin( tables[i],n);
    }
//        for(int j = 0;j < tables[l-1].size();j++){
//            cout<<"["<<tables[l-1][j].num<<" "<<tables[l-1][j].length<<" "<<"("<<tables[l-1][j].pre[0]<<","<<tables[l-1][j].pre[1]<<")"<<"]"<<endl;
//        }
       return Print(tables,G,s,l,n);
}
void Wash(string &s){//����ν�����ϴ����ȥ�����ֶ�
    int pos = 0;
    for(int i = 0;i < stopwords.size();i++)
    {
        for(pos = s.find(stopwords[i]);pos != string::npos;pos = s.find(stopwords[i])){
            s.erase(pos,2);
        }
    }
}
void TF_IDF(string &s,vector<vector<Word>> &G){//TF-IDF�Ż�
    int index = 0;
    int count = 0;
    int n = keywords.size();
    for(int i = 0;i < n;i++)
    {
        index = 0;
        count = 0;
        for(index = s.find(keywords[i].key,index);index != string::npos;index = s.find(keywords[i].key,index)){
            count++;
            index = index + keywords[i].key.size();
        }
        G[keywords[i].i][keywords[i].j].weight = log(SearchInDict(keywords[i].key).weight)/count;//�����Ż������޸�DAGͼ������
    }
    keywords.clear();
}
vector<QString> wg::Run(string &s,int n,int m)
{
    int l;
    GetDict();
    Wash(s);
    l = s.length()/2 + 1;//���㹹��DAG�Ľڵ���
    vector<vector<Word>> G(l,vector<Word>(l));
    G = BuildGraph(s);
    if(m)TF_IDF(s,G);
    return  Dijkstra(G,n,l,s);
}
