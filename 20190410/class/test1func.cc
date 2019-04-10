#include"test1.h"
#include<string.h>
#include<iostream>
using std::cout;
using std::endl;

void computer::setBrand(const char *brand)
{
    strcpy(_brand, brand);
}

void computer::setPrice(float price)
{
    _price=price;
}

void computer::print()
{
    cout<<"brand:"<<_brand<<endl
        <<"price:"<<_price<<endl;
}
