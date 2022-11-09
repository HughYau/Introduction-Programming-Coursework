#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstring>

using namespace std;
#define STACK_INIT_SIZE 10
#define STACKINCREMENT 5

typedef struct{//������˳�����
    double *vector;
    int length;
    int listsize;
}SqList;

bool GetElem_Sq(SqList &L,int i,double e){//˳���ı���
    if(i < 1 || i>L.length)return false;
    e = L.vector[i-1];
    return true;
}
bool InitList_Sq(SqList &L){//˳����ʼ��
    L.vector = (double *)malloc(100*sizeof(double));
    if(!L.vector)return false;
    L.length = 0;
    L.listsize = 100;
    return true;
}
bool ListInsert_Sq(SqList &L,int i,double e){//˳������
    if(i<1 || i>L.length +1)return false;
    if(L.length >= L.listsize){
        double* newbase = (double *)realloc(L.vector,(L.listsize + 10)*sizeof(double));//�������ռ�
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
bool ListDelete_Sq(SqList &L,int i){//˳���ɾ��
    if((i < 1)||(i > L.length))return false;
    double *p = &(L.vector[i-1]);
    double *q = L.vector+L.length-1;
    for(p++;p <= q;p++)*(p-1) = *p;
    L.length--;
    return true;
}

void ListClear_Sq(SqList &L){//˳������
    L.length = 0;
    L.listsize = 0;
    free(L.vector);
    L.vector = NULL;
}
SqList CreateVec(SqList &L){//�����Ĵ���
    InitList_Sq(L);
    cout<<"����������������Ԫ�أ��Կո��Ϊ��������س�����������"<<endl;
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
bool AddVec(SqList &A,SqList &B){//�����ӷ�
    if(A.length != B.length)return false;
    for(int i = 0;i < A.length;i++){
        A.vector[i] += B.vector[i];
    }
    ListClear_Sq(B);
    return true;
}
bool SubtractVec(SqList &A,SqList &B){//��������
    if(A.length != B.length)return false;
    for(int i = 0;i < A.length;i++){
        A.vector[i] -= B.vector[i];
    }
    ListClear_Sq(B);
    return true;
}
bool CosVec(SqList &A,SqList &B,double &cos){//�ù�ʽ�������нǵ�����
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
void PrintVec(SqList &L){//��ӡ����
    cout<<"("<<L.vector[0];
    for(int i = 1;i<L.length;i++)cout<<","<<L.vector[i];
    cout<<")"<<endl;
}
//����˳���Ķ���ʽ
typedef struct PolynElem{
    double coef;
    int expn;
}PolynElem;
typedef struct{
    PolynElem *polyn;
    int length;
    int listsize;
}PolynSq;

int cmp_Sq(PolynElem a,PolynElem b){//�ݼ����ıȽ�
    if(a.expn < b.expn)return -1;
    if(a.expn == b.expn)return 0;
    if(a.expn > b.expn)return 1;
}
bool DeletePolyn_Sq(PolynSq &L,int i){//ɾ����i����Ԫ��
    if((i < 1)||(i > L.length))return false;
    PolynElem *p = &(L.polyn[i-1]);
    PolynElem *q = L.polyn+L.length-1;
    for(p++;p <= q;p++)*(p-1) = *p;
    L.length--;
    return true;
}
void ClearPolyn_Sq(PolynSq &L){//˳�������
    L.length = 0;
    free(L.polyn);
    L.polyn = NULL;
}
void InsertPolyn_Sq(PolynSq &L,PolynElem e){//���ܶ���ʽ������������
    int i = 0;
    while(i < L.length&&cmp_Sq(e,L.polyn[i])>0)i++;//����ָ�����ҵ�����Ԫ�ص�λ��
    if(i == L.length||L.length == 0){//�����ջ����������ֱ�Ӳ���β��
        L.polyn[L.length].coef = e.coef;
        L.polyn[L.length].expn = e.expn;
        L.length++;
    }
    else if((cmp_Sq(L.polyn[i],e) == 0)){//���ָ���Ѿ����ֹ�����ϵ�����
        L.polyn[i].coef += e.coef ;
        L.listsize--;
    }
    else if(cmp_Sq(e,L.polyn[i]) < 0){//�ҵ�λ�ú����
        for(int j = L.length-1;j >= i;j--){
            L.polyn[j+1].coef = L.polyn[j].coef;
            L.polyn[j+1].expn = L.polyn[j].expn;
        }
        L.polyn[i].coef = e.coef;
        L.polyn[i].expn = e.expn;
        L.length++;
    }

}
bool InitPolyn_Sq(PolynSq &L,int m){//˳���ĳ�ʼ��
    L.polyn = (PolynElem *)malloc(m*sizeof(PolynElem));
    if(!L.polyn)return false;
    L.length = 0;
    L.listsize = m;
    L.polyn[0].coef = 0.0;
    L.polyn[0].expn = 0;
    return true;
}
void CreatePolyn_Sq(PolynSq &L){//˳������ʽ�Ĵ���
    int i;
    int m;
    PolynElem p;
    cout<<"���������ʽ������";
    cin>>m;
    InitPolyn_Sq(L,m);
    cout<<"����������ÿһ���ϵ����ָ��"<<endl;
    for (i = 0;i < m;i++) {
        cin>>p.coef>>p.expn;
        InsertPolyn_Sq(L,p);
    }
}
void AddPolyn_Sq(PolynSq &A,PolynSq &B){//����ʽ�ӷ�
    PolynElem* newbase = (PolynElem *)realloc(A.polyn,(A.listsize+B.listsize)*sizeof (PolynElem));//ÿһ�μӼ��˶���ҪԤ����ռ�
    A.polyn = newbase;
    A.listsize += B.listsize;
    for(int i = 0;i < B.listsize;i++){
        InsertPolyn_Sq(A,B.polyn[i]);//ֱ�Ӳ��뼴��
    }
    ClearPolyn_Sq(B);//����B
}
void SubtractPolyn_Sq(PolynSq &A,PolynSq &B){
    PolynElem* newbase = (PolynElem *)realloc(A.polyn,(A.listsize+B.listsize)*sizeof (PolynElem));
    A.polyn = newbase;
    A.listsize += B.listsize;
    for(int i = 0;i < B.listsize;i++){
        B.polyn[i].coef = -B.polyn[i].coef;
        InsertPolyn_Sq(A,B.polyn[i]);
    }
    ClearPolyn_Sq(B);//����B
}
void MultiplyPolyn_Sq(PolynSq &A,PolynSq &B,PolynSq &L){//����ʽ�˷�
    PolynElem e;
    InitPolyn_Sq(L,A.listsize*B.listsize);
    for(int i = 0;i < B.listsize;i++){
        for(int j = 0;j < A.listsize;j++){
            e.coef = A.polyn[j].coef * B.polyn[i].coef;
            e.expn = A.polyn[j].expn + B.polyn[i].expn;
            InsertPolyn_Sq(L,e);
        }
    }
    ClearPolyn_Sq(B);//����A��B
    ClearPolyn_Sq(A);
}
void DerivativePolyn_Sq(PolynSq &L){
    for(int i = 0;i < L.listsize;i++){
        L.polyn[i].coef = L.polyn[i].coef*L.polyn[i].expn;
        if(L.polyn[i].expn != 0)L.polyn[i].expn--;
    }
}
void PrintPolyn_Sq(PolynSq &L){//˳������ʽ�Ĵ�ӡ��ʹ����ӷ�����ѧ����ʹ��
    int flag = 1;//�ж����������Ƿ�Ϊ����
    int p = 0;
    for(int i = 0;i < L.length;i++){
        if(flag == 0&&L.polyn[i].coef>0)cout<<"+";//��Ϊ���������ǰ�����ţ�����ǰ�и���
        else if(L.polyn[i].coef<0)cout<<"-";
        else if(L.polyn[i].coef == 0.0)continue;
        switch(L.polyn[i].expn){//�ж�ָ��Ϊ0����ϵ��Ϊ1ʱ����ʾ״��
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
        flag = 0;//���������Ѿ������
    }
    while(p < L.length&&L.polyn[p].coef == 0)p++;
    if(p == L.length)cout<<"0";
    cout<<endl;
}
//����ͷ���ĵ�����Ķ���ʽ
//�㷨˼�����ͬ˳���һ��
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
void ListInit_L(LinkList &L){//�������ʼ��
    L =(LinkList)malloc(sizeof(elem));
    L->coef = 0.0;
    L->expn = 0;
    L->next = NULL;
}
bool GetElem_L(LinkList L,int i,double m,int n){//���������
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
void ListInsert_L(LinkList &L,elem e){//������Ĳ��룬Ҳ�ǲ���ʱ���������
    LinkList p = L;
    while(p->next != NULL){
        if(cmp(*(p->next),e)<0)p = p->next;
        else break;
    }
    if(p->next!=NULL&&(cmp(*(p->next),e) == 0)){
        p->next->coef += e.coef ;//ָ����ͬ��ϵ�����
        if(p->next->coef == 0.0){//��ϵ��Ϊ0ʱɾ���ڵ�
            LinkList q = p->next;
            p->next = p->next->next;
            free(q);
        }
    }
    else if(e.coef != 0.0){//������ֵ��Чʱ
        LinkList s = (LinkList)malloc(sizeof(elem));//�����µĽ��
        s->coef = e.coef;
        s->expn = e.expn;
        if(p->next == NULL)s->next = NULL;
        else s->next = p->next;
        p->next = s;
    }
}

bool DeleteList_L(LinkList &L,int i){//����ɾ��
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

void ClearList_L(LinkList L){//����������
    LinkList p,q;
    p = L;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}
void CreatePolyn_L(LinkList &L){//������Ĵ���
    elem e;
    ListInit_L(L);
    cout<<"����������ÿһ���ϵ��������(���س�������)��"<<endl;
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
    ClearList_L(B);//�������B
}
void SubtractPolyn_L(LinkList &A,LinkList &B){
    LinkList q;
    for(q = B->next;q != NULL;q = q->next){
        q->coef = -(q->coef);
        ListInsert_L(A,*q);
    }
    ClearList_L(B);//�������B
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
    ClearList_L(A);//�������A��B
    ClearList_L(B);
}
void DerivativePolyn_L(LinkList &L){
    LinkList p;
    for(p = L->next;p != NULL;p = p->next){
        p->coef = p->coef*p->expn;
        if(p->expn != 0)p->expn--;//ָ��Ϊ0����
    }
}
void PrintPolyn_L(LinkList &L){//������Ĵ�ӡ,Ŀ�ĺ�ԭ��ͬ˳���
    LinkList p;
    int flag = 1;//����flag
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
void ModelSelct(int n){//�˵�����
    switch(n){
    case 1:{
        cout<<"��ѡ�����ģʽ��"<<endl;
        cout<<"1.������ʾ"<<endl;
        cout<<"2.�����ӷ�"<<endl;
        cout<<"3.��������"<<endl;
        cout<<"4.�����н�����ֵ"<<endl;
        break;
    }
    case 2:{
        cout<<"��ѡ�����ģʽ��"<<endl;
        cout<<"1.����ʽ��ʾ"<<endl;
        cout<<"2.����ʽ�ӷ�"<<endl;
        cout<<"3.����ʽ����"<<endl;
        cout<<"4.����ʽ�˷�"<<endl;
        cout<<"5.����ʽ��"<<endl;
        break;
    }
    case 3:{
        cout<<"��ѡ�����ģʽ��"<<endl;
        cout<<"1.����ʽ��ʾ"<<endl;
        cout<<"2.����ʽ�ӷ�"<<endl;
        cout<<"3.����ʽ����"<<endl;
        cout<<"4.����ʽ�˷�"<<endl;
        cout<<"5.����ʽ��"<<endl;
        break;
    }
    }
}


char op[8] = {'+','-','*','/','^','(',')','#'};
typedef struct{
    char *base;
    char *top;
    int stacksize;
}Tr_Stack;//�����ջ
typedef struct{
    double *base;
    double *top;
    int stacksize;
}Nd_Stack;//����ջ
typedef struct{
    string f;
    string x;
    string fx;
}Fx_Array;//�����Ľṹ��
//������ģ�廯ջ�Ļ�������
template<typename T,typename E>
bool InitStack(T &S){//��ʼ��
   S.base = (E *)malloc(STACK_INIT_SIZE*sizeof(E));
   if(!S.base)return false;
   S.top = S.base;
   S.stacksize = STACK_INIT_SIZE;
   return true;
}
template<typename T,typename E>
bool Push(T &S,E e){//��ջ
    if(S.top >= S.stacksize + S.base){
        S.base = (E *)realloc(S.base,(S.stacksize + STACKINCREMENT)*sizeof(E));
        if(!S.base)return false;
        S.top = S.base +S.stacksize;
    }
    *S.top++ = e;
    return true;
}
template<typename T,typename E>
bool Pop(T &S,E &e){//��ջ
    if(S.top == S.base)return false;
    e = *--S.top;
    return true;
}
template<typename T,typename E>
bool GetTop(T &S,E &e){//��ȡջ��
    if(S.top == S.base)return false;
    e = *(S.top - 1);
    return true;
}
template<typename T>
bool IsEmpty(T &S){//���ջ�Ƿ�Ϊ��
    if(S.top == S.base)return true;
    else return false;
}
bool IsOperator(char c){//���������ַ��Ƿ�Ϊ�����
    for(int i = 0;i < 8;i++){
        if(c == op[i])return true;
    }
    return false;
}
int cmp(char a,char b){
    int  cmp[5][5]={{1,1,1,1,1},    /*�������бȽ���������ȼ��ľ���,3����'=',2����'>',1����'<',0�����ɱ�*/
                    {1,1,1,1,1},
                    {0,0,1,1,1},
                    {0,0,1,1,1},
                    {0,0,0,0,1}};

    int p = 0,q = 0;
    for(int i = 0;op[i] != a;i++)q = i+1;
    for(int i = 0;op[i] != b;i++)p = i+1;
    return cmp[q][p];
}
double doit(double a,double b,char c){//��������
    if(c == '+')return a+b;
    else if(c == '-')return a-b;
    else if(c == '*')return a*b;
    else if(c == '/')return a/b;
    else if(c == '^')return pow(a,b);
}
string MidtoRB(string mid){//������׺���ʽתΪ��׺���ʽ
    string s;
    Tr_Stack in;
    char c,top,x;
    int i = 0;
    InitStack<Tr_Stack,char>(in);
    c = mid[i];
    while(c != '#'){
        if(!IsOperator(c)){//��������������������ֱ�����
            s.push_back(c);
            c = mid[++i];
            if(IsOperator(c))s.push_back('$');//�����һλ���������������ڴ������֮ǰ����һ�����ֵķָ���
        }
        else{
            if(!GetTop(in,top)||c == '('||top == '(')Push(in,c);//��ջ�ջ�ջ��Ϊ������ֱ����ջ
            else{
                GetTop(in,top);
                if(c == ')'){//�����������������ջ��������ǰ���з���
                    while(top != '('){
                        Pop(in,x);
                        s.push_back(x);
                        if(!GetTop(in,top)){
                            cout<<"���Ų�ƥ��"<<endl;
                            break;
                        }
                    }
                    Pop(in,x);
                }
                else if(cmp(c,top) == 0){//���ȼ��ıȽ�
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

    while(Pop(in,x))s.push_back(x);//������ջ��Ԫ��ȫ������
//    cout<<s;
//    cout<<endl;
    return s;
}

void Evaluation(string s){//����׺���ʽ��������
    Nd_Stack nd;
    InitStack<Nd_Stack,double>(nd);
    double num ,a,b,final;
    for(int i = 0;s[i] != '\0';i++){
        if(!IsOperator(s[i])){//�������������������ַ��򸡵�����ת��
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
        else{//�����������ֱ�ӽ�������
            if(!Pop(nd,b)){
                cout<<"����ı��ʽ�������飡"<<endl;
                break;
            }
            if(!Pop(nd,a)){
                cout<<"����ı��ʽ�������飡"<<endl;
                break;
            }
//            cout<<b<<" "<<a<<endl;
            Push(nd,doit(a,b,s[i]));
        }
    }
    GetTop(nd,final);
    cout<<final<<endl;
}
void Replace(string &str){//�����ʽ�еı����ַ��滻Ϊ�����ַ�
    int n,pos;
    string x[20],t[20];
    cout<<"��������ʽ�б���������";
    cin>>n;
    for(int i = 0;i < n;i++){
        cout<<"�������"<<i+1<<"�����������ƺ͸���(�ÿո����):"<<endl;
        cin>>x[i]>>t[i];
    }
    for(int i = 0;i < n;i++){//ͨ���������в����滻
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

void DefFunction(string s,string &f,string &u,string &x){//������ĺ���ͨ���ַ����ָ�Ϊ�������������������ʽ��������ͬ����
    int pos,pos_left,pos_right;
//    cout<<s<<endl;
    pos = s.find("def ");
        s.erase(0,4);
        pos = s.find("=");
        f = s.substr(pos+1);
        x = s.erase(pos);
        pos_left = s.find("(");//�ҵ���������λ��
        pos_right = s.find(")");
        x = s.substr(pos_left+1,pos_right - pos_left - 1);
        u = s.erase(pos_left);
//        cout<<f<<endl<<u<<endl<<x<<endl;
}
void RunFunction(string s,Fx_Array (&p)[20],int i){//ͬ�����ַ����ָ��ҵ��������ͱ������ں�������Ķ�Ӧλ�ã��Ӷ��ҵ����ʽ��������
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
    cout<<"������ָ�def���庯����run���к���,#�Ž������룩��"<<endl;
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
    cout<<"��ѡ��������"<<endl;
    cout<<"1.�������㣨˳���"<<endl;
    cout<<"2.����ʽ���㣨˳���"<<endl;
    cout<<"3.����ʽ���㣨������"<<endl;
    cout<<"4.���ʽ����"<<endl;
    cout<<"5.���������"<<endl;
    cout<<"6.���岢���к�������"<<endl;
    cout<<"��������ţ�";
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
                cout<<"���󣡽��и����������ά����ͬ"<<endl;
            }
            else PrintVec(S);
        }
        else if(m == 3){
            CreateVec(L);
            if(!SubtractVec(S,L)){
                cout<<"���󣡽��и����������ά����ͬ"<<endl;
            }
            else PrintVec(S);
        }
        else if(m == 4){
            CreateVec(L);
            double t;
            if(!CosVec(S,L,t)){
                cout<<"���󣡽��и����������ά����ͬ"<<endl;
            }
            else cout<<"�����нǵ�����ֵΪ��"<<t<<endl;
        }
        else{
            cout<<"��������ȷ����ţ�"<<endl;
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
            cout<<"�������󵼵Ľ�����";
            cin>>t;
            for(;t!=0;t--)DerivativePolyn_Sq(A);
            PrintPolyn_Sq(A);
        }
        else{
            cout<<"��������ȷ����ţ�"<<endl;
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
            cout<<"�������󵼵Ľ�����";
            cin>>t;
            for(;t!=0;t--)DerivativePolyn_L(C);
            PrintPolyn_L(C);
        }
        else{
            cout<<"��������ȷ����ţ�"<<endl;
            ModelSelct(n);
            }

    }
    else if(n == 4){
        cout<<"��������ʽ(#�Ž�������)��"<<endl;
        cin>>ss;
        tt = MidtoRB(ss);
    Evaluation(tt);
}
    else if(n == 5){
    cout<<"��������ʽ(#�Ž�������)��"<<endl;
    cin>>ss;
    tt = MidtoRB(ss);
    Replace(tt);
    Evaluation(tt);
}
    else if(n == 6){
    Function();
}
    else{
        cout<<"��������ȷ����ţ�"<<endl;
        Select();
    }
    Select();
}
int main(){
    Select();
}
