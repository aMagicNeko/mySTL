#ifndef __MYALGORITHM__
#define __MYALGORITHM__
#include "iterator.h"
#include "__type_traits.h"
//有待完成。
namespace mySTL{
    template<class InputIterator, class OutputIterator, class T, class Iterator_catagory>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, T*, random_access_iterator_tag, __true_type){
        
    }

    template<class InputIterator, class OutputIterator, class T>
    inline OutputIterator __copy_aux(InputIterator first, InputIterator last, OutputIterator result, T*){
        typedef typename __type_traits<T>::has_trivial_assignment_constructor has_trivial_assignment_constructor;
        __copy(first, last, result, iterator_catagory(InputIterator), value_type(InputIterator), iterator_category(InputIterator), has_trivial_assignment_constructor);
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result){
        return __copy_aux(first, last, result, value_type(InputIterator));
    }

};

#endif