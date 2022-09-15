#ifndef __MYMAP__
#define __MYMAP__
#include "__rb_tree.h"

namespace mySTL{
    template<class Key, class T, class Compare = less<Key>, class Alloc = alloc>
    class map{
        private:
            typedef rb_tree<Key, pair<Key, T>, select1st<Key, T>, Compare, Alloc> rbTree;
            rbTree t;
        public:
            typedef pair<Key, T> value_type;
            typedef typename rbTree::pointer pointer;
            typedef typename rbTree::reference reference;
            typedef typename rbTree::const_reference const_reference;
            typedef typename rbTree::const_iterator const_iterator;
            typedef typename rbTree::iterator iterator;
            typedef typename rbTree::size_type size_type;
            typedef Key key_type;
            map(){}
            ~map(){
                t.~rb_tree();
            }
            map(const Compare& comp):t(comp){}
            bool empty()const{return t.empty();}
            size_type size()const{return t.size();}
            size_type max_size()const{return t.max_size();}
            iterator begin(){return iterator(t.begin());}
            iterator end(){return iterator(t.end());}
            iterator find(const_reference x){return t.find(x);}
            size_type count(const_reference x)const{return t.count(x);}
            iterator lower_bound(const key_type& k){return t.lower_bound(value_type(k, T()));}
            iterator upper_bound(const key_type& k){return t.upper_bound(value_type(k, T()));}
            void clear(){
                t.clear();
            }
            pair<iterator, bool> insert(const_reference x){
                return t.insert_unique(x);
            }
            T& operator[](const key_type& k){
                reference x = *(insert(value_type(k, T())).first);
                return x.second;
            }
            void erase(iterator position){t.erase(position);}
            size_type erase(const key_type& k){t.erase_unique(value_type(k, T()));}
            iterator find(const key_type& k){return t.find(value_type(k, T()));}
            size_type count(const key_type& k)const{return t.count(value_type(k, T()));}
    };
};
#endif