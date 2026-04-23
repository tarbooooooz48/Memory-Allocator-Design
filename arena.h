#ifndef ARENA_H

   #include <cstddef>
#include <new>
#include <cassert>
#include <algorithm>


class Arena {
    private:
    std::byte* buffer_;
    size_t capacity_;
    size_t offset_;

public:
   explicit Arena(size_t capacity);
       
    Arena(Arena&& other) noexcept;  //move operation to new Arena

    ~Arena();

    void* allocate(size_t size, size_t alignment );   

    void reset(); 
}; 

#endif 
