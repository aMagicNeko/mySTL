#ifndef __MYHEAP__
#define __MYHEAP__
#include "iterator.h"
namespace mySTL{
    template<class RandomAccessIterator, class Comp, class Distance, class T>
    void __push_heap(RandomAccessIterator first, Comp comp, Distance holeIndex, Distance topIndex, T value){
        Distance parent = (holeIndex-1)/2;
        while(holeIndex > topIndex && comp(value, *(first+parent))){
            *(first+holeIndex) = *(first+parent);
            holeIndex = parent;
            parent = (holeIndex-1)/2;
        }
        *(first+holeIndex) = value;
    }

    template<class RandomAccessIterator, class Comp, class Distance, class T>
    inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Comp comp, Distance*, T*){
        __push_heap(first, comp, Distance(last-first-1), Distance(0), T(*(last-1)));
    }

    template<class RandomAccessIterator, class Comp>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp){
        __push_heap_aux(first, last, comp, difference_type(first), value_type(first));
    }

    template<class RandomAccessIterator, class Comp, class T, class Distance>
    void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp, Distance*, T*){
        T value = *(last-1);
        *(last-1) = *first;
        Distance holeIndex = 0;
        Distance finalIndex = last-first-2;
        while(true){
            Distance leftChildren = 2*holeIndex+1;
            Distance rightChildren = leftChildren+1;
            if(leftChildren > finalIndex)
                break;
            if(comp(value, *(first+leftChildren)) && comp(value, *(first+rightChildren)))
                break;
            if(comp(*(first+leftChildren), *(first+rightChildren))){
                *(first+holeIndex) = *(first+leftChildren);
                holeIndex = leftChildren;
            }
            else{
                if(rightChildren > finalIndex)
                    break;
                *(first+holeIndex) = *(first+rightChildren);
                holeIndex = rightChildren;
            }
        }
        *(first+holeIndex) = value;
    }

    template<class RandomAccessIterator, class Comp>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp){
        __pop_heap(first, last, comp, difference_type(first), value_type(first));
    }

    template<class RandomAccessIterator, class Comp>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp){
        while(last != first)
            mySTL::pop_heap(first, last--, comp);
    }

    template<class RandomAccessIterator, class Comp>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp){
        if(last-first < 2)
            return;
        RandomAccessIterator cur = first+1;
        while(cur != last)
            mySTL::push_heap(first, ++cur, comp);
    }
}
#endif