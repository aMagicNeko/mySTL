#ifndef __MYQUEUE__
#define  __MYQUEUE__
#include "deque.h"
namespace mySTL{
    template<class T, class Sequence = deque<T>>
    class queue{
    protected:
        Sequence c;
    public:
        bool empty()const{return c.empty();}
        size_t size()const{return c.size();}
        T& front(){return c.front();}
        const T& front()const{return c.front();} 
        T& back(){return c.back();}
        const T& back()const{return c.back();}
        void push(const T& x){
            c.push_back(x);
        }
        void pop(){
            c.pop_front();
        }
    };
}
#endif