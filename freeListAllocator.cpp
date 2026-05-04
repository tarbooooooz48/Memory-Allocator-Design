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

/*           // till stage 3 
void* FreeListAllocator:: allocate(std::size_t size) {                 //till stage 3
        
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

    */


    //stage 4 alignment allocate(size,alignment) why alignment because
    // for alignment we require new metadata for setting the aligned payload for it


    void*  FreeListAllocator:: allocate(std::size_t size, std::size_t alignment){

        FreeBlock* curr = head_;

    while (curr) {
        if (!curr->free) {
            curr = curr->next;
            continue;
        }

        std::byte* block_start = reinterpret_cast<std::byte*>(curr);

        // Step 1: compute raw user pointer (after header)
        uintptr_t raw_data = reinterpret_cast<uintptr_t>(
            block_start + sizeof(FreeBlock)
        );

        // Step 2: align it
        uintptr_t aligned_data = alignForward(raw_data, alignment);

        // Step 3: compute adjustment
        uint8_t adjustment = static_cast<uint8_t>(aligned_data - raw_data);

        // Step 4: total required space inside block
        size_t total_size = size + adjustment;

        if (curr->size < total_size) {
            curr = curr->next;
            continue;
        }

        // Step 5: split if enough space remains
        if (curr->size >= total_size + sizeof(FreeBlock)) {
            split(curr, total_size);
        }

        // Step 6: mark as used
        curr->free = false;
        curr->adjustment = adjustment;

        // Step 7: return aligned pointer
        return reinterpret_cast<void*>(aligned_data);
    }

    return nullptr; // no suitable block


    }


    //stage 4: alignment allocate helper to forward it
    
        uintptr_t FreeListAllocator:: alignForward(uintptr_t ptr, size_t alignment) {
    uintptr_t mask = alignment - 1;
    return (ptr + mask) & ~mask;
    }


    //stage 3 split function

/*
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
*/


//stage 4 split function

void FreeListAllocator::split(FreeBlock* block, std::size_t size) {
    // Check if we can actually split
    if (block->size < size + sizeof(FreeBlock)) {
        return; // not enough space to split safely
    }

    std::byte* block_start = reinterpret_cast<std::byte*>(block);

    FreeBlock* new_block = reinterpret_cast<FreeBlock*>(
        block_start + sizeof(FreeBlock) + size
    );

    new_block->size = block->size - size - sizeof(FreeBlock);
    new_block->free = true;

    // link fix
    new_block->next = block->next;
    new_block->prev = block;

    if (block->next)
        block->next->prev = new_block;

    block->next = new_block;
    block->size = size;
}




    //stage 3 deallocate()
/*
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
*/

//stage 4 helper functions for coalesce function

   FreeListAllocator:: FreeBlock*  FreeListAllocator:: nextPhys(FreeBlock* b) {     //targeting the next block
    auto* p = reinterpret_cast<std::byte*>(b);
    return reinterpret_cast<FreeBlock*>(p + sizeof(FreeBlock) + b->size);
}


//stage 4 helper function for coalesce function
    bool FreeListAllocator:: withinHeap(void* p) const {
        auto* b = static_cast<std::byte*> (p);

    return p >= buffer_ && p < (buffer_+capacity_);
    }


    //stage 4 coalesce block

  void FreeListAllocator::coalesce(FreeListAllocator::FreeBlock* block) {
    if (!block) return;

    // ---- merge NEXT
    FreeBlock* next = nextPhys(block);
    if (withinHeap(next) && next->free) {
        block->size += sizeof(FreeBlock) + next->size;

        block->next = next->next;
        if (next->next)
            next->next->prev = block;
    }

    // ---- merge PREV (with adjacency check)
    FreeBlock* prev = block->prev;
    if (prev && prev->free) {
        FreeBlock* prev_next = nextPhys(prev);

        if (prev_next == block) {
            prev->size += sizeof(FreeBlock) + block->size;

            prev->next = block->next;
            if (block->next)
                block->next->prev = prev;
        }
    }
}


        //stage 4 deallocate()
void FreeListAllocator::deallocate(void* ptr) {
    if (!ptr) return;

    std::byte* aligned_ptr = static_cast<std::byte*>(ptr);

    // Step 1: get header just before aligned memory
    FreeBlock* block = reinterpret_cast<FreeBlock*>(
        aligned_ptr - sizeof(FreeBlock)
    );
    

    // Step 2: move back to actual block start using adjustment
    block = reinterpret_cast<FreeBlock*>(
           
        reinterpret_cast<std::byte*>(block) - block->adjustment
    );

    // Step 3: mark free
    block->free = true;

    // Step 4: coalesce with neighbors
    coalesce(block);
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