#ifndef __MYITERATOR__
#define __MYITERATOR__
#include <new>
namespace mySTL{
    class input_iterator_tag{};
    class output_iterator_tag{};
    class forward_iterator_tag : public input_iterator_tag, public output_iterator_tag{};
    class bidirectional_iterator_tag : public forward_iterator_tag{};
    class random_access_iterator_tag : public bidirectional_iterator_tag{};

    template<class T, class Category, class Distance = ptrdiff_t, class Reference = T&, class Pointer = T*>
    class iterator{
    public:
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    template<class Iterator>
    class iterator_traits{
    public:
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    template<class T>
    class iterator_traits<T*>{
    public:
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    template<class T>
    class iterator_traits<const T*>{
    public:
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&){
        typedef typename iterator_traits<Iterator>::value_type value_type;
        return static_cast<value_type*>(0);
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&){
        return iterator_traits<Iterator>::iterator_category();
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&){
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        return static_cast<difference_type*>(0);
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag){
        int n = 0;
        while(first != last){
            ++first;
            ++n;
        }
        return n;
    }

    template<class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
        return last-first;
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last){
        typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
        return __distance(first, last, iterator_category());
    }

    template<class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
        if(n > 0)
            while(n--)
                ++i;
        else
            while(n++)
                --i;
    }

    template<class RandomAccessIterator, class Distance>
    inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag){
        i += n;
    }

    template<class InputIterator, class Distance>
    inline void advance(InputIterator& i, Distance n){
        return __advance(i, n, iterator_traits<InputIterator>::iterator_category);
    }
}
#endif