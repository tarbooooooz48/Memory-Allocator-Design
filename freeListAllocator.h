#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H
#include <cstddef>
#include <cstdint>




class FreeListAllocator {
public:
    explicit FreeListAllocator(std::size_t size);
        

     // void* allocate(std::size_t size);   // till stage 3
        void* allocate(std::size_t size, std::size_t alignment);   //stage 4
    
       static uintptr_t alignForward(uintptr_t ptr, size_t alignment);    //stage 4 alignement helper function
  
    
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
    std::size_t size;   // size of USER DATA (not including header)  (8 byte) 
    bool free;              // whether block is free      (1 byte)

    FreeBlock* next; // next physical block in memory      (8 byte)
    FreeBlock* prev; // previous physical block          (8 byte)

     uint8_t adjustment;  // stage 4: alignment metadata    (1 byte)

};

    //for stage 3,  size of  Freeblock is 32 
    //now after introducing new variable adjustment (1 byte ), the size becomes 33 byte which is not acceptable
                // so by padding of 7 bytes the new size of Freeblock becomes 40 bytes and this is just the metadata for the process

    

    std::byte* buffer_;         // (8 byte)
    std::size_t capacity_;         // (8 byte)
    //FreeBlock* free_list_;  // for stage 2

    FreeBlock* head_;       // for stage 3,  size of header block is 32 
                            // pointer size is (8 byte)

        
         void split(FreeBlock* block, std::size_t size);   //stage 3  split into [used][free]
            void coalesce(FreeBlock* block);      //stage 4  merge free blocks

            static FreeBlock* nextPhys(FreeBlock* b);    //helper funtion for coalesce function
 
        bool withinHeap(void* p) const ;  //helper function for coalesce function (checking if pointer lies inside allocator memory)

};


#endif
