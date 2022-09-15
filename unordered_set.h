#ifndef __MYUNORDEREDSET__
#define __MYUNORDEREDSET__
#include "hashtable.h"
#include "__alloc.h"
#include "__hash_func.h"
#include "functional.h"
namespace mySTL{
    template<class Value, class Hash = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
    class unordered_set{
        public:
            typedef Value value_type;
            typedef hashtable<Value, Value, Hash, identity<Value>, EqualKey, Alloc> _hashtable;
            typedef typename _hashtable::const_iterator iterator;
            typedef typename _hashtable::const_reference reference;
            typedef typename _hashtable::const_reference const_reference;
            typedef const typename _hashtable::pointer pointer;
            typedef typename _hashtable::size_type size_type;
        private:
            _hashtable ht;
        public:
            unordered_set():ht(100, Hash(), EqualKey()){}
            iterator begin(){return iterator(ht.begin());}
            iterator end(){return iterator(ht.end());}
            iterator erase(iterator p){return iterator(ht.erase(_hashtable::iterator(p)));}
            iterator find(const_reference x){return iterator(ht.find(x));}
            size_type erase(const_reference x){return ht.erase_unique(x);}
            iterator insert(const_reference x){
                pair<typename _hashtable::iterator, bool> ret(ht.insert_unique(x));
                if(ret.second)
                    return iterator(ret.first);
                else return end();
            }
            size_type count(const_reference x){
                return ht.count(x);
            }
            size_type size()const{return ht.size();}
            bool empty()const{return ht.empty();}
            void clear(ht.clear());
    };
};

#endif