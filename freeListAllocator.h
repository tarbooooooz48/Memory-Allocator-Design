#ifndef FREELISTALLOCATOR_H

#include <cstddef>




class FreeListAllocator {
public:
    explicit FreeListAllocator(std::size_t size);
        

    void* allocate(std::size_t size);
     void deallocate(void* ptr);
     void dump();
    


private:

// STAGE 2
    /*
    struct FreeBlock {
        std::size_t size;
        FreeBlock* next;
    };
    */

    struct FreeBlock {       //sizeof(FreeBlock) is always 32
    std::size_t size;   // size of USER DATA (not including header)
    bool free;

    FreeBlock* next; // next physical block in memory
    FreeBlock* prev; // previous physical block

       

};


    std::byte* buffer_;
    std::size_t capacity_;
    //FreeBlock* free_list_;  // for stage 2

    FreeBlock* head_;       // for stage 3,  size of header block is 32 as well


        
         void split(FreeBlock* block, std::size_t size); 

};


#endif
