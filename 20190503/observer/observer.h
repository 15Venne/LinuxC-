

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include<iostream>
#include<string>
using std::cout;
using std::endl;
using std::string;

class Subject;

class Observer
{
public:
    virtual void update() = 0;
    virtual 
    ~Observer()
    {
        cout << "~Observer()" << endl;
    }
protected:

};//

class Baby
: public Observer
{
public:
    Baby(const string &);
    void action();
    void update();
private:
    string _name;
    int _random;
};//

class Nurse
: public Observer
{
public:
    Nurse(const string &);
    void action();
    void update();
private:
    string _name;
    int _random;
};//

#endif
