
// PIMPL 设计模式
#ifndef __WD_HEAD_H__
#define __WD_HEAD_H__

#include<iostream>
using std::cout;
using std::endl;

class Line
{
public:
    Line(int x1, int y1, int x2, int y2);
    
    void printLine() const;

    ~Line();
private:
    class LineImpl;
private:
    LineImpl *_pimpl; 
};

#endif
