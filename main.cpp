#include <iostream>
#include <map>
#include <vector>

#include "version.h"

using namespace std;

template <typename T>
struct logging_allocator
{
    using value_type = T;
    int block_size;
    int allocated_blocks;
    int head;
    T *head_ptr;
    void *p;

    logging_allocator(int block_size_ = 10)
    {
        block_size = block_size_;
        allocated_blocks = 0;
        head = 0;
        p = nullptr;
        cout << "logging_allocator(int) ctor, block_size = " << block_size << endl;
    }

    T* allocate(size_t n)
    {
        if (allocated_blocks == 0)
        {
            p = malloc(block_size * sizeof(T));
            cout << "allocate malloc " << block_size << "\t at " << p << endl;
            if(!p)
            {
                throw bad_alloc();
            }
            allocated_blocks++;
            head++;
            head_ptr = reinterpret_cast<T *>(p);
        }
        else
        {
            head += n;
            head_ptr += n * sizeof(T);
            cout << "allocate move ptr " << n << "\t at " << head_ptr << endl;
        }

        return head_ptr;
    }

    void deallocate(T* pp, size_t n)
    {
        if (head > 0)
        {
            head -= n;
            head_ptr -= n * sizeof(T);
            cout << "deallocate move ptr " << n << "\t at " << head_ptr << endl;
        }

        if(head <= 0)
        {
            cout << "deallocate free " << n << "\t at " << p << endl;
            free(p);
        }
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&& ...args)
    {
        cout << "construct " << "\t\t at " << p << endl;
        new(p) U(forward<Args>(args) ...);
    }

    void destroy(T* p)
    {
        p->~T();
        cout << "destroy " << "\t\t at " << p << endl;
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

//    auto v = vector<int, logging_allocator<int>>{};

//    for(size_t i = 0; i < 5; ++i)
//    {
//        v.emplace_back(i);
//    }

//    for (auto i :v)
//    {
//        cout << i << endl;
//    }

    cout << endl << "map1:" << endl;
    auto m1 = map<int, int>{};
    for(size_t i = 0; i < 10; ++i)
    {
        m1.insert(pair<int, int>{i, fact(i)});
    }

    for(auto mm : m1)
    {
        cout << mm.first << "\t" << mm.second << endl;
    }

    cout << endl << "map2:" << endl;
    auto m2 = map<int, int, less<int>, logging_allocator<pair<const int, int>>>{};
    for(size_t i = 0; i < 10; ++i)
    {
        m2.insert(pair<int, int>{i, fact(i)});
    }

    for(auto mm : m2)
    {
        cout << mm.first << "\t" << mm.second << endl;
    }

    return 0;
}
