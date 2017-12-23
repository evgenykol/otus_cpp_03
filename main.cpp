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
    //void *p;

    logging_allocator(int block_size_ = 5)
    {
        block_size = block_size_;
        allocated_blocks = 0;
        head = 0;
        cout << "logging_allocator(int) ctor, block_size = " << block_size << endl;
    }

    T* allocate(size_t n)
    {
/*        int blocs_to_allocate = 0;
        if (head == 0)
        {
            blocs_to_allocate = (n % block_size) + (n/block_size ? 1 : 0);
        }

        if (head + n <= block_size)
        {

        }

        if (blocks_to_allocate)
        {*/
            auto p = malloc(n * sizeof(T));
            cout << "allocate " << block_size * n << "\t at " << p << endl;
            if(!p)
                throw bad_alloc();
        //}
        return reinterpret_cast<T *>(p);
    }

    void deallocate(T* p, size_t n)
    {
        cout << "dealloca " << n << "\t at " << p << endl;
        free(p);
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&& ...args)
    {
        cout << "construct " << "\t at " << p << endl;
        new(p) U(forward<Args>(args) ...);
    }

    void destroy(T* p)
    {
        p->~T();
        cout << "destroy " << "\t at " << p << endl;
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

    auto v = vector<int, logging_allocator<int>>{};

    for(size_t i = 0; i < 5; ++i)
    {
        v.emplace_back(i);
    }

    for (auto i :v)
    {
        cout << i << endl;
    }
    return 0;
}
