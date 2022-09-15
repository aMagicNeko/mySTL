#ifndef __MYALLOC__
#define __MYALLOC__
#include <cstdlib>
#include <new>
#ifndef __THROW_BAD_ALLOC
#define __THROW_BAD_ALLOC throw std::bad_alloc()
#endif
namespace mySTL{
    template<int inst>
    class __malloc_alloc_template{
    private:
        static void* oom_malloc(size_t);
        static void* oom_realloc(void*, size_t);
        static void (*__malloc_alloc_oom_handler)();
    public:
        static void* allocate(size_t n){
            void* res = std::malloc(n);
            if(res == 0)
                res = oom_malloc(n);
            return res;
        }
        static void* reallocate(void* p, size_t/*old_sz*/, size_t new_sz){
            void* res = std::realloc(p, new_sz);
            if(res == 0)
                res = oom_realloc(p, new_sz);
            return res;
        }
        static void deallocate(void* p, size_t){
            free(p);
        }
        static void (*set_malloc_handler(void(*f)()))(){
            void (*old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }
        
    };

    template<int inst>
    void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

    template<int inst> 
    void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
        for(;;){
            if(__malloc_alloc_oom_handler == 0)
                __THROW_BAD_ALLOC;
            (__malloc_alloc_oom_handler)();
            void* p = malloc(n);
            if(p)
                return p;
        }
    }

    template<int inst> 
    void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
        for(;;){
            if(__malloc_alloc_oom_handler == 0)
                {__THROW_BAD_ALLOC;}
            (*__malloc_alloc_oom_handler)();
            void* p = realloc(p, n);
            if(p)
                return p;
        }
    }

    typedef __malloc_alloc_template<0> malloc_alloc;

    enum{__ALIGN = 8};
    enum{__MAX_BYTES = 128};
    enum{__NFREELISTS = __MAX_BYTES/__ALIGN};
    template<bool, int>
    class __malloc_alloc{
    private:
        union obj{
            union obj* free_list_link;
            char clientdata[1];
        };
        static size_t round_up(size_t n){
            return (n+__ALIGN-1)&~(__ALIGN-1);
        }
        static obj* volatile free_list[__NFREELISTS];
        static size_t free_list_index(size_t n){
            return (n+__ALIGN-1)/__ALIGN-1;
        }
        static void* refill(size_t n);
        //memory pool
        static char* start_free;
        static char* end_free;
        static char* chunk_alloc(size_t, int&);
        static int heap_size;
    public:
        static void* allocate(size_t n);
        static void deallocate(void* p, size_t n);
        static void* reallocate(void* p, size_t old_sz, size_t new_sz);
    };
    template<bool thread, int inst> char* __malloc_alloc<thread, inst>::start_free = 0;
    template<bool thread, int inst> char* __malloc_alloc<thread, inst>::end_free = 0;
    template<bool thread, int inst> int __malloc_alloc<thread, inst>::heap_size = 0;
    template<bool thread, int inst> typename __malloc_alloc<thread, inst>::obj* volatile __malloc_alloc<thread, inst>::free_list[__NFREELISTS]
    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    template<bool thread, int inst>
    void* __malloc_alloc<thread, inst>::allocate(size_t n){
        if(n > (size_t)__MAX_BYTES)
            return malloc_alloc::allocate(n);
        obj* volatile* my_free_list = free_list+free_list_index(n);
        obj* res = *my_free_list;
        if(res == 0)
            return refill(round_up(n));
        *my_free_list = res->free_list_link;
        return res;
    }
    template<bool thread, int inst>
    void* __malloc_alloc<thread, inst>::refill(size_t n){
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        if(nobjs == 1)
            return chunk;
        obj* volatile* my_free_list = free_list+free_list_index(n);
        obj* p = (obj*)(chunk+n);
        *my_free_list = p;
        for(int i = 1; i < nobjs-1; ++i){
            p->free_list_link = (obj*)(((char*)p)+n);
            p = p->free_list_link;
        }
        p->free_list_link = 0;
        return chunk;
    }
    template<bool thread, int inst>
    char* __malloc_alloc<thread, inst>::chunk_alloc(size_t size, int& nobjs){
        size_t need = size*nobjs;
        size_t left = end_free-start_free;
        if(left >= need){
            char* res = start_free;
            start_free += need;
            return res;
        }
        else if(left >= size){
            nobjs = left/size;
            char* res = start_free;
            start_free += nobjs*size;
            return res;
        }
        else{
            int bytes_to_get = 2*need+(heap_size>>4);
            if(left > 0){
                obj* volatile* my_free_list = free_list+free_list_index(left);
                ((obj*)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj*)start_free;
            }
            start_free = (char*)malloc(bytes_to_get);
            if(start_free == 0){
                for(int i = size; i <= __MAX_BYTES; i += __ALIGN){
                    obj* volatile* my_free_list = free_list+free_list_index(i);
                    if(*my_free_list){
                        start_free = (char*)my_free_list;
                        *my_free_list = (*my_free_list)->free_list_link;
                        end_free = start_free+i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = 0;
                __THROW_BAD_ALLOC;
            }
            end_free = start_free+bytes_to_get;
            heap_size += bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }
    template<bool thread, int inst>
    void __malloc_alloc<thread, inst>::deallocate(void* p, size_t n){
        if(n > (size_t)__MAX_BYTES)
            return malloc_alloc::deallocate(p, n);
        obj* q = (obj*)p;
        obj* volatile* my_free_list = free_list+free_list_index(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    typedef __malloc_alloc<true, 0> alloc;

    template<class T, class Alloc = alloc>
    class simple_alloc{
    public:
        static T* allocate(size_t n){
            return n==0 ? 0 : (T*)Alloc::allocate(n*sizeof(T));
        }
        static T* allocate(void){
            return (T*)Alloc::allocate(sizeof(T));
        }
        static void deallocate(T* p, size_t n){
            if(n != 0)
                Alloc::deallocate(p, n*sizeof(T));
        }
        static void deallocate(T* p){
            Alloc::deallocate(p, sizeof(T));
        }
    };
}
#endif
