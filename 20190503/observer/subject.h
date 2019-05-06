

#ifndef __SUBJECT_H__ 
#define __SUBJECT_H__

#include<iostream>
#include<list>
#include<string>

using std::cout;
using std::endl;
using std::list;
using std::string;

typedef int Status;

class Observer;

class Subject
{
public:
    virtual void attach(Observer *) = 0;
    virtual void detach(Observer *) = 0;
    virtual void notify() = 0;
    virtual Status getStatus() = 0;
    virtual
    ~Subject()
    {
        cout << "~Subject()" << endl;
    }
};//

class Ring
: public Subject
{
public:
    Ring();
    void attach(Observer *);
    void detach(Observer *);
    void notify();
    Status getStatus()
    {
        return 10;
    }

    void press();
private:
    list<Observer *> _oblist;
    bool _isPressed;
};//

class Guest
{
public:
    Guest(const string &name)
    : _name(name)
    {
        cout << "Guest(const string &)" << endl;
    }

    void KnockAt(Ring &);

    ~Guest()
    { cout << "~Guest()" << endl; }
private:
    string _name;
};//


#endif
