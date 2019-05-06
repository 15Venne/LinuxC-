

#include"subject.h"
#include"observer.h"

#include<iostream>
#include<algorithm>

using std::cout;
using std::endl;
using std::find;

Ring::Ring()
: _isPressed(false)
{  }

void Ring::attach(Observer *pob)
{
    auto it = find(_oblist.begin(), _oblist.end(), pob);
    if(it == _oblist.end())
    {
        _oblist.push_front(pob);
    }
}

void Ring::detach(Observer *pob)
{
    auto it = find(_oblist.begin(), _oblist.end(), pob);
    if(it != _oblist.end())
    {
        _oblist.erase(it);
    }
}

void Ring::notify()
{
    for(auto & pob : _oblist)
    {
        pob->update();
    }
}

void Ring::press()
{
    _isPressed = true;
    notify();
}

void Guest::KnockAt(Ring &ring)// 敲门--按铃--通知--反应
{
    cout << "Guest " << _name << " is Knocking the door" << endl;
    ring.press(); 
}
