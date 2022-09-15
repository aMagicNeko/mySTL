#ifndef __MYLIST__
#define __MYLIST__
#include "__alloc.h"
#include "iterator.h"
#include "__construct.h"
namespace mySTL{
    template<class T>
    struct __list_node{
        typedef __list_node<T>* list_pointer;
        list_pointer next;
        list_pointer prev;
        T data;
    };

    template<class T, class Reference, class Ptr>
    struct __list_iterator{
        typedef __list_iterator<T, Reference, Ptr> self;
        typedef T value_type;
        typedef bidirectional_iterator_tag iterator_category;
        typedef Ptr pointer;
        typedef Reference reference;
        typedef __list_node<T>* link_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        link_type node;

        __list_iterator(link_type x):node(x){}
        __list_iterator(const self& x):node(x.node){}
        __list_iterator(){}

        bool operator==(const self& x)const{return node == x.node;}
        bool operator!=(const self& x)const{return node != x.node;}
        Reference operator*()const{return node->data;}
        Ptr operator->()const{return &(node->data);}
        self& operator++(){node = node->next; return *this;}
        self operator++(int)const{self tmp = *this; node = node->next; return tmp;}
        self& operator--(){node = node->prev; return *this;}
        self operator--(int)const{self tmp = *this; node = node->prev; return tmp;}
    };

    template<class T, class Alloc = alloc>
    class list{
    protected:
        typedef __list_node<T> list_node;
    public:
        typedef list_node* link_type;
    protected:
        link_type node;
        typedef simple_alloc<list_node, Alloc> list_node_allocator;
        link_type get_node(){return list_node_allocator::allocate();}
        void put_node(link_type p){list_node_allocator::deallocate(p);}
        link_type create_node(const T& x){
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        void destroy_node(link_type p){
            destroy(&p->data);
            put_node(p);
        }
    public:
        typedef __list_iterator<T, T&, T*> iterator;
    protected:
        void transfer(iterator position, iterator first, iterator last){
            link_type tmp = last.node->prev;
            first.node->prev->next = last.node;
            last.node->prev = first.node->prev;
            first.node->prev = position.node->prev;
            position.node->prev->next = first.node;
            position.node->prev = tmp;
            tmp->next = position.node;
        }
    public:
        list(){
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        bool empty()const{
            return node == node->next;
        }
        iterator begin(){
            return iterator(node->next);
        }
        iterator end(){
            return iterator(node);
        }
        size_t size(){
            return __distance(begin(), end());
        }
        T& front(){
            return node->next->data;
        }
        T& back(){
            return node->prev->data;
        }
        void push_back(const T& x){
            link_type p = create_node(x);
            p->prev = node->prev;
            p->next = node;
            node->prev->next = p;
            node->prev = p;
        }
        iterator insert(iterator position, const T& x){
            link_type p = create_node(x);
            p->next = position.node;
            p->prev = position.node->prev;
            position.node->prev->next = p;
            return p;
        }
        void push_front(const T&x){
            insert(begin(), x);
        }
        iterator erase(iterator position){
            link_type p = position.node;
            link_type res = p->next;
            p->prev->next = p->next;
            p->next->prev = p->prev;
            destroy_node(p);
            return res;
        }
        void pop_front(){
            erase(begin());
        }
        void pop_back(){
            erase(--end());
        }
        void clear();
        void remove(const T& val);
        void unique();
        void splice(iterator position, list<T, Alloc>& x);
        void splice(iterator, iterator, iterator);
        void merge(list<T, Alloc>& x);
        void sort();
        void reverse();
    };

    template<class T, class Alloc>
    void list<T, Alloc>::clear(){
        link_type cur = node->next;
        while(cur != node){
            link_type tmp = cur->next;
            destroy_node(cur);
            cur = tmp;
        }
        node->next = node;
        node->prev = node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::remove(const T& val){
        link_type cur = node->next;
        while(cur != node){
            link_type tmp = cur;
            cur = cur->next;
            if(tmp->data == val){
                tmp->prev->next = cur;
                cur->prev = tmp->prev;
                destroy_node(tmp);
            }
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::unique(){
        link_type cur = node->next;
        while(cur != node){
            link_type first = cur;
            const T& x = cur->data;
            cur = cur->next;
            while(cur->data == x){
                link_type tmp = cur;
                cur = cur->next;
                destroy_node(tmp);
            }
            cur->prev = first;
            first->next = cur;
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list<T, Alloc>& x){
        if(!x.empty())
            transfer(position, x.begin(), x.end());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, iterator first, iterator last){
        if(first != last)
            transfer(position, first, last);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::merge(list<T, Alloc>& x){
        iterator first = begin();
        iterator last = end();
        iterator first1 = x.begin();
        iterator last1 = x.end();
        while(first != last && first1 != last1){
            if(*first > *first1){
                iterator tmp = first1;
                transfer(first, tmp, ++first1);
            }
            else
                ++first;
        }
        if(first1 != last1)
            transfer(last, first1, last1);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::reverse(){
        iterator first = ++begin();
        iterator last = end();
        if(first == last)
            return;
        while(first != last){
            iterator tmp = first;
            transfer(begin(), tmp, ++first);
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::sort(){
        bool flag = true;
        for(int n = 1; flag; n *= 2){
            flag = false;
            link_type start = node;
            while(true){
                link_type first = start->next;
                if(first == node)
                    break;
                int k = n;
                link_type last = first;
                while(k > 0 && last != node){
                    last = last->next;
                    --k;
                }
                if(last == node)
                    break;
                k = n;
                link_type last1 = last;
                while(k > 0 && last1 != node){
                    last1 = last1->next;
                    --k;
                }
                link_type first1 = last;
                link_type p = start;
                last = last->prev;
                last->next = 0;
                while(first && first1 != last1)
                    if(first->data <= first1->data){
                        p->next = first;
                        first->prev = p;
                        p = first;
                        first = first->next;
                    }
                    else{
                        p->next = first1;
                        first1->prev = p;
                        p = first1;
                        first1 = first1->next;
                    }
                if(first == 0){
                    p->next = first1;
                    first1->prev = p;
                }
                else{
                    p->next = first;
                    first->prev = p;
                    last->next = last1;
                    last1->prev = last;
                }
                flag = true;
                start = last1->prev;
            }
        }
    }
}
#endif