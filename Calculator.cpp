#include<iostream>     //标准输入输出流头文件。
#include<cstdlib>     //标准exit()所包含的头文件。
#include<iomanip>     //标准流格式头文件。
#include<stdexcept>     //标准异常类头文件。
#include<stack>     //标准栈头文件。
#include<gmpxx.h>     //GMP大数库头文件(第三方库)。
#include<windows.h>    //Windows头文件。
using namespace std;     //命名空间。
stack<mpf_class> A;     //数据栈(存有效数字)。
stack<char> B;     //字符栈(存运算符)。
void about(void);    //关于计算器。
void color(unsigned int);     //字体颜色设置。
bool run(void);     //混合运算函数。
unsigned int priority(char);     //运算符优先级。
mpf_class computing(char);     //简单运算函数。
mpf_class fmod(mpf_class, mpf_class);     //自定义求模运算函数。
mpf_class pow(mpf_class, unsigned long);     //自定义乘方运算函数。
const mpf_class Max("1e+768",2560);     //2560位大数的最大范围，用科学计数法表示。
const mpf_class Min("-1e+768",2560);     //2560位大数的最大范围，用科学计数法表示。
const mpf_class Pi("3.1415926535897932384626433832795",128);     //Pi的高精度值。
//////////////////////////////////////////////////////////////////////////////////////////
int main(){
    system("cls");     //清屏函数。
    about();    //关于计算器。
    bool Go;     //声明一个布尔变量。
    do{     //程序循环执行。
        Go=run();     //调用混合运算函数。
    }while(Go);     //判断程序是否继续进行。
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void about(){
    color(8);    //更改输出的字体颜色。
    cout<<"Calculator 2.8 Author: HuaJun Liu"<<endl;
}
//////////////////////////////////////////////////////////////////////////////////////////
void color(unsigned int x){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x|0);
}
//////////////////////////////////////////////////////////////////////////////////////////
mpf_class fmod(mpf_class x, mpf_class y){     //自定义求模运算函数。
    mpf_class n(0,2560);
    if(x<0)
        n=ceil(x/y);
    else
        n=floor(x/y);
    return x-n*y;
}
//////////////////////////////////////////////////////////////////////////////////////////
mpf_class pow(mpf_class x, unsigned long y){    //自定义乘方运算函数。
    unsigned long n=y;
    mpf_class z(0,2560);
    for(z=1;;x*=x){
        if((n&1)!=0)
            z*=x;
        if((n>>=1)==0)
            return y<0?1/z:z;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
bool run(){
    unsigned int p1=0,p2=0,bk=0;
    mpf_class P(0,2560),Z(0,2560),Re(0,2560);
    char ch=0,ab=0;     //"ab"变量用来标记上一次进栈的是数字还是运算符。
    while(!A.empty())
        A.pop();       //清空数据栈。
    while(!B.empty())
        B.pop();       //清空运算符栈。
    cin.clear(); cin.sync();      //清空输入流。
    color(8);    //更改输出的字体颜色。
    cout<<"Import: ";
    color(7);    //更改输入的字体颜色。
    while(cin.get(ch)){      //输入表达式(能读取空格符、换行符、回车符)。
        switch(ch){
        case '(':
            if(ab!='A'){     //左括号前不能是数字。
                B.push(ch); bk++;
                break;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Expression Error"<<endl;      //输入的表达式错误。
            return true;
        case ')':
            if(bk>0 && ab=='A'){    //运算符栈中必须有左括号与右括号前必须是数字。
                while(B.top()!='('){
                    try{     //异常定义(可能抛出异常的语句)。
                        A.push(computing(B.top()));    //如果出现异常，函数内部会抛出异常。
                    }catch(const logic_error &e){     //捕获并处理异常。
                        color(4);    //更改输出的字体颜色。
                        cerr<<e.what()<<endl;     //捕获的错误提示。
                        return true;
                    }catch(const runtime_error &e){
                        color(4);    //更改输出的字体颜色。
                        cerr<<e.what()<<endl;     //捕获的错误提示。
                        return true;
                    }
                    B.pop();     //运算符出栈。
                }
                B.pop(); bk--;     //左括号出栈。
                break;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Expression Error"<<endl;      //输入的表达式错误。
            return true;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
            if(ch=='-' && ab!='A'){     //当上一次进栈的是运算符，当前'-'为负数标号。
                cin.putback(ch);     //此函数将当前字符送回到缓冲区中。
                if(cin>>P){          //检查读取的数字是否有效。
                    if(Min<P){
                        A.push(P); ab='A';      //读取的数字进栈，"ab"变量记录下来。
                        break;
                    }
                    color(4);    //更改输出的字体颜色。
                    cerr<<"Overflow Error"<<endl;      //超出有效的数据范围。
                    return true;
                }
                color(4);    //更改输出的字体颜色。
                cerr<<"Invalid Number"<<endl;      //输入无效的数字。
                return true;
            }else if(ab!='A'){     //运算符前面必须是数字。
                color(4);    //更改输出的字体颜色。
                cerr<<"Expression Error"<<endl;      //输入的表达式错误。
                return true;
            }
            while(!B.empty() && B.top()!='('){   //运算符栈为空或栈顶元素为左括号时停止循环。
                p1= priority(B.top());
                p2= priority(ch);
                if(p1>=p2){
                    try{     //异常定义(可能抛出异常的语句)。
                        A.push(computing(B.top()));    //如果出现异常，函数内部会抛出异常。
                    }catch(const logic_error &e){     //捕获并处理异常。
                        color(4);    //更改输出的字体颜色。
                        cerr<<e.what()<<endl;     //捕获的错误提示。
                        return true;
                    }catch(const runtime_error &e){
                        color(4);    //更改输出的字体颜色。
                        cerr<<e.what()<<endl;     //捕获的错误提示。
                        return true;
                    }
                    B.pop();
                }else
                    break;     //跳出循环。
            }
            B.push(ch); ab='B';     //运算符进栈，"ab"变量记录下来。
            break;
        case 'A':
            if(ab=='A'){     //标记符前面必须是数字。
                Re= A.top(); A.pop();     //（数字栈）栈顶元素赋给(Re)，然后出栈。
                A.push(abs(Re));     //abs(x) x的绝对值。
                break;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Expression Error"<<endl;      //输入的表达式错误。
            return true;
        case 'S':
            if(ab=='A'){
                Re= A.top(); A.pop();     //（数字栈）栈顶元素赋给(Re)，然后出栈。
                if(Re>=0){     //标记符前面必须是数字，而且不能是负数。
                    A.push(sqrt(Re));     //sqrt(x) x的平方根。
                    break;
                }
                color(4);    //更改输出的字体颜色。
                cerr<<"Expression Error"<<endl;      //输入的表达式错误。
                return true;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Expression Error"<<endl;      //输入的表达式错误。
            return true;
        case 'P':
            if(ab!='A'){     //标记符前面必须是字符或者为空。
                A.push(Pi); ab='A';      //读取的数字进栈，"ab"变量记录下来。
                break;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Expression Error"<<endl;      //输入的表达式错误。
            return true;
        case '\n':
            if(B.empty() && A.empty()){
                system("cls");
                about();     //清屏后打印帮助与程序说明。
                return true;
            }else if(bk>0 || ab!='A'){    //等号前不能出现左括号和不能没有数字。
                color(4);    //更改输出的字体颜色。
                cerr<<"Expression Error"<<endl;      //输入的表达式错误。
                return true;
            }
            while(!B.empty()){
                try{     //异常定义(可能抛出异常的语句)。
                    A.push(computing(B.top()));    //如果出现异常，函数内部会抛出异常。
                }catch(const logic_error &e){     //捕获并处理异常。
                    color(4);    //更改输出的字体颜色。
                    cerr<<e.what()<<endl;     //捕获的错误提示。
                    return true;
                }catch(const runtime_error &e){
                    color(4);    //更改输出的字体颜色。
                    cerr<<e.what()<<endl;     //捕获的错误提示。
                    return true;
                }
                B.pop();
            }
            Z= A.top(); A.pop();	  //清空最后一个数据栈。
            color(8);    //更改输出的字体颜色。
            cout<<"Amount: ";
            color(2);    //更改输出的字体颜色。
            cout<<fixed<<setprecision(4)<<Z<<endl;
            return true;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            cin.putback(ch);     //此函数将当前字符送回到缓冲区中。
            if(ab=='A'){     //上一次进栈如果是数字，就说明表达式错误。
                color(4);    //更改输出的字体颜色。
                cerr<<"Expression Error"<<endl;      //输入的表达式错误。
                return true;
            }else if(cin>>P){      //检查读取的数字是否有效。
                if(Max>P){
                    A.push(P); ab='A';      //读取的数字进栈，"ab"变量记录下来。
                    break;
                }
                color(4);    //更改输出的字体颜色。
                cerr<<"Overflow Error"<<endl;      //超出有效的数据范围。
                return true;
            }
            color(4);    //更改输出的字体颜色。
            cerr<<"Invalid Number"<<endl;      //输入无效的数字。
            return true;
        case 'Q':
            return false;     //命令退出程序。
        default:
            color(4);    //更改输出的字体颜色。
            cerr<<"Invalid Character"<<endl;      //输入无效的运算符。
            return true;
        }
    }
    return false;     //当遇到"EOF"时退出程序(组合键是"Ctrl+z"回车退出)。
}
//////////////////////////////////////////////////////////////////////////////////////////
unsigned int priority(char x){
    switch(x){
    case '+':
    case '-': return 1;     //将运算符优先级标记为数字。
    case '*':
    case '/':
    case '%': return 2;
    case '^': return 3;     //乘方运算符的优先级为最高。
    default: exit(1);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
mpf_class computing(char x){
    mpf_class a(0,2560),b(0,2560),c(0,2560);     //初始化大数类型。
    switch(x){
    case '+':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        c= a+b;
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    case '-':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        c= a-b;
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    case '*':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        c= a*b;
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    case '/':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        if(b==0)     //检查除数是否为零。
            throw logic_error("Divisor Is Zero");    //抛出运行前错误异常。
        c= a/b;
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    case '%':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        if(b==0)     //检查除数是否为零。
            throw logic_error("Divisor Is Zero");    //抛出运行前错误异常。
        c= fmod(a,b);    //调用自定义求模运算函数。
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    case '^':
        b= A.top(); A.pop();     //（数字栈）栈顶元素赋给b，然后出栈。
        a= A.top(); A.pop();
        c= pow(a,b.get_ui());    //调用自定义乘方运算函数。
        if(Max>c && Min<c)     //检查结果是否超出最大数据范围。
            return c;
        throw runtime_error("Overflow Error");    //抛出运行时错误异常。
    default: exit(1);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////