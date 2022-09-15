#ifndef __MYPRIORITY_QUEUE__
#define __MYPRIORITY_QUEUE__
#include "vector.h"
#include "heap.h"
namespace mySTL{
    template<class T, class Compare, class Sequence = vector<T>>
    class priority_queue{
    protected:
        Sequence c;
        Compare comp;
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        priority_queue():c(){}
        priority_queue(const Compare& x):c(), comp(x){}
        bool empty()const{return c.empty();}
        size_type size()const{return c.size();}
        const_reference top()const{return c.front();}
        void push(const value_type& x){
            c.push_back(x);
            mySTL::push_heap(c.begin(), c.end(), comp);
        }
        void pop(){
            mySTL::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
    };

};
#endif