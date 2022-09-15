#ifndef __MYVECTOR__
#define __MYVECTOR__
#include "__alloc.h"
#include "iterator.h"
#include "__uninitialized.h"
#include <algorithm>
namespace mySTL{
    template<class T, class Alloc = alloc>
    class vector;
    template<class T, class Alloc>
    void swap(vector<T, Alloc>& v1, vector<T, Alloc>& v2){
        typename vector<T, Alloc>::iterator tmp = v1.start;
        v1.start = v2.start;
        v2.start = tmp;
        tmp = v1.finish;
        v1.finish = v2.finish;
        v2.finish = tmp;
        tmp = v1.end_of_storage;
        v1.end_of_storage = v2.end_of_storage;
        v2.end_of_storage = tmp;
    }
    template<class T, class Alloc>
    class vector{
    public:
        typedef T* iterator;
        typedef size_t size_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef ptrdiff_t difference_type;
        typedef const T& const_reference;
    protected:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        typedef simple_alloc<T, Alloc> data_allocator;
        void allocate_and_fill(size_t n, const T& val){
            start = data_allocator::allocate(n);
            finish = end_of_storage = start+n;
            my_uninitialized_fill_n(start, n, val);
        }
        void insert_aux(iterator position, const T& x);
    public:
        iterator begin(){return start;}
        iterator end(){return finish;}
        size_type size()const{return (finish-start);}
        size_type capacity()const{return (end_of_storage-start);}
        bool empty()const{return start==finish;}
        reference operator[](size_type n){return *(start+n);}

        vector():start(0), finish(0), end_of_storage(0){}
        vector(size_type n, const T& val){allocate_and_fill(n, val);}
        explicit vector(size_type n){allocate_and_fill(n, T());}

        ~vector(){
            destroy(start, finish);
            data_allocator::deallocate(start, end_of_storage-start);
        }
        reference front(){
            return *start;
        }
        reference back(){
            return *(finish-1);
        }
        const_reference front()const{
            return *start;
        }
        const_reference back()const{
            return *(finish-1);
        }
        void push_back(const T& x);
        void pop_back(){
            --finish;
            destroy(finish);
        }
        iterator erase(iterator first, iterator last){
            destroy(first, last);
            iterator i = uninitialized_copy(last, finish, first);
            destroy(i, finish);
            finish = i;
            return first;
        }
        iterator erase(iterator position){
            destroy(position);
            uninitialized_copy(position+1, finish, position);
            --finish;
            destroy(finish);
            return position;
        }
        iterator insert(iterator position, size_type n, const T& x);
        void resize(size_type new_size, const T& x){
            if(new_size < size())
                erase(start+new_size, finish);
            else
                insert(finish, new_size-size(), x);
        }
        void resize(size_type new_size){
            resize(new_size, T());
        }
        void clear(){
            erase(start, finish);
        }
        friend void swap<T, Alloc>(vector<T, Alloc>& v1, vector<T, Alloc>& v2);
    };


    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const T& x){
        if(finish != end_of_storage){
            construct(finish, x);
            ++finish;
        }
        else
            insert_aux(finish, x);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T& x){
        if(finish != end_of_storage){
            construct(finish, *(finish-1));
            ++finish;
            uninitialized_copy(position, finish-2, position+1);
            construct(position, x);
        }
        else{
            const size_type old_size = finish-start;
            size_type new_size = old_size?2*old_size:1;
            iterator new_finish;
            iterator new_start = data_allocator::allocate(new_size);
            try{
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...){
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy(start, finish);
            data_allocator::deallocate(start, end_of_storage-start);
            start = new_start;
            finish = new_finish;
            end_of_storage = start+new_size;
        }
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, size_type n, const T& x){
        if(n == 0)
            return position;
        size_type spare_space = end_of_storage-finish;
        iterator res;
        if(spare_space >= n){
            uninitialized_copy(position, finish, position+n);
            my_uninitialized_fill_n(position, n, x);
            finish += n;
            res = position+n;
        }
        else{
            const size_type old_size = end_of_storage-start;
            const size_type new_size = old_size?2*(old_size+n):2*n;
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try{
                new_finish = uninitialized_copy(start, position, new_start);
                res = new_finish = my_uninitialized_fill_n(new_finish, n, x);
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...){
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy(start, finish);
            data_allocator::deallocate(start, end_of_storage-start);
            start = new_start;
            finish = new_finish;
            end_of_storage = start+new_size;
        }
        return res;
    }
}
#endif