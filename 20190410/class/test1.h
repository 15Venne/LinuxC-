#ifndef __WD_TEST1_H__
#define __WD_TEST1_H__

class computer
{
public:
    void setBrand(const char *brand);
    void setPrice(float price);
    void print();
protected:
    char _brand[20];
private:
    float _price;
};
#endif
