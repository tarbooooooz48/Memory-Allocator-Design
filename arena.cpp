
    #include "arena.h"

#include <iostream>



   Arena::Arena(size_t capacity)   : capacity_(capacity), offset_(0)
    {
        buffer_ = static_cast<std::byte*>(::operator new(capacity_)); //has address for buffer size

        std::cout<< " Arena with size_t capacity : " << capacity_<<" formed...."<<std::endl;
    }

      void* Arena:: allocate(size_t size, size_t alignment = alignof(std::max_align_t))  // maximum alignment which gives padding if low in 
                                                                                        //size for eg: char - 1, int - 4 , now alignof(int) 
                                                                                        //in arena char - 1, padding 3 (coz int alignof 4)
                                                                                         // and next int - 4   
        {
        size_t current = reinterpret_cast<size_t>(buffer_ + offset_);
        size_t aligned = (current + alignment - 1) & ~(alignment - 1);
        size_t newOffset = aligned - reinterpret_cast<size_t>(buffer_) + size;

        if (newOffset > capacity_) {
            throw std::bad_alloc();
        }
        
        std::cout<<" Changing the Arena offset from "<<offset_<<" to newOffset : "<<newOffset<<std::endl;

        offset_ = newOffset;


        return reinterpret_cast<void*>(aligned);
    }

  Arena::Arena(Arena&& other) noexcept
    : buffer_(other.buffer_), capacity_(other.capacity_), offset_(other.offset_) {
    other.buffer_ = nullptr;
    }


    void Arena:: reset()
    {
        std::cout<<"Arena is being reset, so Offset "<<offset_<<" is turned to 0..."<<std::endl; 
        offset_ = 0;
        
    }



    Arena :: ~Arena()
    {

        std::cout<<" Delete the Arena with size : "<<capacity_ <<std::endl;
        ::operator delete(buffer_);
    }





