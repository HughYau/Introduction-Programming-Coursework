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
typedef struct Table{//DAG每个结点所存储的信息
    int num;//源点到该结点最短路径的序号
    double length;//源点到该节点的最短路径
    int pre[2];//pre[0]表示到该节点的最短路径的前驱的下标，pre[1]表示该前驱中对应的前n短路径的序号
}Info;
typedef struct Keyword{//TF-IDF优化时需要的关键词，负责方便记录和修改在语段中出现的词典的词
    string key;
    int i;//对应DAG中的下标
    int j;
}Keyword;
typedef struct Word{//每个词的结构体，有权重和词性两种类型
    double weight;
    string kind = "unknown";
}Word;
vector<Keyword>keywords;//存储关键词的数组
vector<string> stopwords;//可以自行添加停词表
map<string,Word>dict;//存储词典的索引表，方便通过查找字串找到对应字典的权重
void GetDict(){//创建词典和不可分词表
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
    while(getline(infile,s)){//采用字符串分割
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
    while(getline(specialwords,s)){//向创建好的词典中添加停用词
        Word sp;
        sp.weight = 0;
        map<string,Word>::iterator iter;
        iter = dict.find(s);
        if(iter != dict.end())iter->second.weight = 0;//如果该词在词典中有，则将其权重修改为0
        else dict.insert(pair<string,Word>(s,sp));
    }
    while(getline(stop,s)){
        stopwords.push_back(s);
    }
}
Word SearchInDict(string &s){//根据字符串查找词典中对应的权重
    Word non;
    non.weight = INF;
    map<string,Word>::iterator iter;
    iter = dict.find(s);
    if(iter != dict.end())return iter->second;
    else return non;
}
vector<QString> Print(vector<vector<Info>> &tables,vector<vector<Word>> &G,string &s,int l,int n){//对分词结果的打印
    int k,i,j,t;
    vector<QString> qs;
    string as,ss,final;//as存储词性，ss存储分词后的语段
    for(k = 0;k < n;k++){
        ss = s;
        final.clear();
        as.clear();
        t = l-1;
        cout<<"第"<<k+1<<"短路径分词结果为：";
        for(i = tables[l-1][k].pre[0],j = tables[l-1][k].pre[1];i != 0;i = tables[t][j].pre[0],j = tables[t][j].pre[1]){
            as = "-" + G[i][t].kind + as;//词性标记
            ss.insert(2*i,"/");//在语段中插入“/”，达到分词的目的
            t = i;
        }
        as = G[0][t].kind + as;
        final = ss+"\n词性标记："+as + "\n路径长度：" + to_string(tables[l-1][k].length);
        QTextCodec* pCodec = QTextCodec::codecForName("gb2312");//中文编码的转换先将gb2312转Unicode再转UTF-8
        QString qstr = pCodec->toUnicode(final.c_str(), final.length());
        qs.push_back(qstr);
    }
    return qs;
}
vector<vector<Word>> BuildGraph(string &s){//根据词典和语段构造DAG，用邻接矩阵来表示，i->j边代表语段从i+1个字开始的j-i个字符
    int i,j,l;
    string ps;
    Word non;
    Keyword Key;
    non.weight = INF;
    l = s.length()/2+1;
    vector<vector<Word>>graph(l,vector<Word>(l,non));//邻接矩阵的初始化
    for(i = 0;i < l;i++){
        for(j = i;j < l;j++){
            if(i == j)graph[i][i].weight = 0;
            else{
                ps = s.substr(2*i,2*(j-i));//选取字符串的子串赋给邻接矩阵
//                cout<<ps;
                graph[i][j].weight = SearchInDict(ps).weight;
                if(graph[i][j].weight < INF){//记录下关键词
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
int cmp(const Info &a,Info &b){//比较方便使用sort函数，根据Info中路径长度经i选哪个排序
    return a.length<b.length;
}
void NMin(vector<Info> &can,int n){//对每个结点信息进行整理，只保留源点到其的前n条最短路径
    vector<Info> newcan;
    sort(can.begin(),can.end(),cmp);//排序
    if(can.size() > n)can.resize(n);
    for(int i = 0;i < can.size();i++){
        if(i!=0 && can[i].pre == can[i-1].pre)can.erase(can.begin()+i);//如果出现重复路径，则删除其中一个
            else can[i].num = i;
    }
}
vector<QString> Dijkstra(vector<vector<Word>> &G,int n,int l,string &s){//在Dijkstra算法基础之上进行改进
    int k;
    vector<vector<Info>> tables(l);//二维的table数组代表每个结点存储的信息
    vector<Info> table(n);//table存储每个结点的信息，即每个table最多存储n条最短路径
    tables[0].push_back({0,0,{0,0}});//初始化
    for(int i = 1;i < l;i++){
        k = 0;
        for(int j = 0;j < i;j++){
            if(G[j][i].weight < INF){
                for(int q = 0;q < tables[j].size();q++){
                    double ll = tables[j][q].length + G[j][i].weight;//每一个结点的路径是基于源点到前驱的最短路径与该点到前驱路径相加
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
void Wash(string &s){//对语段进行清洗，除去无用字段
    int pos = 0;
    for(int i = 0;i < stopwords.size();i++)
    {
        for(pos = s.find(stopwords[i]);pos != string::npos;pos = s.find(stopwords[i])){
            s.erase(pos,2);
        }
    }
}
void TF_IDF(string &s,vector<vector<Word>> &G){//TF-IDF优化
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
        G[keywords[i].i][keywords[i].j].weight = log(SearchInDict(keywords[i].key).weight)/count;//根据优化后结果修改DAG图的数据
    }
    keywords.clear();
}
vector<QString> wg::Run(string &s,int n,int m)
{
    int l;
    GetDict();
    Wash(s);
    l = s.length()/2 + 1;//计算构造DAG的节点数
    vector<vector<Word>> G(l,vector<Word>(l));
    G = BuildGraph(s);
    if(m)TF_IDF(s,G);
    return  Dijkstra(G,n,l,s);
}
