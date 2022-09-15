#ifndef __MYHASHTABLE__
#define __MYHASHTABLE__
#include "__alloc.h"
#include "__construct.h"
#include "iterator.h"
#include "vector.h"
#include <algorithm>
#include "functional.h"
namespace mySTL{
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
    class hashtable;
    template<class Value>
    struct __hashtable_node{
        __hashtable_node* next;
        Value val;
    };
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
    struct __hashtable_const_iterator;
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
    struct __hashtable_iterator{
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> _hashtable;
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        typedef __hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef Value& reference;
        typedef Value* pointer;

        node* cur;
        _hashtable* ht;
        __hashtable_iterator(node* n, _hashtable* h):cur(n), ht(h){}
        __hashtable_iterator(){}
        __hashtable_iterator(const iterator& p):cur(p.cur), ht(p.ht){}
        reference operator*()const{
            return cur->val;
        }
        pointer operator->()const{
            return &(cur->val);
        }
        iterator& operator++(){
            cur = ht->next_element(cur);
            return *this;
        }
        iterator operator++(int){
            iterator tmp(*this);
            ++*this;
            return tmp;
        }
        bool operator==(const iterator& p)const{
            return cur == p.cur;
        }
        bool operator!=(const iterator& p)const{
            return cur != p.cur;
        }
    };
    
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_const_iterator{
       typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> _hashtable;
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        typedef __hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef const Value& reference;
        typedef const Value* pointer;

        const node* cur;
        _hashtable* ht;
        __hashtable_const_iterator(node* n, _hashtable* h):cur(n), ht(h){}
        __hashtable_const_iterator(){}
        __hashtable_const_iterator(const iterator& p):cur(p.cur), ht(p.ht){}
        __hashtable_const_iterator(const const_iterator& p):cur(p.cur), ht(p.ht){}
        reference operator*()const{
            return cur->val;
        }
        reference operator->()const{
            return &(cur->val);
        }
        const_iterator& operator++(){
            cur = ht->next_element(cur);
            return *this;
        }
        const_iterator operator++(int){
            const_iterator tmp(*this);
            ++*this;
            return tmp;
        }
        bool operator==(const const_iterator& p)const{
            return cur == p.cur;
        }
        bool operator!=(const const_iterator& p)const{
            return cur != p.cur;
        }
    };

    enum{__num_primes = 28};

    static const unsigned long __prime_list[__num_primes] ={
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul
    };

    inline unsigned long __next_prime(unsigned long __n){
        const unsigned long* __first = __prime_list;
        const unsigned long* __last = __prime_list + (int)__num_primes;
        const unsigned long* pos = std::lower_bound(__first, __last, __n);
        return pos == __last ? *(__last - 1) : *pos;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    class hashtable{
        public:
            typedef HashFcn hasher;
            typedef EqualKey key_equal;
            typedef size_t size_type;
            typedef Value value_type;
            typedef Value* pointer;
            typedef Value& reference;
            typedef Key key_type;
            typedef const Value& const_reference;
            typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
            typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        private:
            typedef __hashtable_node<Value> node;
            typedef simple_alloc<node, Alloc> node_allocator;
            hasher hash;
            key_equal equals;
            ExtractKey get_key;
            vector<node*, Alloc> buckets;
            size_type num_elements;
            node* get_node(const_reference x){
                node* n = node_allocator::allocate();
                n->next = 0;
                construct(&n->val, x);
                return n;
            }
            void put_node(node* n){
                destroy(&n->val);
                node_allocator::deallocate(n);
            }
            void initialize_buckets(size_type n){
                size_type num_buckets = __next_prime(n);
                buckets.resize(n, 0);
            }
            void resize(size_type num_elements_hint);
        public:
            size_type bucket_index(const_reference x)const{
                return hash(get_key(x))%buckets.size();
            }
            size_type bucket_count()const{return buckets.size();}
            size_type max_bucket_size()const{return __prime_list[__num_primes-1];}
            hashtable(size_type n, const hasher& h, const key_equal& ke):equals(ke), hash(h), num_elements(0){
                initialize_buckets(n);
            }
            ~hashtable(){
                clear();
            }
            pair<iterator, bool> insert_unique(const_reference x);
            iterator insert_equal(const_reference x);
            void clear();
            iterator erase(iterator);
            size_type erase_unique(const_reference x);
            size_type erase_equal(const_reference x);
            size_type count(const_reference x)const;
            iterator find(const_reference x);
            iterator begin();
            iterator end(){
                return iterator(0, this);
            }
            node* next_element(node* n);
            const node* next_element(const node* n);
            size_type size()const{return num_elements;}
            bool empty()const{return num_elements == 0;}
    };
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint){
        size_type old_bucket_size = buckets.size();
        if(old_bucket_size < num_elements_hint){
            size_type n = __next_prime(num_elements_hint);
            vector<node*, Alloc> tmp(n, 0);
            for(size_type bucket = 0; bucket < old_bucket_size; ++bucket){
                node* first = buckets[bucket];
                while(first){
                    size_type new_bucket = hash(get_key(first->val))%n;
                    buckets[bucket] = first->next;
                    first->next = tmp[new_bucket];
                    tmp[new_bucket] = first;
                    first = buckets[bucket];
                }
            }
            swap(tmp, buckets);
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const_reference x){
        resize(num_elements+1);
        Key key = get_key(x);
        const size_type bucket = hash(key)%buckets.size();
        node* first = buckets[bucket];
        while(first){
            if(equals(get_key(first->val), key))
                return pair<iterator, bool>(iterator(first, this), false);
            first = first->next;
        }
        node* n = get_node(x);
        n->next = buckets[bucket];
        buckets[bucket] = n;
        ++num_elements;
        return pair<iterator, bool>(iterator(n, this), true);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const_reference x){
        resize(num_elements+1);
        Key key = get_key(x);
        const size_type bucket = hash(key)%buckets.size();
        node* first = buckets[bucket];
        node* n = get_node(x);
        while(first){
            if(equals(get_key(first->val), key)){
                n->next = first->next;
                first->next = n;
                ++num_elements;
                return iterator(n, this);
            }
            first = first->next;
        }
        n->next = buckets[bucket];
        buckets[bucket] = n;
        ++num_elements;
        return iterator(n, this);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear(){
        for(size_type i = 0; i < buckets.size(); ++i){
            node* first = buckets[i];
            while(first){
                node* tmp = first;
                first = first->next;
                put_node(tmp);
            }
        }
        buckets.resize(__prime_list[0]);
        num_elements = 0;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const_reference x)const{
        const size_type bucket = bucket_index(x);
        const node* first = const_cast<vector<node*, Alloc>&>(buckets)[bucket];
        bool flag = false;
        size_type res = 0;
        while(first){
            if(equals(get_key(first->val), get_key(x))){
                ++res;
                flag = true;
            }
            else if(flag)
                break;
            first = first->next;
        }
        return res;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator p){
        const size_type bucket = bucket_index(*p);
        node* first = buckets[bucket];
        node* n = p.cur;
        iterator tmp = p;
        ++tmp;
        if(first == n){
            buckets[bucket] = n->next;
            put_node(n);
            --num_elements;
            return tmp;
        }
        while(first){
            if(first->next == n)
                break;
            first = first->next;
        }
        first->next = n->next;
        --num_elements;
        put_node(n);
        return tmp;
    }
    
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_equal(const_reference x){
        iterator p = find(x);
        Key key = get_key(x);
        size_type res = 0;
        while(p.cur && equals(get_key(*p), key)){
            p = erase(p);
            ++res;
        }
        return res;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_unique(const_reference x){
        iterator p = find(x);
        Key key = get_key(x);
        size_type res = 0;
        if(p.cur && equals(get_key(*p), key)){
            erase(p);
            ++res;
        }
        return res;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const_reference x){
        const size_type bucket = bucket_index(x);
        node* first = buckets[bucket];
        Key key = get_key(x);
        while(first){
            if(equals(get_key((first->val)), key))
                return iterator(first, this);
        }
        return end();
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin(){
        if(num_elements == 0)
            return end();
        for(size_type i = 0;; ++i)
            if(buckets[i])
                return iterator(buckets[i], this);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::next_element(node* n){
        if(n->next)
            return n->next;
        size_type bucket = bucket_index(n->val)+1;
        while(bucket < buckets.size()){
            if(buckets[bucket])
                return buckets[bucket];
            ++bucket;
        }
        return 0;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    const typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::next_element(const node* n){
        if(n->next)
            return n->next;
        size_type bucket = bucket_index(n->val)+1;
        while(bucket < buckets.size()){
            if(buckets[bucket])
                return buckets[bucket];
            ++bucket;
        }
        return 0;
    }

};
#endif