#include "freeListAllocator.h"
#include<iostream>

//STAGE 2

/*
FreeListAllocator :: FreeListAllocator(std::size_t size)
        : buffer_(static_cast<std::byte*>(::operator new(size))),
          capacity_(size) {
        free_list_ = reinterpret_cast<FreeBlock*>(buffer_);
        free_list_->size = size;
        free_list_->next = nullptr;
        
        std::cout<<" FreeListAllocator of size "<<capacity_<<" is created at memory location "<<buffer_<<std::endl;

    }


void* FreeListAllocator:: allocate(std::size_t size) {
        FreeBlock* prev = nullptr;
        FreeBlock* curr = free_list_;

        std::cout<<" freelist allocator(size) pointer function called ...... "<<std::endl;
        std::cout<<std::endl;
       
        while (curr) {

              std::size_t usable = curr->size - sizeof(FreeBlock);

            if (usable >= size + sizeof(FreeBlock)) {
                if (prev)
                    prev->next = curr->next;
                else
                    free_list_ = curr->next;
                
                std::cout<<" freelistallocator's allocate(size) pointer function has "<<std::endl;

                

                std::cout<<" current next address "<<curr->next<<" with size "<<curr->size<<std::endl;
                return reinterpret_cast<std::byte*>(curr) + sizeof(FreeBlock);
            }
            prev = curr;

            if(prev != nullptr){
                    std::cout<< " previous next address "<<prev->next<< " with size  "<< prev->size<< " and "<<std::endl;
                    std::cout<<std::endl;
                }

            curr = curr->next;
        }
                 return nullptr; // out of memory
    }



    void FreeListAllocator::deallocate(void* ptr) {
    if (!ptr) return;

    FreeBlock* block = reinterpret_cast<FreeBlock*>(
        reinterpret_cast<std::byte*>(ptr) - sizeof(FreeBlock)
    );

    block->next = free_list_;     //pushing back into the freelist
    free_list_ = block;
        }

        */


//STAGE 3

FreeListAllocator::FreeListAllocator(std::size_t size) {
    buffer_ = static_cast<std::byte*>(::operator new(size));
    capacity_ = size;

    auto* block = reinterpret_cast<FreeBlock*>(buffer_);

    block->size = size - sizeof(FreeBlock);
    block->free = true;
    block->next = nullptr;
    block->prev = nullptr;

    head_ = block; // start of block chain
}


void* FreeListAllocator:: allocate(std::size_t size) {
        
        FreeBlock* curr = head_;

        // std::cout<<" freelist allocator(size) pointer function called ...... "<<std::endl;
        // std::cout<<std::endl;
       
        while (curr) {


            if (curr->free && curr->size >= size) {  //not complete in this stage because block is  
                                                    //usable if it fully satisfies the request or be split safely
        
        


        //         std::cout << "Checking block: "
        //   << "addr=" << curr 
        //   << " size=" << curr->size
        //   << " free=" << curr->free
        //   << "\n";



                std::cout << "\n[ALLOC]\n";
                std::cout << "Request size: " << size << "\n";
                std::cout << "Using block at: " << curr << "\n";
                std::cout << "Block payload before: " << curr->size << "\n";



                std::cout << "sizeof(Block): " << sizeof(FreeBlock) << "\n";
            if (curr->size >= size + sizeof(FreeBlock)) {
                split(curr, size);


                std::cout << "After split:\n";
                std::cout << "  allocated size: " << curr->size << "\n";
                std::cout << "  next block: " << curr->next
                                << " size=" << (curr->next ? curr->next->size : 0) << "\n";


            }

            curr->free = false;
                
                // std::cout<<" freelistallocator's allocate(size) pointer function has "<<std::endl;

                // std::cout<<" current next address "<<curr->next<<" with size "<<curr->size<<std::endl;

                std::cout << "Allocated block at: " << curr
          << " | data ptr: " 
          << (void*)(reinterpret_cast<std::byte*>(curr) + sizeof(FreeBlock))
          << " | size: " << curr->size
          << " | next: " << curr->next
          << "\n";

                return reinterpret_cast<std::byte*>(curr) + sizeof(FreeBlock);
            }
          

            // if(prev != nullptr){
            //         std::cout<< " previous next address "<<prev->next<< " with size  "<< prev->size<< " and "<<std::endl;
            //         std::cout<<std::endl;
            //     }

            curr = curr->next;
        }
                 return nullptr; // out of memory
    }



    void FreeListAllocator:: split(FreeBlock* block, std::size_t size) {
    std::byte* block_start = reinterpret_cast<std::byte*>(block);

    // location of new block
    FreeBlock* new_block = reinterpret_cast<FreeBlock*>(
        block_start + sizeof(FreeBlock) + size
    );

    new_block->size = block->size - size - sizeof(FreeBlock);
    new_block->free = true;

    // fix links
    new_block->next = block->next;
    new_block->prev = block;

    if (block->next)
        block->next->prev = new_block;

    block->next = new_block;
    block->size = size;
}





    void FreeListAllocator::deallocate(void* ptr) {
    if (!ptr) return;

    std::cout << "\n[FREE]\n";
std::cout << "Freeing ptr: " << ptr << "\n";

    FreeBlock* block = reinterpret_cast<FreeBlock*>(
        static_cast<std::byte*>(ptr) - sizeof(FreeBlock)
    );

    std::cout << "Block addr: " << block
          << " size=" << block->size << "\n";

    block->free = true;

    // merge with next
    if (block->next && block->next->free) {
        block->size += sizeof(FreeBlock) + block->next->size;
        block->next = block->next->next;

        if (block->next)
            block->next->prev = block;

         std::cout << "Merging with NEXT block at "
              << block->next
              << " size=" << block->next->size << "\n";

    }

    // merge with prev
    if (block->prev && block->prev->free) {
        block->prev->size += sizeof(FreeBlock) + block->size;
        block->prev->next = block->next;

        if (block->next)
            block->next->prev = block->prev;


         std::cout << "Merging with PREV block at "
              << block->prev
              << " size=" << block->prev->size << "\n";;

    }
}




//printing values (dump)

void FreeListAllocator::dump() {
    std::cout << "\n=== MEMORY DUMP ===\n";

    FreeBlock* curr = head_;
    int i = 0;

    while (curr) {
        std::byte* addr = reinterpret_cast<std::byte*>(curr);

        std::cout << "Block " << i++ << "\n";
        std::cout << "  addr      : " << (void*)addr << "\n";
        std::cout << "  payload   : " << curr->size << "\n";
        std::cout << "  total     : " << curr->size + sizeof(FreeBlock) << "\n";
        std::cout << "  free      : " << curr->free << "\n";
        std::cout << "  next      : " << curr->next << "\n";
        std::cout << "  prev      : " << curr->prev << "\n";

        curr = curr->next;
    }

//     std::byte* expected_next =
//     reinterpret_cast<std::byte*>(curr)
//     + sizeof(FreeBlock)
//     + curr->size;

// std::cout << "  expected next addr: " << (void*)expected_next << "\n";

// if (curr->next) {
//     std::cout << "  actual next addr  : " << curr->next << "\n";
// }

    std::cout << "===================\n";
}