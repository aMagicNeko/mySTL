#ifndef __MYRB_TREE__
#define __MYRB_TREE__
#include "iterator.h"
#include "__alloc.h"
#include "__construct.h"
#include "queue.h"
#include "functional.h"

namespace mySTL{
    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;
    const __rb_tree_color_type __rb_tree_black = true;

    struct __rb_tree_node_base{
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;
        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;
        static base_ptr minimum(base_ptr x){
            while(x->left)
                x = x->left;
            return x;
        }
        static base_ptr maximum(base_ptr x){
            while(x->right)
                x = x->right;
            return x;
        }
    };

    template<class Value>
    struct __rb_tree_node:public __rb_tree_node_base{
        typedef __rb_tree_node<Value>* link_type;
        Value value_field;
    };

    struct __rb_tree_iterator_base{
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        base_ptr node;
    };
    template<class Value, class Ref, class Ptr>
    struct __rb_tree_iterator:public __rb_tree_iterator_base{
        typedef Value value_type;
        typedef Ref reference_type;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef __rb_tree_iterator<const Value, const Value&, const Value*> const_iterator;
        typedef __rb_tree_iterator<Value, Ref, Ptr> self;
        typedef typename __rb_tree_node<Value>::link_type link_type;

        __rb_tree_iterator(){}
        __rb_tree_iterator(link_type x){node = x;}
        __rb_tree_iterator(const iterator&it){node = it.node;}


        reference_type operator*(){return link_type(node)->value_field;}
        pointer operator->(){return &(link_type(node)->value_field);}
        self& operator++();
        self operator++(int){
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--();
        self operator--(int){
            self tmp = *this;
            --*this;
            return tmp;
        }
        bool operator==(const iterator& x)const{return node == x.node;}
        bool operator!=(const iterator& x)const{return node != x.node;}
    };

    template<class T>
    struct __rb_tree_iterator<const T, const T&, const T*>:public __rb_tree_iterator_base{
        typedef T value_type;
        typedef const T& reference_type;
        typedef const T* pointer;
        typedef __rb_tree_iterator<T, T&, T*> iterator;
        typedef __rb_tree_iterator<const T, const T&, const T*> const_iterator;
        typedef const_iterator self;
        typedef typename __rb_tree_node<T>::link_type link_type;

        __rb_tree_iterator(){}
        __rb_tree_iterator(link_type x){node = x;}
        __rb_tree_iterator(const iterator&it){node = it.node;}
        __rb_tree_iterator(const const_iterator&it){node = it.node;}

        reference_type operator*()const{return link_type(node)->value_field;}
        pointer operator->()const{return &(link_type(node)->value_field);}
        self& operator++();
        self operator++(int){
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--();
        self operator--(int){
            self tmp = *this;
            --*this;
            return tmp;
        }
        bool operator==(const const_iterator& x)const{return node == x.node;}
        bool operator!=(const const_iterator& x)const{return node != x.node;}
    };

    template<class Value, class Ref, class Ptr>
    typename __rb_tree_iterator<Value, Ref, Ptr>::self&  __rb_tree_iterator<Value, Ref, Ptr>::operator++(){
        if(node->right){
            node = node->right;
            while(node->left)
                node = node->left;
        }
        else{
            while(node->parent && node != node->parent->left)
                node = node->parent;
            if(node->parent)
                node = node->parent;
            //otherwise the node is the root
        }
        return *this;
    }
    template<class Value, class Ref, class Ptr>
    typename __rb_tree_iterator<Value, Ref, Ptr>::self&  __rb_tree_iterator<Value, Ref, Ptr>::operator--(){
        if(node->left){
            node = node->left;
            while(node->right)
                node = node->right;
        }
        else{
            while(node->parent && node != node->parent->right)
                node = node->parent;
            if(node->parent)
                node = node->parent;
            //otherwise the node is the root
        }
        return *this;
    }

    template<class T>
    typename __rb_tree_iterator<const T, const T&, const T*>::self&  __rb_tree_iterator<const T, const T&, const T*>::operator++(){
        if(node->right){
            node = node->right;
            while(node->left)
                node = node->left;
        }
        else{
            while(node->parent && node != node->parent->left)
                node = node->parent;
            if(node->parent)
                node = node->parent;
            //otherwise the node is the root
        }
        return *this;
    }
    template<class T>
    typename __rb_tree_iterator<const T, const T&, const T*>::self&  __rb_tree_iterator<const T, const T&, const T*>::operator--(){
        if(node->left){
            node = node->left;
            while(node->right)
                node = node->right;
        }
        else{
            while(node->parent && node != node->parent->right)
                node = node->parent;
            if(node->parent)
                node = node->parent;
            //otherwise the node is the root
        }
        return *this;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
    class rb_tree{
    protected:
        typedef void* void_pointer;
        typedef __rb_tree_node_base* base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
        typedef __rb_tree_color_type color_type;
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef rb_tree_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
        typedef __rb_tree_iterator<const value_type, const_reference, const value_type*> const_iterator;
    protected:
        link_type get_node(){return rb_tree_node_allocator::allocate();}
        void put_node(link_type p){rb_tree_node_allocator::deallocate(p);}
        link_type create_node(const_reference x){
            link_type p = get_node();
            construct(&p->value_field, x);
            return p;
        }
        link_type destroy_node(link_type p){
            destroy(&p->value_field);
            put_node(p);
        }
        link_type clone_node(link_type x){
            link_type p = create_node(x->value_field);
            p->color = x->color;
            p->parent = x->parent;
            p->left = x->left;
            p->right = x->right;
            return p;
        }

        size_type node_count;
        link_type header;
        Compare key_compare;
    private:
        link_type root() const{
            if(header->left == header)
                return 0;
            return link_type(header->left);
        }
        void init(){
            header = get_node();
            header->color = __rb_tree_black;
            header->parent = 0;
            header->left = header->right = header;
        }
        iterator __insert(link_type y/*parent of inserted point*/, const_reference v);
    public:
        void clear();
        rb_tree(const Compare& comp = Compare()):node_count(0), key_compare(comp){init();}
        ~rb_tree(){clear(); put_node(header);}
        iterator begin()const;
        iterator end()const{return iterator(header);}
        bool empty()const{return node_count == 0;}
        size_type size() const{return node_count;}
        size_type max_size()const{return size_type(-1);}
        iterator insert_equal(const_reference v);
        pair<iterator, bool> insert_unique(const_reference v);
        void erase(const_iterator);
        iterator find(const_reference)const;
        size_type erase_equal(const_reference);
        size_type erase_unique(const_reference);
        iterator lower_bound(const_reference)const;
        iterator upper_bound(const_reference)const;
        size_type count(const_reference)const;
        /*void print_tree()const{
            queue<link_type> q;
            int n = 1;
            int tmpn = 0;
            q.push(root());
            while(!q.empty()){
                link_type x = q.front();
                q.pop();
                --n;
                if(!x)
                    std::cout << "NULL ";
                else{
                    tmpn += 2;
                    q.push(link_type(x->left));
                    q.push(link_type(x->right));
                    std::cout << x->color << ' ' << x->value_field << "     ";
                }
                if(n == 0){
                    std::cout << '\n';
                    n = tmpn;
                    tmpn = 0;
                }
            }
        }
        int test(link_type p){
            if(!p)
                return 0;
            int x = p->left?test(link_type(p->left)):0;
            int y = p->right?test(link_type(p->right)):0;
            if(x < 0 || y < 0)
                throw "errr";
            if(x != y)
                throw "errr";
            if(p->color == __rb_tree_black)
                ++x;
            return x;
        }
        int test(){
            return test(root());
        }*/
    };

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const_reference x)const{
        iterator p = lower_bound(x);
        size_type res = 0;
        while(p != end() && KeyOfValue()(*p) == KeyOfValue()(x)){
            ++p;
            ++res;
        }
        return res;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear(){
        queue<link_type>q;
        q.push(root());
        while(!q.empty()){
            link_type p = q.front();
            q.pop();
            if(p){
                q.push(link_type(p->left));
                q.push(link_type(p->right));
                put_node(p);
            }
        }
        header->left = header->right = header;
        node_count = 0;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::begin()const{
        link_type x = root();
        link_type y = header;
        while(x){
            y = x;
            x = link_type(x->left);
        }
        return iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const_reference v){
        link_type x = root();
        link_type y = header;
        while(x){
            y = x;
            if(key_compare(KeyOfValue()(v), KeyOfValue()(x->value_field)))
                x = link_type(x->left);
            else
                x = link_type(x->right);
        }
        __insert(y, v);
        return iterator(key_compare(KeyOfValue()(v), KeyOfValue()(y->value_field))?link_type(y->left):link_type(y->right));
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> rb_tree<Key, Value, KeyOfValue, Compare, Alloc>:: insert_unique(const_reference v){
        link_type x = root();
        link_type y = header;
        if(!x)
            return pair<iterator, bool>(__insert(y, v), true);
        bool comp = true;
        while(x){
            y = x;
            if(comp=key_compare((KeyOfValue()(v)), KeyOfValue()(x->value_field)))
                x = link_type(x->left);
            else
                x = link_type(x->right);
        }
        iterator p(y);
        if(comp){
            if(p == begin())
                return pair<iterator, bool>(__insert(y, v), true);
            --p;
        }
        if(key_compare(KeyOfValue()(*p), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(y, v), true);
        else
            return pair<iterator, bool>(p, true);
    }


    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base* root){
        __rb_tree_node_base* y = x->left;
        __rb_tree_node_base* z = y->right;
        if(x == root)
            x->parent->left = x->parent->right = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->parent = x->parent;
        x->parent = y;
        y->right = x;
        x->left = z;
        if(z)
            z->parent = x;
    }

    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base* root){
        __rb_tree_node_base* y = x->right;
        __rb_tree_node_base* z = y->left;
        if(x == root)
            x->parent->right = x->parent->left = y;
        else if(x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->parent = x->parent;
        x->parent = y;
        y->left = x;
        x->right = z;
        if(z)
            z->parent = x;
    }

    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base* header){
        while(x->parent->color == __rb_tree_red)
            if(x->parent == x->parent->parent->left){
                __rb_tree_node_base* y = x->parent->parent->right;
                if(y && y->color == __rb_tree_red){
                    y->color = x->parent->color = __rb_tree_black;
                    x = y->parent;
                    x->color = __rb_tree_red;
                }
                else{
                    if(x == x->parent->right){
                        x = x->parent;
                        __rb_tree_rotate_left(x, header->left);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, header->left);
                }
            }
            else{
                __rb_tree_node_base* y = x->parent->parent->left;
                if(y && y->color == __rb_tree_red){
                    y->color = x->parent->color = __rb_tree_black;
                    x = y->parent;
                    x->color = __rb_tree_red;
                }
                else{
                    if(x == x->parent->left){
                        x = x->parent;
                        __rb_tree_rotate_right(x, header->left);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, header->left);
                }
            }
        header->left->color = __rb_tree_black;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type y, const_reference v){
        ++node_count;
        link_type z = create_node(v);
        z->parent = y;
        z->left = z->right = 0;
        z->color = __rb_tree_red;
        if(y == header){
            y->right = y->left = z;
            z->color = __rb_tree_black;
        }
        else if(key_compare(KeyOfValue()(v), KeyOfValue()(y->value_field)))
            y->left = z;
        else
            y->right = z;
        __rb_tree_rebalance(z, header);
        return iterator(z);
    }

    inline void __rb_tree_transplant(__rb_tree_node_base* x, __rb_tree_node_base* pos){
        if(pos == pos->parent->right)
            pos->parent->right = x;
        else
            pos->parent->left = x;
        if(x)
            x->parent = pos->parent;
    }

    inline void __rb_tree_fix_up(__rb_tree_node_base* y, bool isLeft, __rb_tree_node_base* root){
        if(isLeft?y->left:y->right)
            y = isLeft?y->left:y->right;
        else{
            if(isLeft){
                __rb_tree_node_base* z = y->right;
                if(z->color == __rb_tree_red){
                    z->color = __rb_tree_black;
                    __rb_tree_rotate_left(y, root);
                    y->right->color = __rb_tree_red;
                    return;
                }
                if(z->left && z->right){
                    __rb_tree_rotate_left(y, root);
                    if(y->color == __rb_tree_red){
                        y->color = __rb_tree_black;
                        z->color = __rb_tree_red;
                        z->right->color = __rb_tree_black;
                    }
                    else
                        z->right->color = __rb_tree_black;
                    return;
                }
                else if(z->right || z->left){
                    if(z->left){
                        z->color = __rb_tree_red;
                        z->left->color = __rb_tree_black;
                        __rb_tree_rotate_right(z, root);
                        z = y->right;
                    }
                    __rb_tree_rotate_left(y, root);
                    if(y->color == __rb_tree_red)
                        return;
                    else{
                        z->right->color = __rb_tree_black;
                        y = z;
                        return;
                    }
                }
                else{
                    if(y->color == __rb_tree_red){
                        z->color = __rb_tree_red;
                        y->color = __rb_tree_black;
                        return;
                    }
                    else
                        z->color = __rb_tree_red;//step in
                }
            }
            else{
                __rb_tree_node_base* z = y->left;
                if(z->color == __rb_tree_red){
                    z->color = __rb_tree_black;
                    __rb_tree_rotate_right(y, root);
                    y->left->color = __rb_tree_red;
                    return;
                }
                if(z->left && z->right){
                    __rb_tree_rotate_right(y, root);
                    if(y->color == __rb_tree_red){
                        y->color = __rb_tree_black;
                        z->color = __rb_tree_red;
                        z->left->color = __rb_tree_black;
                    }
                    else
                        z->left->color = __rb_tree_black;
                    return;
                }
                else if(z->right || z->left){
                    if(z->right){
                        z->color = __rb_tree_red;
                        z->right->color = __rb_tree_black;
                        __rb_tree_rotate_left(z, root);
                        z = y->left;
                    }
                    __rb_tree_rotate_right(y, root);
                    if(y->color == __rb_tree_red)
                        return;
                    else{
                        z->left->color = __rb_tree_black;
                        y = z;
                        return;
                    }
                }
                else{
                    if(y->color == __rb_tree_red){
                        z->color = __rb_tree_red;
                        y->color = __rb_tree_black;
                        return;
                    }
                    else
                        z->color = __rb_tree_red;//step in
                }
            }
        }
        while(y != root && y->color == __rb_tree_black){
            if(y == y->parent->left){
                __rb_tree_node_base* z = y->parent->right;
                if(z->color == __rb_tree_red){
                    z->color = __rb_tree_black;
                    y->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(y->parent, root);
                    z = y->parent->right;
                }
                if(z->left->color == __rb_tree_black && z->right->color == __rb_tree_black){
                    z->color = __rb_tree_red;
                    y = y->parent;
                }
                else{
                    if(z->right->color != __rb_tree_red){
                        z->left->color = __rb_tree_black;
                        z->color == __rb_tree_red;
                        __rb_tree_rotate_right(z, root);
                        z = y->parent->right;
                    }
                    z->color = y->parent->color;
                    z->right->color = __rb_tree_black;
                    y->parent->color = __rb_tree_black;
                    __rb_tree_rotate_left(y->parent, root);
                    y = root;
                }
            }
            else{
                __rb_tree_node_base* z = y->parent->left;
                if(z->color == __rb_tree_red){
                    z->color = __rb_tree_black;
                    y->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(y->parent, root);
                    z = y->parent->left;
                }
                if(z->left->color == __rb_tree_black && z->right->color == __rb_tree_black){
                    z->color = __rb_tree_red;
                    y = y->parent;
                }
                else{
                    if(z->left->color != __rb_tree_red){
                        z->right->color = __rb_tree_black;
                        z->color == __rb_tree_red;
                        __rb_tree_rotate_left(z, root);
                        z = y->parent->left;
                    }
                    z->color = y->parent->color;
                    z->left->color = __rb_tree_black;
                    y->parent->color = __rb_tree_black;
                    __rb_tree_rotate_right(y->parent, root);
                    y = root;
                }
            }
        }
        y->color = __rb_tree_black;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const_iterator p){
        --node_count;
        link_type x = link_type(p.node);
        link_type y = link_type(x->parent);
        __rb_tree_color_type original_color = x->color;
        bool isLeft = x==x->parent->left;
        if(x->left == 0)
            __rb_tree_transplant(x->right, x);
        else if(x->right == 0)
            __rb_tree_transplant(x->left, x);
        else{
            link_type z = link_type(x->right);
            while(z->left)
                z = link_type(z->left);
            original_color = z->color;
            z->color = x->color;
            x->left->parent = z;
            z->left = x->left;
            if(z->parent != x){
                y = link_type(z->parent);
                isLeft = true;
                __rb_tree_transplant(z->right, z);
                z->right = x->right;
                x->right->parent = z;
            }
            else{
                y = z;
                isLeft = false;
            }
            __rb_tree_transplant(z, x);
        }
        put_node(x);
        if(original_color == __rb_tree_black)
            __rb_tree_fix_up(y, isLeft, root());
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const_reference v)const{
        link_type x = root();
        while(x){
            if(KeyOfValue()(x->value_field) == KeyOfValue()(v))
                return iterator(x);
            if(key_compare(KeyOfValue()(v), KeyOfValue()(x->value_field)))
                x = link_type(x->left);
            else
                x = link_type(x->right);
        }
        return end();
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const_reference v)const{
        link_type x = root();
        link_type mem = 0;
        while(x)
            if(key_compare(KeyOfValue()(x->value_field), KeyOfValue()(v))){
                mem = x;
                x = link_type(x->right);
            }
            else
                x = link_type(x->left);
        if(mem)
            return ++iterator(mem);
        return end();
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const_reference v)const{
        link_type x = root();
        link_type mem = 0;
        while(x)
            if(key_compare(KeyOfValue()(v), KeyOfValue()(x->value_field))){
                mem = x;
                x = link_type(x->left);
            }
            else
                x = link_type(x->right);
        if(mem)
            return iterator(mem);
        return end();
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_equal(const_reference v){
        iterator first = lower_bound(v);
        size_type res = 0;
        while(first != end() && KeyOfValue()(*first) == KeyOfValue()(v)){
            erase(first++);
            ++res;
        }
        return res;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_unique(const_reference v){
        iterator p = find(v);
        if(p == end())
            return 0;
        erase(p);
        return 1;
    }
};


#endif