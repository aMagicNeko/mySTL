#ifndef __MYSTACK__
#define __MYSTACK__
#include "deque.h"
namespace mySTL{
    template<class T, class Alloc = alloc>
    class stack:private deque<T, Alloc>{
    public:
        bool empty()const{
            return deque<T, alloc>::empty();
        }
        void push(const T& x){
            deque<T, alloc>::push_back(x);
        }
        void pop(){
            deque<T, alloc>::pop_back();
        }
        size_t size()const{
            return deque<T, alloc>::size();
        }
        T& top(){
            return deque<T, alloc>::back();
        }
        const T& top()const{
            return deque<T, alloc>::back();
        }
    };
}
#endif