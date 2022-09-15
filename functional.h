#ifndef __MYFUNCTIONAL__
#define __MYFUNCTIONAL__
namespace mySTL{
    template<class T>
    struct identity{
        const T& operator()(const T& x)const{
            return x;
        }
    };
    template<class T>
    struct less{
        bool operator()(const T& x, const T& y)const{
            return x < y;
        }
    };
    template<class T1, class T2>
    struct pair{
        T1 first;
        T2 second;
        pair(const T1& x, const T2& y):first(x), second(y){}
        pair(){}
        pair(const pair<T1, T2>& p):first(p.first), second(p.second){}
    };
    template<class T1, class T2>
    struct select1st{
        const T1& operator()(const pair<T1, T2>& p)const{
            return p.first;
        }
    };
    template<class T>
    struct equal_to{
        bool operator()(const T& x, const T& y)const{
            return x == y;
        }
    };
    template<class T>
    struct multiply{
        T operator()(const T& x, const T& y)const{
            return x*y;
        }
    };
};

#endif
