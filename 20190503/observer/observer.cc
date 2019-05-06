

#include"observer.h"

#include<stdlib.h>

Baby::Baby(const string &name)
: _name(name)
{
    cout << "Baby(const string &)" << endl;
}

void Baby::action()
{
    update();
}

void Baby::update()
{
    ::srand(::clock());
    _random = ::rand() % 100;
    if(_random < 30)
    {
        cout << ">> Baby " << _name << " is crying " << endl;
    }else
    {
        cout << ">> Baby " << _name << " is still sleeping" << endl;
    }
}

Nurse::Nurse(const string &name)
: _name(name), _random(-1)
{
    cout << "Nurse(const string &)" << endl;
}

void Nurse::action()
{
    update();
}

void Nurse::update()
{
    ::srand(::clock());
    _random = ::rand() % 100;
    if(_random > 0 && _random < 70)
    {
        cout << "Nurse " << _name << " is going to open the door" << endl;
    }else{
        cout << "Nurse " << _name << " is still sleeping" << endl;
    }
}
