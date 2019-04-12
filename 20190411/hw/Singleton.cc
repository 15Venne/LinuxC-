#include<iostream>
using std::cout;
using std::endl;

/* class Singleton */
/* { */
/* public: */
/*     static Singleton *getInstance() */
/*     { */
/*         if(!_pInstance) */
/*         { */
/*             _pInstance = new Singleton(); */
/*         } */
/*         return _pInstance; */
/*     } */

/*     static void Sfree() */
/*     { */
/*         if(_pInstance) */
/*         { */
/*             delete _pInstance; */
/*         } */
/*     } */
/* private: */
/*     Singleton(){} */
/*     ~Singleton(){} */
/*     static Singleton *_pInstance; */
/* }; */

/* Singleton * Singleton::_pInstance = nullptr; */

/* int main() */
/* { */
/*     Singleton *p1 = Singleton::getInstance(); */
/*     Singleton *p2 = Singleton::getInstance(); */
/*     cout << "p1 = " << p1 << endl */
/*          << "p2 = " << p2 << endl; */

/*     Singleton::Sfree(); */
/*     Singleton::Sfree(); */
/*     return 0; */
/* } */

class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(!_pInstance)
        {
            _pInstance = new Singleton();
        }
        return _pInstance;
    }

    static void Sfree()
    {
        if(_pInstance)
        {
            delete _pInstance;
            _pInstance = nullptr;
            cout << "static void Sfree()" << endl;
        }
    }

    void print() const
    {
        cout << "Singleton::print()" << endl;
    }
private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }
    ~Singleton()
    {
        cout << "~Singleton()" << endl; 
    }
    static Singleton *_pInstance;
};

Singleton *Singleton::_pInstance = nullptr;

int main()
{
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
    cout << "p1 = " << p1 << endl
         << "p2 = " << p2 << endl;
    
    Singleton::getInstance()->print();
    //Singleton::Sfree();
    p1->Sfree();
    p2->Sfree();
    return 0;
}
