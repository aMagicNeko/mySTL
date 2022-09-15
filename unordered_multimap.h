#ifndef __MYUNORDERED_MULTIMAP__
#define __MYUNORDERED_MULTIMAP__
#include "hashtable.h"
#include "functional.h"
#include "__hash_func.h"
namespace mySTL{
    template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
    class unordered_multimap{
        public:
            typedef hashtable<pair<Key, T>, Key, HashFcn, select1st<Key, T>, EqualKey, Alloc> _hashtable;
            typedef typename _hashtable::iterator iterator;
            typedef typename _hashtable::reference reference;
            typedef typename _hashtable::const_reference const_reference;
            typedef typename _hashtable::key_type key_type;
            typedef typename _hashtable::value_type value_type;
            typedef typename _hashtable::size_type size_type;
        private:
            _hashtable ht;
        public:
            unordered_multimap():ht(100, HashFcn(), EqualKey()){}
            iterator begin(){return ht.begin();}
            iterator end(){return ht.end();}
            bool empty()const{return ht.empty();}
            size_type size()const{return ht.size();}
            iterator find(const T& x){return ht.find(value_type(x, T()));}
            size_type erase(const key_type& x){return ht.erase_equal(value_type(x, T()));}
            iterator erase(iterator p){return ht.erase(p);}
            size_type count(const T& x)const{return ht.count(value_type(x, T()));}
            iterator insert(const_reference x){return ht.insert_equal(x);}
            T& operator[](const key_type& k){return (*insert(value_type(k, T()))).second;}
            iterator emplace(const key_type& k, const T& v){return insert(value_type(k, v));}
            void clear(){ht.clear();}
    };
};
#endif