#include <new>
#ifndef __MYCONSTRUCT__
#define __MYCONSTRUCT__
#include "__type_traits.h"
namespace mySTL{
    template<class T1, class T2>
    inline void construct(T1* p, const T2& val){
        new (p) T1(val);
    }

    template<class T>
    inline void destroy(T *p){
        p->~T();
    }

    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator end, __false_type){
        for(; first < end; ++first)
            destroy(&*first);
    }

    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator end, __true_type){}

    template<class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator end, T*){
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, end, trivial_destructor());
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator end){
        __destroy(first, end, value_type(first));
    }
}
#endif