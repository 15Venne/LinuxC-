#include<iostream>
#include<string>

using std::cout;
using std::endl;
using std::string;

template <typename T, typename Compare=std::less<T>>
class MergeSort
{
public:
    int min(int x, int y)
    {
        return x < y ? x : y;
    }
    void sort(T *arr, int size)
    {
        T *a = arr;
        T *b = new T[size]();
        Compare cmp;
        for(int i = 1; i < size; i += i)
        {
            for(int j = 0; j < size; j += i*2)
            {
                int left = j;
                int mid = min(j+i, size);
                int right = min(j+i*2, size);
                int k = j;
                int start1 = j, end1 = mid;
                int start2 = mid, end2 = right;
                while(start1 < end1 && start2 < end2)
                {
                    //if(a[start1] < a[start2])
                    if(cmp(a[start1], a[start2]))
                    {
                        b[k] = a[start1];
                        ++start1;
                    } else{
                        b[k] = a[start2];
                        ++start2;
                    }
                    ++k;
                }
                while(start1 < end1)
                {
                    b[k] = a[start1];
                    ++k; ++start1;
                }
                while(start2 < end2)
                {
                    b[k] = a[start2];
                    ++k; ++start2;
                }
            }
            T *temp = a;
            a = b;
            b = temp;
        }
        if(a != arr)
        {
            for(int i = 0; i < size; ++i)
            {
                b[i] = a[i];
            }
            b = a;
        }
        delete [] b;
    }
private:

};

void test1()
{
    string st[]={"beijing", "shanghai", "shenzhen", "guangzhou", "tianjin"};
    MergeSort<string> a; 
    MergeSort<string, std::greater<string>> b;
    a.sort(st, 5);
    for(auto & w : st)
    {
        cout << w << " ";
    }
    cout << endl;

    b.sort(st, 5);
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
    MergeSort<int> a;
    MergeSort<int ,std::greater<int>> b;
    a.sort(arr, 8);
    for(auto & w : arr)
    {
        cout << w << " ";
    }
    cout << endl;
    b.sort(arr2, 5);
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
}
