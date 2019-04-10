#include<iostream>

//using namespace std;

namespace ab
{

int num = 10;

void display()
{
    std::cout<<"ab::display()"<<std::endl;
}

}//end of namespace ab;

int main()
{
    std::cout << "num=" << ab::num << std::endl;
    ab::display();

    return 0;
}
