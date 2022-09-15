#ifndef __MYUNINITIALIZED__
#define __MYUNINITIALIZED__
#include "__construct.h"
#include "__type_traits.h"
#include "iterator.h"
#include <algorithm>
namespace mySTL{
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result);

    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val);

    template<class ForwardIterator, class T, class Size>
    inline ForwardIterator my_uninitialized_fill_n(ForwardIterator first, Size n, const T& val);

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type){
        return std::copy(first, last, result);
    }

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type){
        for(;first != last; ++first, ++result)
            contruct(&*result, *first);
        return result;
    }

    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T* p){
        typedef typename __type_traits<T>::is_POD_type is_POD_type;
        return __uninitialized_copy_aux(first, last, result, is_POD_type());
    }

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        return __uninitialized_copy(first, last, result, value_type(result));
    }

    template<class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __true_type){
        std::fill(first, last, val);
    }

    template<class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __false_type){
        for(; first != last; ++first)
            construct(&*first, val);
    }

    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val){
        typedef typename __type_traits<T>::is_POD_type is_POD_type;
        __uninitialized_fill_aux(first, last, val, is_POD_type());
    }

    template<class ForwardIterator, class T, class Size>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& val, __true_type){
        std::fill_n(first, n, val);
        return first+n;
    }

    template<class ForwardIterator, class T, class Size>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& val, __false_type){
        for(; n > 0; --n, ++first)
            construct(&*first, val);
        return first;
    }

    template<class ForwardIterator, class T, class Size>
    inline ForwardIterator my_uninitialized_fill_n(ForwardIterator first, Size n, const T& val){
        typedef typename __type_traits<T>::is_POD_type is_POD_type;
        return __uninitialized_fill_n_aux(first, n, val, is_POD_type());
    }
}
#endif