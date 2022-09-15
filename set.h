#ifndef __MYSET__
#define __MYSET__
#include "__alloc.h"
#include "__rb_tree.h"

namespace mySTL{
    template<class Value, class Compare = less<int>, class Alloc = alloc>
    class set{
        private:
            typedef rb_tree<Value, Value, identity<Value>, Compare, Alloc> rbTree;
            rbTree t;
        public:
            typedef Value value_type;
            typedef Value* pointer;
            typedef Value& reference;
            typedef const Value& const_reference;
            typedef typename rbTree::const_iterator iterator;
            typedef size_t size_type;
            set(){}
            ~set(){
                t.~rb_tree();
            }
            set(const Compare& comp):t(comp){}
            bool empty()const{return t.empty();}
            size_type size()const{return t.size();}
            size_type max_size()const{return t.max_size();}
            iterator begin()const{return iterator(t.begin());}
            iterator end()const{return iterator(t.end());}
            iterator find(const_reference x)const{return t.find(x);}
            size_type count(const_reference x)const{return t.count(x);}
            iterator lower_bound(const_reference x)const{return t.lower_bound(x);}
            iterator upper_bound(const_reference x)const{return t.upper_bound(x);}
            void insert(const_reference x){t.insert_unique(x);}
            size_type erase(const_reference x){return t.erase_unique(x);}
            void erase(iterator p){
                typedef typename rbTree::iterator iter;
                t.erase(p);
            }
            void clear(){
                t.clear();
            }
    };
};
#endif