#include<iostream>
#include<string.h>
using namespace std;
class X
{
public:
    void print()
    {
        cout<<y<<endl;
    }
private:
    static double y;
};

double X::y;

int main()
{
    //static double X::y;
    X xn;
    cout << sizeof(xn) << endl;
    X *px=nullptr;
    px->print();
    return 0;
}
