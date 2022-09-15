#ifndef __MYDEQUE__
#define __MYDEQUE__
#include "iterator.h"
#include "__alloc.h"
#include "__uninitialized.h"
namespace mySTL{
    template<class T, class Reference, class Ptr, size_t BufSize>
    struct __deque_iterator{
        //cur will not equel to last
        typedef __deque_iterator self;
        typedef T value_type;
        typedef Reference reference;
        typedef Ptr pointer;
        typedef random_access_iterator_tag iterator_category;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T** map_pointer;

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        reference operator*()const{
            return *cur;
        }
        pointer operator->()const{
            return &(*cur);
        }
        difference_type operator-(const self& x)const{
            return (node-x.node)*BufSize+(cur-first)-(x.cur-x.first);
        }
        self& operator++(){
            ++cur;
            if(cur == last){
                ++node;
                first = *node;
                last = first+BufSize;
                cur = first;
            }
            return *this;
        }
        self operator++(int)const{
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--(){
            if(cur == first){
                --node;
                first = *node;
                last = first+BufSize;
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--()const{
            self tmp = *this;
            --*this;
            return tmp;
        }
        self& operator+=(difference_type n){
            n += cur-first;
            cur = first;
            if(n < 0)
                return *this -= -n;
            if(n >= BufSize){
                node += n/BufSize;
                first = *node;
                last = first+BufSize;
                n %= BufSize;
            }
            cur = first+n;
            return *this;
        }
        self operator+(difference_type n)const{
            self tmp = *this;
            tmp += n;
            return tmp;
        }
        self& operator-=(difference_type n){
            n -= cur-first;
            cur = first;
            if(n < 0)
                return *this += -n;
            node -= n/BufSize;
            n %= BufSize;
            if(n != 0){
                --node;
                n = BufSize-n;
            }
            first = *node;
            last = first+BufSize;
            cur = first+n;
            return *this;
        }
        self operator-(difference_type n)const{
            self tmp = *this;
            tmp -= n;
            return tmp;
        }
        reference operator[](difference_type n)const{
            return *(*this+n);
        }
        bool operator==(const self& x)const{
            return x.cur == cur;
        }
        bool operator!=(const self& x)const{
            return x.cur != cur;
        }
        bool operator<(const self& x)const{
            if(node != x.node)
                return node < x.node;
            return cur < x.cur;
        }
        void set_node(map_pointer p){
            cur = *p+(cur-first);
            first = *p;
            last = *p+BufSize;
            node = p;
        }
    };

    template<class T, class Alloc = alloc, size_t BufSize=512>
    class deque{
    public:
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef const T& const_reference;
        typedef __deque_iterator<T, reference, pointer, BufSize> iterator;

    protected:
        typedef pointer* map_pointer;
        map_pointer map;
        size_type map_size;
        iterator start;
        iterator finish;//always point t
        typedef simple_alloc<value_type> data_allocator;
        typedef simple_alloc<pointer> map_allocator;
        void create_map_and_nodes(size_type num_elements);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
    public:
        void clear();
        iterator begin(){return start;}
        iterator end(){return finish;}
        reference operator[](size_type n){
            return start[n];
        }
        const reference front()const{return *start;}
        const reference back()const{iterator tmp = finish; --tmp; return *tmp;}
        reference front(){return *start;}
        reference back(){iterator tmp = finish; --tmp; return *tmp;}
        size_type size()const{return finish-start;}
        size_type max_size()const{return size_type(-1);}
        bool empty()const{return start == finish;}
        deque(size_type n, const value_type& x);
        deque(){
            create_map_and_nodes(0);
        }
        void push_back(const value_type& x);
        void push_front(const value_type& x);
        void pop_back(){
            --finish;
            destroy(finish.cur);
            if(finish.cur == finish.first && finish.node != start.node){
                data_allocator::deallocate(finish.first, BufSize);
                *finish.node = 0;
                finish.set_node(finish.node);
            }
        }
        void pop_front(){
            destroy(start.cur);
            if(start.cur == start.last-1 && start.node != finish.node){
                data_allocator::deallocate(start.first, BufSize);
                *start.node = 0;
                start.set_node(start.node);
            }
            ++start;
        }
        iterator erase(iterator first, iterator last);
        iterator erase(iterator p);
        ~deque(){
            clear();
            data_allocator::deallocate(start.first, BufSize);
            map_allocator::deallocate(map, map_size);
        }
        iterator insert(iterator p, const value_type& x);
    };

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::insert(iterator p, const value_type& x){
        if(p == start){
            push_front(x);
            return start;
        }
        else if(p == finish){
            push_back(x);
            return finish-1;
        }
        else if(p-start < finish-p){
            push_front(*start);
            std::copy(start+2, p, start+1);
            --p;
            construct(p.cur, x);
            return p;
        }
        else{
            push_back(*(finish-1));
            std::copy_backward(p, finish-2, finish-1);
            construct(p.cur, x);
            return p;
        }
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(iterator first, iterator last){
        if(first == start && last == finish){
            clear();
            return start;
        }
        difference_type n = last-first;
        int elements_before = first-start;
        if(first-start > finish-last){
            std::copy(last, finish, first);
            iterator new_finish = finish-n;
            destroy(new_finish, finish);
            for(map_pointer p = new_finish.node+1; p < finish.node; ++p)
                data_allocator::deallocate(*p, BufSize);
            if(finish.cur != finish.first && new_finish.node != finish.node)
                data_allocator::deallocate(finish.first, BufSize);
            if(new_finish.cur == new_finish.first && new_finish.node != start.node)
                data_allocator::deallocate(new_finish.first, BufSize);
            finish = new_finish;
        }
        else{
            std::copy_backward(start, first, last);
            iterator new_start = start+n;
            destroy(start, new_start);
            for(map_pointer p = start.node+1; p < new_start.node; ++p)
                data_allocator::deallocate(*p, BufSize);
            if(start.node != finish.node)
                data_allocator::deallocate(start.first, BufSize);
            start = new_start;
        }
        return start+elements_before;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(iterator p){
        iterator next = p;
        ++next;
        if(p-start > finish-p){
            std::copy(next, finish, p);
            pop_back();
            return p;
        }
        else{
            std::copy_backward(start, p, next);
            pop_front();
            return next;
        }
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::clear(){
        if(start.node == finish.node){
            destroy(start.cur, finish.cur);
            finish.cur = start.cur;
            return;
        }
        for(map_pointer p = start.node+1; p < finish.node; ++p){
            destroy(*p, *p+BufSize);
            data_allocator::deallocate(*p);
        }
        destroy(start.cur, start.last);
        if(finish.first != finish.cur){
            destroy(finish.first, finish.cur);
            data_allocator::deallocate(finish.first, BufSize);
        }
        finish = start;
    }


    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::deque(size_type n, const value_type& x){
        if(n == 0){
            create_map_and_nodes(0);
            return;
        }
        create_map_and_nodes(n);
        for(map_pointer p = start.node; p < finish.node; ++p)
            uninitialized_fill(*p, *p+BufSize, x);
        uninitialized_fill(finish.first, finish.cur, x);
    }
    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements){
        map_size = num_elements/BufSize+3;
        map = map_allocator::allocate(map_size);
        finish.node = map+1+num_elements/BufSize;
        num_elements %= BufSize;
        if(num_elements != 0)
            ++finish.node;
        for(map_pointer p = map+2; p < finish.node; ++p){
            *p = data_allocator::allocate(BufSize);
        }
        start.node = map+1;
        *(start.node) = data_allocator::allocate(BufSize);
        start.first = start.cur = *(map+1);
        start.last = start.first+BufSize;
        if(finish.node == start.node){
            finish.set_node(finish.node);
            finish.cur = finish.first + num_elements;
            return;
        }
        if(num_elements){
            --finish.node;
            finish.first = *finish.node;
            finish.cur = finish.first+num_elements;
            finish.last = finish.first+BufSize;
        }
        else
            finish.first = finish.last = finish.cur = 0;
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_back(const value_type& x){
        if(finish.node == start.node){
            construct(finish.cur, x);
            ++finish;
            return;
        }
        if(finish.node-map == map_size-1){
            reallocate_map(1, 0);
            return push_back(x);
        }
        if(finish.cur == finish.first){
            *finish.node = data_allocator::allocate(BufSize);
            finish.cur = finish.first = *finish.node;
            finish.last = finish.first+BufSize;
        }
        construct(finish.cur, x);
        ++finish;
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_front(const value_type& x){
        if(start.node == map && start.cur == start.first)
            reallocate_map(1, 1);
        if(start.cur == start.first){
            --start.node;
            start.first = *start.node = data_allocator::allocate(BufSize);
            start.last = start.cur = start.first+BufSize;
        }
        --start;
        construct(start.cur, x);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front){
        size_type old_num_nodes = finish.node-start.node;
        size_type new_num_nodes = old_num_nodes+nodes_to_add;
        iterator new_start;
        if(new_num_nodes*2 < map_size+2){
            new_start.node = map+(map_size-new_num_nodes)/2+add_at_front*nodes_to_add;
            if(new_start < start)
                std::copy(start.node, finish.node+1, new_start.node);
            else
                std::copy_backward(start.node, finish.node+1, new_start.node+old_num_nodes+1);
        }
        else{
            size_type new_map_size = map_size+(nodes_to_add<map_size?map_size:nodes_to_add)+3;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start.node = new_map+(new_map_size-new_num_nodes)/2+add_at_front*nodes_to_add;
            std::copy(start.node, finish.node+1, new_start.node);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        finish.node = new_start.node+(finish.node-start.node);
        start.node = new_start.node;
    }
}
#endif