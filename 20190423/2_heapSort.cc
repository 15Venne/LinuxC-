#include<iostream>
#include<vector>
#include<string>



using std::cout;
using std::endl;
using std::vector;
using std::string;

template <typename T, typename Compare = std::less<T> >
class HeapSort
{
public:
    HeapSort(T *arr, int size)
    {
        sort(arr, size);
    }
    void heapAdjust(T*, int, int);
    void sort(T*, int);
    void Tswap(T &a, T &b)
    {
        T tmp = b;
        b = a;
        a = tmp;
    }
private:
    
};

template <typename T, typename Compare>
void HeapSort<T, Compare>::heapAdjust(T *arr, int begin, int end)
{
    int dad = begin;
    int son = dad *2 +1;
    Compare cmp;
    while(son < end)
    {
        //if(son + 1 < end && arr[son] < arr[son + 1])
        if(son+1<end && cmp(arr[son], arr[son+1]))  
            ++son;
        //if(arr[dad] < arr[son])
        if(cmp(arr[dad], arr[son]))
        {
            Tswap(arr[dad], arr[son]);
            dad = son;
            son = dad *2 +1;
        }
        else
        {
            break;
        }
    }
}

template <typename T, typename Compare>
void HeapSort<T, Compare>::sort(T *arr, int size)
{
    int i;
    for(i = size/2 - 1; i >= 0; --i)
    {
        heapAdjust(arr, i, size);
    }
    Tswap(arr[0], arr[size - 1]);
    for(i = size - 1; i > 1; --i)
    {
        heapAdjust(arr, 0, i);
        Tswap(arr[0], arr[i - 1]);
    }
}

void test1()
{
    string st[]={"beijing", "shanghai", "shenzhen", "guangzhou", "tianjin"};
    HeapSort<string> a(st, 5);
    for(auto & w : st)
    {
        cout << w << " ";
    }
    cout << endl;
    HeapSort<string, std::greater<string>> b(st, 5);
    for(auto & w: st)
    {
        cout << w << " ";
    }
    cout << endl;
}

void test0()
{
    int arr[8] = {5, 4, 6, 9, 1, 10, 2, 3};
    int arr2[5]= {1,9,2,0,7};
    HeapSort<int, std::greater<int>> a(arr2, 5);
    for(auto & w : arr2)
    {
        cout << w << " ";
    }
    cout << endl;
}

int main()
{
    test0();
    test1();
    return 0;
}
