

#include"subject.h"
#include"observer.h"

#include<iostream>
#include<memory>

using std::cout;
using std::endl;
using std::unique_ptr;

int main()
{
    Ring ring;
    unique_ptr<Observer> baby1(new Baby("dabao"));
    unique_ptr<Observer> baby2(new Baby("erbao"));
    unique_ptr<Observer> nurse1(new Nurse("mary"));
    unique_ptr<Observer> nurse2(new Nurse("lily"));

    ring.attach(baby1.get());
    ring.attach(baby2.get());
    ring.attach(nurse1.get());
    ring.attach(nurse2.get());

    Guest g("John");
    g.KnockAt(ring);

    ring.detach(baby1.get());
    cout << "------------------" << endl;
    g.KnockAt(ring);
    return 0;
}
