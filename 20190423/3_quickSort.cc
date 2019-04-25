#include<iostream>
#include<vector>
#include<string>

using std::cout;
using std::endl;
using std::vector;
using std::string;

template<typename T, typename Compare = std::less<T>>
class QuickSort
{
public:

    void sort(T *arr, int left, int right)
    {

        if(left >= right)
            return;
        int i=left;
        int j=right;
        T tmp = arr[left];
        Compare cmp;
        while(i < j)
        {
            //while(i < j && arr[j] >= tmp)
            while(i < j && cmp(tmp, arr[j]))
            {
                --j;
            }
            Tswap(arr[i], arr[j]);

            //while(i < j && arr[i] <= tmp)
            while(i < j && cmp(arr[i], tmp))
            {
                ++i;
            }
            Tswap(arr[i], arr[j]);
        }
        sort(arr, left, j-1);
        sort(arr, j+1, right);
    }

    void Tswap(T &a, T &b)
    {
        T tmp = b;
        b = a;
        a = tmp;
    }

private:

};


void test1()
{
    string st[]={"beijing", "shanghai", "shenzhen", "guangzhou", "tianjin"};
    QuickSort<string> a; 
    QuickSort<string, std::greater<string>> b;
    a.sort(st, 0, 4);
    for(auto & w : st)
    {
        cout << w << " ";
    }
    cout << endl;

    b.sort(st, 0, 4);
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
    QuickSort<int> a;
    QuickSort<int ,std::greater<int>> b;
    a.sort(arr, 0, 7);
    for(auto & w : arr)
    {
        cout << w << " ";
    }
    cout << endl;
    b.sort(arr2, 0, 4);
    for(auto &w:arr2)
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
