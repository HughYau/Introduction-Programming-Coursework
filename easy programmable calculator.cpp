#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstring>

using namespace std;
#define STACK_INIT_SIZE 10
#define STACKINCREMENT 5

typedef struct{//向量的顺序表构建
    double *vector;
    int length;
    int listsize;
}SqList;

bool GetElem_Sq(SqList &L,int i,double e){//顺序表的遍历
    if(i < 1 || i>L.length)return false;
    e = L.vector[i-1];
    return true;
}
bool InitList_Sq(SqList &L){//顺序表初始化
    L.vector = (double *)malloc(100*sizeof(double));
    if(!L.vector)return false;
    L.length = 0;
    L.listsize = 100;
    return true;
}
bool ListInsert_Sq(SqList &L,int i,double e){//顺序表插入
    if(i<1 || i>L.length +1)return false;
    if(L.length >= L.listsize){
        double* newbase = (double *)realloc(L.vector,(L.listsize + 10)*sizeof(double));//额外分配空间
        if(!newbase)return false;
        L.vector = newbase;
        L.listsize += 10;
    }
    double *q = &(L.vector[i-1]);
    for(double *p = &(L.vector[L.length-1]);p >= q;p--)*(p+1) = *p;
    *q = e;
    L.length++;
    return true;
}
bool ListDelete_Sq(SqList &L,int i){//顺序表删除
    if((i < 1)||(i > L.length))return false;
    double *p = &(L.vector[i-1]);
    double *q = L.vector+L.length-1;
    for(p++;p <= q;p++)*(p-1) = *p;
    L.length--;
    return true;
}

void ListClear_Sq(SqList &L){//顺序表清空
    L.length = 0;
    L.listsize = 0;
    free(L.vector);
    L.vector = NULL;
}
SqList CreateVec(SqList &L){//向量的创建
    InitList_Sq(L);
    cout<<"请依次输入向量的元素（以空格键为间隔，按回车键结束）："<<endl;
    double m;
    int i = 1;
    char r;
    while(true){
        cin>>m;
        ListInsert_Sq(L,i,m);
        i++;
        r = cin.get();
        if(r == 10)break;
    }
    return L;
}
bool AddVec(SqList &A,SqList &B){//向量加法
    if(A.length != B.length)return false;
    for(int i = 0;i < A.length;i++){
        A.vector[i] += B.vector[i];
    }
    ListClear_Sq(B);
    return true;
}
bool SubtractVec(SqList &A,SqList &B){//向量减法
    if(A.length != B.length)return false;
    for(int i = 0;i < A.length;i++){
        A.vector[i] -= B.vector[i];
    }
    ListClear_Sq(B);
    return true;
}
bool CosVec(SqList &A,SqList &B,double &cos){//用公式求向量夹角的余弦
    if(A.length != B.length)return false;
    double r = 0,t = 0,s = 0;
    for(int i = 0;i < A.length;i++){
        r += (A.vector[i]*B.vector[i]);
        t += (A.vector[i]*A.vector[i]);
        s += (B.vector[i]*B.vector[i]);
    }
    cos = r/(sqrt(t)*sqrt(s));
    return true;
}
void PrintVec(SqList &L){//打印向量
    cout<<"("<<L.vector[0];
    for(int i = 1;i<L.length;i++)cout<<","<<L.vector[i];
    cout<<")"<<endl;
}
//定义顺序表的多项式
typedef struct PolynElem{
    double coef;
    int expn;
}PolynElem;
typedef struct{
    PolynElem *polyn;
    int length;
    int listsize;
}PolynSq;

int cmp_Sq(PolynElem a,PolynElem b){//幂级数的比较
    if(a.expn < b.expn)return -1;
    if(a.expn == b.expn)return 0;
    if(a.expn > b.expn)return 1;
}
bool DeletePolyn_Sq(PolynSq &L,int i){//删除第i个的元素
    if((i < 1)||(i > L.length))return false;
    PolynElem *p = &(L.polyn[i-1]);
    PolynElem *q = L.polyn+L.length-1;
    for(p++;p <= q;p++)*(p-1) = *p;
    L.length--;
    return true;
}
void ClearPolyn_Sq(PolynSq &L){//顺序表的清空
    L.length = 0;
    free(L.polyn);
    L.polyn = NULL;
}
void InsertPolyn_Sq(PolynSq &L,PolynElem e){//接受多项式，并进行排序
    int i = 0;
    while(i < L.length&&cmp_Sq(e,L.polyn[i])>0)i++;//按照指数，找到插入元素的位置
    if(i == L.length||L.length == 0){//如果表空或表已满，则直接插在尾端
        L.polyn[L.length].coef = e.coef;
        L.polyn[L.length].expn = e.expn;
        L.length++;
    }
    else if((cmp_Sq(L.polyn[i],e) == 0)){//如果指数已经出现过，则系数相加
        L.polyn[i].coef += e.coef ;
        L.listsize--;
    }
    else if(cmp_Sq(e,L.polyn[i]) < 0){//找到位置后插入
        for(int j = L.length-1;j >= i;j--){
            L.polyn[j+1].coef = L.polyn[j].coef;
            L.polyn[j+1].expn = L.polyn[j].expn;
        }
        L.polyn[i].coef = e.coef;
        L.polyn[i].expn = e.expn;
        L.length++;
    }

}
bool InitPolyn_Sq(PolynSq &L,int m){//顺序表的初始化
    L.polyn = (PolynElem *)malloc(m*sizeof(PolynElem));
    if(!L.polyn)return false;
    L.length = 0;
    L.listsize = m;
    L.polyn[0].coef = 0.0;
    L.polyn[0].expn = 0;
    return true;
}
void CreatePolyn_Sq(PolynSq &L){//顺序表多项式的创建
    int i;
    int m;
    PolynElem p;
    cout<<"请输入多项式项数：";
    cin>>m;
    InitPolyn_Sq(L,m);
    cout<<"请依次输入每一项的系数和指数"<<endl;
    for (i = 0;i < m;i++) {
        cin>>p.coef>>p.expn;
        InsertPolyn_Sq(L,p);
    }
}
void AddPolyn_Sq(PolynSq &A,PolynSq &B){//多项式加法
    PolynElem* newbase = (PolynElem *)realloc(A.polyn,(A.listsize+B.listsize)*sizeof (PolynElem));//每一次加减乘都需要预分配空间
    A.polyn = newbase;
    A.listsize += B.listsize;
    for(int i = 0;i < B.listsize;i++){
        InsertPolyn_Sq(A,B.polyn[i]);//直接插入即可
    }
    ClearPolyn_Sq(B);//销毁B
}
void SubtractPolyn_Sq(PolynSq &A,PolynSq &B){
    PolynElem* newbase = (PolynElem *)realloc(A.polyn,(A.listsize+B.listsize)*sizeof (PolynElem));
    A.polyn = newbase;
    A.listsize += B.listsize;
    for(int i = 0;i < B.listsize;i++){
        B.polyn[i].coef = -B.polyn[i].coef;
        InsertPolyn_Sq(A,B.polyn[i]);
    }
    ClearPolyn_Sq(B);//销毁B
}
void MultiplyPolyn_Sq(PolynSq &A,PolynSq &B,PolynSq &L){//多项式乘法
    PolynElem e;
    InitPolyn_Sq(L,A.listsize*B.listsize);
    for(int i = 0;i < B.listsize;i++){
        for(int j = 0;j < A.listsize;j++){
            e.coef = A.polyn[j].coef * B.polyn[i].coef;
            e.expn = A.polyn[j].expn + B.polyn[i].expn;
            InsertPolyn_Sq(L,e);
        }
    }
    ClearPolyn_Sq(B);//销毁A和B
    ClearPolyn_Sq(A);
}
void DerivativePolyn_Sq(PolynSq &L){
    for(int i = 0;i < L.listsize;i++){
        L.polyn[i].coef = L.polyn[i].coef*L.polyn[i].expn;
        if(L.polyn[i].expn != 0)L.polyn[i].expn--;
    }
}
void PrintPolyn_Sq(PolynSq &L){//顺序表多项式的打印，使其更加符合数学语句的使用
    int flag = 1;//判断所处理项是否为首项
    int p = 0;
    for(int i = 0;i < L.length;i++){
        if(flag == 0&&L.polyn[i].coef>0)cout<<"+";//不为首项，则正项前有正号，负项前有负号
        else if(L.polyn[i].coef<0)cout<<"-";
        else if(L.polyn[i].coef == 0.0)continue;
        switch(L.polyn[i].expn){//判断指数为0或者系数为1时的显示状况
        case 0:cout<<fabs(L.polyn[i].coef);break;
        case 1:{
            if(fabs(L.polyn[i].coef)== 1.0)cout<<"x";
            else cout<<fabs(L.polyn[i].coef)<<"x";
            break;
        }
        default:{
            if(fabs(L.polyn[i].coef)== 1.0)cout<<"x^"<<L.polyn[i].expn;
            else cout<<fabs(L.polyn[i].coef)<<"x^"<<L.polyn[i].expn;
            break;
        }
        }
        flag = 0;//代表首项已经处理过
    }
    while(p < L.length&&L.polyn[p].coef == 0)p++;
    if(p == L.length)cout<<"0";
    cout<<endl;
}
//带表头结点的单链表的多项式
//算法思想基本同顺序表一样
typedef struct elem{
    double coef;
    int expn;
    struct elem *next;
}elem,*LinkList;

int cmp(elem a,elem b){
    if(a.expn < b.expn)return -1;
    if(a.expn == b.expn)return 0;
    if(a.expn > b.expn)return 1;
}
void ListInit_L(LinkList &L){//单链表初始化
    L =(LinkList)malloc(sizeof(elem));
    L->coef = 0.0;
    L->expn = 0;
    L->next = NULL;
}
bool GetElem_L(LinkList L,int i,double m,int n){//单链表遍历
    LinkList p = L->next;
    int j = 1;
    while(p &&j <i){
       p = p->next;
       j++;
    }
    if(!p || j<i)return false;
    m = p->coef;
    n = p->expn;
    return true;
}
void ListInsert_L(LinkList &L,elem e){//单链表的插入，也是插入时遍进行排序
    LinkList p = L;
    while(p->next != NULL){
        if(cmp(*(p->next),e)<0)p = p->next;
        else break;
    }
    if(p->next!=NULL&&(cmp(*(p->next),e) == 0)){
        p->next->coef += e.coef ;//指数相同，系数相加
        if(p->next->coef == 0.0){//当系数为0时删除节点
            LinkList q = p->next;
            p->next = p->next->next;
            free(q);
        }
    }
    else if(e.coef != 0.0){//当输入值有效时
        LinkList s = (LinkList)malloc(sizeof(elem));//插入新的结点
        s->coef = e.coef;
        s->expn = e.expn;
        if(p->next == NULL)s->next = NULL;
        else s->next = p->next;
        p->next = s;
    }
}

bool DeleteList_L(LinkList &L,int i){//结点的删除
    LinkList p = L;
    int j = 0;
    while(p->next && j<i-1){
        p = p->next;
        j++;
    }
    if(!(p->next) || j > i-1)return false;
    LinkList q = p->next;
    p->next = p->next->next;
    free(q);
    return true;
}

void ClearList_L(LinkList L){//单链表的清空
    LinkList p,q;
    p = L;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}
void CreatePolyn_L(LinkList &L){//单链表的创建
    elem e;
    ListInit_L(L);
    cout<<"请依次输入每一项的系数、次数(按回车键结束)："<<endl;
    char r;
    while(1){
        cin>>e.coef;
        cin>>e.expn;
        ListInsert_L(L,e);
        r=cin.get();
        if(r==10) break;
    }
}

void AddPolyn_L(LinkList &A,LinkList &B){
    LinkList q;
    for(q = B->next;q != NULL;q = q->next){
        ListInsert_L(A,*q);
    }
    ClearList_L(B);//清空链表B
}
void SubtractPolyn_L(LinkList &A,LinkList &B){
    LinkList q;
    for(q = B->next;q != NULL;q = q->next){
        q->coef = -(q->coef);
        ListInsert_L(A,*q);
    }
    ClearList_L(B);//清空链表B
}
void MultiplyPolyn_L(LinkList &A,LinkList &B,LinkList &L){
    LinkList p,q;
    elem e;
    ListInit_L(L);
    for(q = B->next;q != NULL;q = q->next){
        for(p = A->next;p != NULL;p = p->next){
            e.coef = p->coef * q->coef;
            e.expn = p->expn + q->expn;
            ListInsert_L(L,e);
        }
    }
    ClearList_L(A);//清空链表A和B
    ClearList_L(B);
}
void DerivativePolyn_L(LinkList &L){
    LinkList p;
    for(p = L->next;p != NULL;p = p->next){
        p->coef = p->coef*p->expn;
        if(p->expn != 0)p->expn--;//指数为0跳过
    }
}
void PrintPolyn_L(LinkList &L){//单链表的打印,目的和原理同顺序表
    LinkList p;
    int flag = 1;//首项flag
    for(p = L->next;p != NULL;p = p->next){
        if(flag == 0&&p->coef>0)cout<<"+";
        else if(p->coef<0)cout<<"-";
        else if(p->coef == 0.0)continue;
        switch(p->expn){
        case 0:cout<<fabs(p->coef);break;
        case 1:{
            if(fabs(p->coef)== 1.0)cout<<"x";
            else cout<<fabs(p->coef)<<"x";
            break;
        }
        default:{
            if(fabs(p->coef)== 1.0)cout<<"x^"<<p->expn;
            else cout<<fabs(p->coef)<<"x^"<<p->expn;
            break;
        }
        }
        flag = 0;
    }
        if(L->next == NULL)cout<<"0";
    cout<<endl;
}
void ModelSelct(int n){//菜单界面
    switch(n){
    case 1:{
        cout<<"请选择计算模式："<<endl;
        cout<<"1.向量表示"<<endl;
        cout<<"2.向量加法"<<endl;
        cout<<"3.向量减法"<<endl;
        cout<<"4.向量夹角余弦值"<<endl;
        break;
    }
    case 2:{
        cout<<"请选择计算模式："<<endl;
        cout<<"1.多项式表示"<<endl;
        cout<<"2.多项式加法"<<endl;
        cout<<"3.多项式减法"<<endl;
        cout<<"4.多项式乘法"<<endl;
        cout<<"5.多项式求导"<<endl;
        break;
    }
    case 3:{
        cout<<"请选择计算模式："<<endl;
        cout<<"1.多项式表示"<<endl;
        cout<<"2.多项式加法"<<endl;
        cout<<"3.多项式减法"<<endl;
        cout<<"4.多项式乘法"<<endl;
        cout<<"5.多项式求导"<<endl;
        break;
    }
    }
}


char op[8] = {'+','-','*','/','^','(',')','#'};
typedef struct{
    char *base;
    char *top;
    int stacksize;
}Tr_Stack;//运算符栈
typedef struct{
    double *base;
    double *top;
    int stacksize;
}Nd_Stack;//数字栈
typedef struct{
    string f;
    string x;
    string fx;
}Fx_Array;//函数的结构体
//下面是模板化栈的基本操作
template<typename T,typename E>
bool InitStack(T &S){//初始化
   S.base = (E *)malloc(STACK_INIT_SIZE*sizeof(E));
   if(!S.base)return false;
   S.top = S.base;
   S.stacksize = STACK_INIT_SIZE;
   return true;
}
template<typename T,typename E>
bool Push(T &S,E e){//入栈
    if(S.top >= S.stacksize + S.base){
        S.base = (E *)realloc(S.base,(S.stacksize + STACKINCREMENT)*sizeof(E));
        if(!S.base)return false;
        S.top = S.base +S.stacksize;
    }
    *S.top++ = e;
    return true;
}
template<typename T,typename E>
bool Pop(T &S,E &e){//出栈
    if(S.top == S.base)return false;
    e = *--S.top;
    return true;
}
template<typename T,typename E>
bool GetTop(T &S,E &e){//获取栈顶
    if(S.top == S.base)return false;
    e = *(S.top - 1);
    return true;
}
template<typename T>
bool IsEmpty(T &S){//检测栈是否为空
    if(S.top == S.base)return true;
    else return false;
}
bool IsOperator(char c){//检测输入的字符是否为运算符
    for(int i = 0;i < 8;i++){
        if(c == op[i])return true;
    }
    return false;
}
int cmp(char a,char b){
    int  cmp[5][5]={{1,1,1,1,1},    /*用来进行比较运算符优先级的矩阵,3代表'=',2代表'>',1代表'<',0代表不可比*/
                    {1,1,1,1,1},
                    {0,0,1,1,1},
                    {0,0,1,1,1},
                    {0,0,0,0,1}};

    int p = 0,q = 0;
    for(int i = 0;op[i] != a;i++)q = i+1;
    for(int i = 0;op[i] != b;i++)p = i+1;
    return cmp[q][p];
}
double doit(double a,double b,char c){//进行运算
    if(c == '+')return a+b;
    else if(c == '-')return a-b;
    else if(c == '*')return a*b;
    else if(c == '/')return a/b;
    else if(c == '^')return pow(a,b);
}
string MidtoRB(string mid){//将将中缀表达式转为后缀表达式
    string s;
    Tr_Stack in;
    char c,top,x;
    int i = 0;
    InitStack<Tr_Stack,char>(in);
    c = mid[i];
    while(c != '#'){
        if(!IsOperator(c)){//若非运算符，则进行数字直接输出
            s.push_back(c);
            c = mid[++i];
            if(IsOperator(c))s.push_back('$');//如果下一位输入的是运算符则在此运算符之前插入一个数字的分隔符
        }
        else{
            if(!GetTop(in,top)||c == '('||top == '(')Push(in,c);//若栈空或栈顶为括号则直接入栈
            else{
                GetTop(in,top);
                if(c == ')'){//若遇到右括号则输出栈内左括号前所有符号
                    while(top != '('){
                        Pop(in,x);
                        s.push_back(x);
                        if(!GetTop(in,top)){
                            cout<<"括号不匹配"<<endl;
                            break;
                        }
                    }
                    Pop(in,x);
                }
                else if(cmp(c,top) == 0){//优先级的比较
                    Push(in,c);
                }
                else if(cmp(c,top) == 1){
                    while(cmp(c,top) == 1 && top != '('){
                        Pop(in,x);
                        s.push_back(x);
                        if(!GetTop(in,top))break;
                    }
                    Push(in,c);
                }
            }
            c = mid[++i];
        }
    }

    while(Pop(in,x))s.push_back(x);//结束后将栈中元素全部弹出
//    cout<<s;
//    cout<<endl;
    return s;
}

void Evaluation(string s){//将后缀表达式进行运算
    Nd_Stack nd;
    InitStack<Nd_Stack,double>(nd);
    double num ,a,b,final;
    for(int i = 0;s[i] != '\0';i++){
        if(!IsOperator(s[i])){//若不是运算符，则进行字符向浮点数的转化
        num = 0.0;
        while(s[i] != '$'&&!IsOperator(s[i])){
            if(s[i] != '.'){
                num = num*10 + s[i] - '0';
                i++;
            }
            else{
                i++;
                double e = 10.0;
                while(s[i] != '$'){
                    num = num + (s[i] - '0')/e;
                    e = e*10;
                    i++;
                }
            }
        }
        Push(nd,num);
//        cout<<"num:"<<num<<endl;
        }
        else{//遇到运算符则直接进行运算
            if(!Pop(nd,b)){
                cout<<"输入的表达式有误，请检查！"<<endl;
                break;
            }
            if(!Pop(nd,a)){
                cout<<"输入的表达式有误，请检查！"<<endl;
                break;
            }
//            cout<<b<<" "<<a<<endl;
            Push(nd,doit(a,b,s[i]));
        }
    }
    GetTop(nd,final);
    cout<<final<<endl;
}
void Replace(string &str){//将表达式中的变量字符替换为数字字符
    int n,pos;
    string x[20],t[20];
    cout<<"请输入表达式中变量个数：";
    cin>>n;
    for(int i = 0;i < n;i++){
        cout<<"请输入第"<<i+1<<"个变量的名称和个数(用空格隔开):"<<endl;
        cin>>x[i]>>t[i];
    }
    for(int i = 0;i < n;i++){//通过遍历进行查找替换
        for(pos = str.find(x[i]);pos != string::npos;pos = str.find(x[i])){
            if(!IsOperator(str[pos-1])&&str[pos-1] != '$'){
                str.insert(pos,"$");
                str.insert(pos+x[i].length()+2,"*");
                str.replace(pos+1,x[i].length(),t[i]);
            }
            else str.replace(pos,x[i].length(),t[i]);
        }

    }
//    cout<<str<<endl;
}

void DefFunction(string s,string &f,string &u,string &x){//将定义的函数通过字符串分割为函数名，变量名，表达式的三个不同数组
    int pos,pos_left,pos_right;
//    cout<<s<<endl;
    pos = s.find("def ");
        s.erase(0,4);
        pos = s.find("=");
        f = s.substr(pos+1);
        x = s.erase(pos);
        pos_left = s.find("(");//找到左右括号位置
        pos_right = s.find(")");
        x = s.substr(pos_left+1,pos_right - pos_left - 1);
        u = s.erase(pos_left);
//        cout<<f<<endl<<u<<endl<<x<<endl;
}
void RunFunction(string s,Fx_Array (&p)[20],int i){//同样的字符串分割找到函数名和变量名在函数数组的对应位置，从而找到表达式进行运算
    int pos_left,pos_right,pos;
    string u,x,ss;
    int m = 0;
    s.erase(0,4);
    pos_left = s.find("(");
    pos_right = s.find(")");
    x = s.substr(pos_left+1,pos_right - pos_left - 1);
    u = s.erase(pos_left);
    while(p[m].f != u&&m <= i)m++;
//    cout<<p[m].fx<<endl;
    ss = MidtoRB(p[m].fx);
//    cout<<p[m].x<<endl;
    for(pos = ss.find(p[m].x);pos != string::npos;pos = ss.find(p[m].x)){
        if(!IsOperator(ss[pos-1])&&ss[pos-1] != '$'){
            ss.insert(pos,"$");
            ss.insert(pos+p[m].x.length()+2,"*");
            ss.replace(pos+1,p[m].x.length(),x);
        }
        else ss.replace(pos,p[m].x.length(),x);
    }
//    cout<<ss<<endl;
    Evaluation(ss);
}
void Function(){
    int i = 0;
    string s;
    Fx_Array Fx[20];
    cout<<"请输入指令（def定义函数，run运行函数,#号结束输入）："<<endl;
    getline(cin,s);
    while(true){
    if(s.find("def") != string::npos){
        DefFunction(s,Fx[i].fx,Fx[i].f,Fx[i].x);
    }
    else if(s.find("run ") != string::npos)RunFunction(s,Fx,i);
    i++;
    getline(cin,s);
    }
}
void Select()
{
    int mm;
    string ss,tt;
    PolynSq A,B,M;
    LinkList C,D,T;
    SqList S,L;
    int m,n,t;
    cout<<"请选择计算对象："<<endl;
    cout<<"1.向量计算（顺序表）"<<endl;
    cout<<"2.多项式计算（顺序表）"<<endl;
    cout<<"3.多项式计算（单链表）"<<endl;
    cout<<"4.表达式计算"<<endl;
    cout<<"5.多变量计算"<<endl;
    cout<<"6.定义并进行函数计算"<<endl;
    cout<<"请输入序号：";
    cin>>n;
    if(n <= 3){
        ModelSelct(n);
        cin>>m;
    }
    if(n == 1){
        CreateVec(S);
        if(m == 1){
            PrintVec(S);
        }
        else if(m == 2){
            CreateVec(L);
            if(!AddVec(S,L)){
                cout<<"错误！进行该运算的向量维数不同"<<endl;
            }
            else PrintVec(S);
        }
        else if(m == 3){
            CreateVec(L);
            if(!SubtractVec(S,L)){
                cout<<"错误！进行该运算的向量维数不同"<<endl;
            }
            else PrintVec(S);
        }
        else if(m == 4){
            CreateVec(L);
            double t;
            if(!CosVec(S,L,t)){
                cout<<"错误！进行该运算的向量维数不同"<<endl;
            }
            else cout<<"向量夹角的余弦值为："<<t<<endl;
        }
        else{
            cout<<"请输入正确的序号！"<<endl;
            ModelSelct(n);
            }
    }
    else if(n == 2){
        CreatePolyn_Sq(A);
        if(m == 1){
            PrintPolyn_Sq(A);
        }
        else if(m == 2){
            CreatePolyn_Sq(B);
            AddPolyn_Sq(A,B);
            PrintPolyn_Sq(A);
        }
        else if(m == 3){
            CreatePolyn_Sq(B);
            SubtractPolyn_Sq(A,B);
            PrintPolyn_Sq(A);
        }
        else if(m == 4){
            CreatePolyn_Sq(B);
            MultiplyPolyn_Sq(A,B,M);
            PrintPolyn_Sq(M);
        }
        else if(m == 5){
            cout<<"请输入求导的阶数：";
            cin>>t;
            for(;t!=0;t--)DerivativePolyn_Sq(A);
            PrintPolyn_Sq(A);
        }
        else{
            cout<<"请输入正确的序号！"<<endl;
            ModelSelct(n);
            }
        cout<<endl;
    }
    else if(n == 3){
        CreatePolyn_L(C);
        if(m == 1){
            PrintPolyn_L(C);
        }
        else if(m == 2){
            CreatePolyn_L(D);
            AddPolyn_L(C,D);
            PrintPolyn_L(C);
        }
        else if(m == 3){
            CreatePolyn_L(D);
            SubtractPolyn_L(C,D);
            PrintPolyn_L(C);
        }
        else if(m == 4){
            CreatePolyn_L(D);
            MultiplyPolyn_L(C,D,T);
            PrintPolyn_L(T);
        }
        else if(m == 5){
            cout<<"请输入求导的阶数：";
            cin>>t;
            for(;t!=0;t--)DerivativePolyn_L(C);
            PrintPolyn_L(C);
        }
        else{
            cout<<"请输入正确的序号！"<<endl;
            ModelSelct(n);
            }

    }
    else if(n == 4){
        cout<<"请输入表达式(#号结束输入)："<<endl;
        cin>>ss;
        tt = MidtoRB(ss);
    Evaluation(tt);
}
    else if(n == 5){
    cout<<"请输入表达式(#号结束输入)："<<endl;
    cin>>ss;
    tt = MidtoRB(ss);
    Replace(tt);
    Evaluation(tt);
}
    else if(n == 6){
    Function();
}
    else{
        cout<<"请输入正确的序号！"<<endl;
        Select();
    }
    Select();
}
int main(){
    Select();
}
