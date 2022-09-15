#ifndef __MYNUMERIC__
#define __MYNUMERIC__
#include "functional.h"
namespace mySTL{
    template<class InputIterator, class T>
    T accumulate(InputIterator first, InputIterator last, T init){
        for(; first != last; ++first)
            init = init+*first;
        return init;
    }

    template<class InputIterator, class T, class BinaryOperation>
    T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op){
        for(; first != last; ++first)
            init = binary_op(init, *first);
        return init;
    }

    template<class InputIterator, class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result){
        if(first == last)
            return result;
        InputIterator::value_type val = *first;
        *result = val;
        while(++first != last){
            InputIterator::value_type tmp = *first;
            *++result = tmp-val;
            val = tmp;
        }
        return ++result;
    }

    template<class InputIterator, class OutputIterator, class BinaryOperation>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op){
        if(first == last)
            return result;
        InputIterator::value_type val = *first;
        *result = val;
        while(++first != last){
            InputIterator::value_type tmp = *first;
            *++result = binary_op(tmp, val);
            val = tmp;
        }
        return ++result;
    }

    template <class InputIterator1, class InputIterator2, class T>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init){
        for( ; first1 != last1; ++first1, ++first2)
            init = init + (*first1 * *first2);
        return init;
    }

    template <class InputIterator1, class InputIterator2, class T,class BinaryOperation1, class BinaryOperation2>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2){
        for( ; first1 != last1; ++first1, ++first2)
            init = binary_op1(init, binary_op2(*first1, *first2));
        return init;
    }

    template <class InputIterator, class OutputIterator, class T>
    OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*){
        T value = *first;
        while(++first != last) {
            value = value + *first;
            *++result = value;
        }
        return ++result;
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result){
        if(first == last) return result;
        *result = *first;
        return __partial_sum(first, last, result, value_type(first));
    }

    template <class InputIterator, class OutputIterator, class T,
            class BinaryOperation>
    OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op){
        T value = *first;
        while(++first != last) {
            value = binary_op(value, *first);
            *++result = value;
        }
        return ++result;
    }

    template <class InputIterator, class OutputIterator, class BinaryOperation>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op){
        if(first == last) return result;
        *result = *first;
        return __partial_sum(first, last, result, value_type(first), 
                            binary_op);
    }

    template <class T, class Integer, class MonoidOperation>
    T __power(T x, Integer n, MonoidOperation opr){
        if(n == 0)
            return identity_element(opr);
        else{
            while((n & 1) == 0){
                n >>= 1;
                x = opr(x, x);
            }
            T result = x;
            n >>= 1;
            while(n != 0){
                x = opr(x, x);
                if((n & 1) != 0)
                    result = opr(result, x);
                n >>= 1;
            }
            return result;
        }
    }

    template <class T, class Integer>
    inline T __power(T x, Integer n){
        return __power(x, n, multiply<T>());
    }

    template <class T, class Integer, class MonoidOperation>
    inline T power(T x, Integer n, MonoidOperation opr){
        return __power(x, n, opr);
    }

    template <class T, class Integer>
    inline T power(T x, Integer n){
        return __power(x, n);
    }

    template <class ForwardIter, class T>
    void iota(ForwardIter first, ForwardIter last, T value){
        while(first != last)
            *first++ = value++;
    }
};
#endif