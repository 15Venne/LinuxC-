
//接口： std::allocator<T> _alloc
//       _alloc.allocate(size_t); //申请空间 
//       std::uninitialized_copy(开始迭代器, 结束迭代器, 空间的指针); //复制数据给没有数据的新空间
//       _alloc.destroy(迭代器); //销毁迭代器指向的对象
//       _alloc.deallocate(开始的迭代器，size_t);    //销毁空间
//       _alloc.construct(迭代器, 对象) //创建对象
#include<math.h>

#include<iostream>
#include<string>
#include<algorithm>
#include<memory>

using std::cout;
using std::endl;
using std::string;

template<typename T>
class MyVector
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    MyVector()
    : _start(nullptr)
    , _finish(nullptr)
    , _end_of_storage(nullptr)
    {  }
    ~MyVector()
    {  
        if(_start)//以_start是否为nullptr为条件
        {
            while(_start != _finish)
            { _alloc.destroy(--_finish); } //销毁对象
            _alloc.deallocate(_start, my_capacity());//销毁空间
        }
    }

    void my_push_back(const T &);
    void my_pop_back();
    
    size_t my_size() const
    {
        return _finish - _start;
    }
    size_t my_capacity() const
    {
        return _end_of_storage - _start;
    }
    
    void display() const
    {
        cout << "size = " << my_size() << endl
             << "capacity = " << my_capacity() << endl;
    }

    T & operator[](size_t idx)
    {
        return _start[idx];
    }

    iterator begin()
    { return _start; }
    iterator end()
    { return _finish; }
    const_iterator begin() const
    { return _start; }
    const_iterator end() const
    { return _finish; }


private:
    void my_reallocate();
private:
    static std::allocator<T> _alloc;
    T *_start;
    T *_finish;
    T *_end_of_storage;
};//

template <typename T>
std::allocator<T> MyVector<T>::_alloc;

template <typename T>
void MyVector<T>::my_push_back(const T &t)
{
    if(my_size() == my_capacity()) // 如果满了，扩容
    {
        my_reallocate();
    }
    _alloc.construct(_finish, t); // _finish位置构造对象
    ++_finish;
}

template <typename T>
void MyVector<T>::my_pop_back()
{
    if(my_size())
    {
        _alloc.destroy(--_finish);//_finish-1,销毁_finish位置对象
    }
}

template <typename T>
void MyVector<T>::my_reallocate()//开空间，复制，销毁原来的对象，销毁原来的空间
{
    size_t capa = my_capacity();
    size_t newCapa = capa * 2 > 0 ? capa * 2 : 1;//新空间大小

    T *ptemp = _alloc.allocate(newCapa);
    if(_start)
    {
        //copy
        std::uninitialized_copy(_start, _finish, ptemp);//对没有数据的新空间(ptemp)进行拷贝
        while(_start != _finish)
        {
            _alloc.destroy(--_finish); //销毁原对象
        }
        _alloc.deallocate(_start, capa); //销毁空间
    }
    _start = ptemp;
    _finish = _start + capa;
    _end_of_storage = _start + newCapa;

}



void test0()
{
    MyVector<int> numbers;
    numbers.display();

    numbers.my_push_back(1);
    numbers.display();
    numbers.my_push_back(2);
    numbers.display();
    numbers.my_push_back(3);
    numbers.display();
    numbers.my_push_back(4);
    numbers.display();
    numbers.my_push_back(5);
    numbers.display();
    cout << "下标: " << endl;
    for(int i = 0; i < numbers.my_size(); ++i)
    {
        cout << numbers[i] << " ";
    }
    cout << endl;
    cout << "迭代器: " << endl;
    for(MyVector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
    for(auto & e : numbers)
    {
        cout << e << " ";
    }
    cout << endl;
}

void test1()
{
    MyVector<string> numbers;
    numbers.display();

    numbers.my_push_back("shanghai");
    numbers.display();
    numbers.my_push_back("beijing");
    numbers.display();
    numbers.my_push_back("shenzhen");
    numbers.display();
    numbers.my_push_back("guangzhou");
    numbers.display();
    numbers.my_push_back("tianjin");
    numbers.display();
    cout << "下标: " << endl;
    for(int i = 0; i < numbers.my_size(); ++i)
    {
        cout << numbers[i] << " ";
    }
    cout << endl;
    cout << "迭代器: " << endl;
    for(MyVector<string>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
    for(auto & e : numbers)
    {
        cout << e << " ";
    }
    cout << endl;
}

int main()
{
    test0();
    test1();
    return 0;    
}













