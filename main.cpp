#include <iostream>
#include <map>
#include <vector>

#include "version.h"

#define ENABLE_LOGGING
#define SIZE 10


using namespace std;

template <typename T>
struct logging_allocator
{
    using value_type = T;

    T* allocate(size_t n)
    {
        auto p = malloc(n * sizeof(T));
        if(!p)
        {
            throw bad_alloc();
        }
        cout << "allocate " << n << "\t\t at " << p << " sizeof " << sizeof(T) << endl;
        return reinterpret_cast<T *>(p);
    }

    void deallocate(T* p, size_t n)
    {
        cout << "deallocate " << n << "\t at " << p << endl;
        free(p);
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&& ...args)
    {
        cout << "construct " << "\t\t at " << p << " sizeof " << sizeof(U) << endl;
        new(p) U(forward<Args>(args) ...);
    }

    void destroy(T* p)
    {
        p->~T();
        cout << "destroy " << "\t\t at " << p << endl;
    }

};

template <typename T>
struct custom_allocator
{
    using value_type = T;
    int block_size;
    int allocated_blocks;
    int head;
    T *head_ptr;
    void *p;

    custom_allocator(int block_size_ = SIZE)
    {
        block_size = block_size_;
        allocated_blocks = 0;
        head = 0;
        head_ptr = nullptr;
        p = nullptr;
#ifdef ENABLE_LOGGING
        cout << "custom_allocator ctor, block_size = " << block_size << endl;
#endif
    }

    T* allocate(size_t n)
    {
        if (allocated_blocks == 0)
        {
            p = malloc(block_size * sizeof(T));
            if(!p)
            {
                throw bad_alloc();
            }
            allocated_blocks++;

            head_ptr = reinterpret_cast<T *>(p);
#ifdef ENABLE_LOGGING
            cout << "allocate malloc " << block_size << "\t at " << p << " head_ptr " << head_ptr << endl;
#endif
        }
        else if (head >= block_size)
        {
#ifdef ENABLE_LOGGING
            cout << "allocate ptr reached top" << "\t at " << head_ptr << endl;
#endif
        }
        else
        {
            head += n;
            head_ptr += n * sizeof(T);
#ifdef ENABLE_LOGGING
            cout << "allocate move head " << n << "\t at " << head_ptr << endl;
#endif
        }

        return head_ptr;
    }

    void deallocate(T* pp, size_t n)
    {
        if(allocated_blocks && (head <= 0))
        {
#ifdef ENABLE_LOGGING
            cout << "deallocate free " << n << "\t at " << pp << endl;
#endif
            free(p);
            allocated_blocks--;
        }

        if (head > 0)
        {
            head -= n;
            head_ptr -= n * sizeof(T);
#ifdef ENABLE_LOGGING
            cout << "deallocate move head " << n << "\t at " << pp << endl;
#endif
        }
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&& ...args)
    {
#ifdef ENABLE_LOGGING
        cout << "construct " << "\t\t at " << p << endl;
#endif
        new(p) U(forward<Args>(args) ...);
    }

    void destroy(T* p)
    {
        p->~T();
#ifdef ENABLE_LOGGING
        cout << "destroy " << "\t\t at " << p << endl;
#endif
    }

};

int fact (int n)
{
    int r = 1;
    for (int i=2; i<=n; i++)
        r *= i;
    return r;
}

int main()
{
    cout << "version " << version() << endl << endl;

//    auto v = vector<int, custom_allocator<int>>{};
//    v.reserve(10);

//    for(size_t i = 0; i < 10; ++i)
//    {
//        v.emplace_back(i);
//    }

//    for (auto i :v)
//    {
//        cout << i << endl;
//    }

    cout << endl << "map1:" << endl;
    auto m1 = map<int, int>{};
    for(size_t i = 0; i < SIZE; ++i)
    {
        m1.emplace(pair<int, int>{i, fact(i)});
    }

    for(auto mm : m1)
    {
        cout << mm.first << " " << mm.second << endl;
    }

    cout << endl << "map2:" << endl;
    auto m2 = map<int, int, less<int>, custom_allocator<pair<const int, int>>>{};
    for(size_t i = 0; i < SIZE; ++i)
    {
        m2.emplace(pair<int, int>{i, fact(i)});
    }

    for(auto mm : m2)
    {
        cout << mm.first << " " << mm.second << endl;
    }

    return 0;
}
