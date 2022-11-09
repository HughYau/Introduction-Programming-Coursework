#ifndef WG_H
#define WG_H
#include <vector>
#include <QString>
using namespace std;


class wg
{
public:
    wg();
    vector<QString>Run(string&,int,int);//核心程序与界面的接口，在原来main函数基础之上改
};

#endif // WG_H
