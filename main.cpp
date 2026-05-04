#include <bits/stdc++.h>
#include <memory>
#include<array>
#include  "arena.h"
#include "freeListAllocator.h"

using namespace std;

struct A {
    int x = 42;
};

int main() {

    // Proper byte buffer
    // std::byte* p = new std::byte[16];       // cannot dereference the byte,  only exists as a raw pointer
    //                                         // but can be reinterpreted to a particular class or object

    // std::byte* q = p + 8;
    // cout << (void*)p << " and " << (void*)q << endl;

    // // Allocator usage
    // std::allocator<A> allocator;

    // A* mem = allocator.allocate(1);   // allocate space for 1 A

    // A* obj = new (mem) A();           // placement new

    // cout << obj->x << endl;

    // // obj->~A();                        // manual destruction
    // allocator.deallocate(mem, 1);     // correct deallocation

    // delete[] p;


//         void* mem = allocator.allocate(sizeof(A));
// A* obj = new (mem) A();   // placement new
// obj->~A();                // manual destruction
// allocator.deallocate(obj,sizeof(obj));

// Stage 1 : arena allocator
  /*  
Arena arena(2000);

void* mem = arena.allocate(sizeof(int), alignof(int));  

int* x = new (mem) int(42);

int temparr[10];

void* memForArrayOfInt = arena.allocate(sizeof(temparr), alignof(int));

int *y = new (memForArrayOfInt) int[]{1,2,3,4,5,6,7,8};

cout << *x << endl;

cout<<*y+1<<endl;

// x-> ~int();   // manual destruction not possible by compiler

// delete x;     // not possible because it would initiate destructor of arena 2 times, while we considered it to be 
                    //targeted to only int pointer x

arena.reset();   // bulk free
*/

// =-----------------------------------------------------

// freeList allocator stage 2


/*
    FreeListAllocator alloc(128);

    void* a = alloc.allocate(16);
    void* b = alloc.allocate(16);

    alloc.deallocate(a);
    alloc.deallocate(b);

    void* c = alloc.allocate(16);

    std::cout << "Reused: " << c << "\n";


   
    // FreeListAllocator alloc(64);

    // int* a = static_cast<int*>(alloc.allocate(sizeof(int)));
    // *a = 10;

    // alloc.deallocate(a);

    // int* b = static_cast<int*>(alloc.allocate(sizeof(int)));

    // std::cout << "a: " << a << "\n";
    // std::cout << "b: " << b << "\n";


    */


    // =--------------------------------------------------------------------------------------------------------------------

 
    // STAGE 3: BLOCK SPLITTING AND COALESCING (CONTINUATION OF STAGE 2) 

    //review: it is just like list (STL) where we are trying to play around with
                // the memory values but on hardmemory level and using pointers

    // test 1 : basic allocation

 /*   // till step 3

         FreeListAllocator alloc1(256);    // for 64 bytes the size will throw exception , cause b will be 
                                        // pointing to null ptr
        std::cout<<" Allocator 1(256) "<<std:: endl;

    std::cout << "=== Basic Allocation ===\n";

    void* a = alloc1.allocate(32);
    
    void* b = alloc1.allocate(32);  // size of previous block(32) - header(32).
                                    // so 224 - 32 - 32 = 160

                                    //will point to 0 if no memory remaining

    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";

    //assert in cassert header file , where it checks the pointer behaviour and if false throws exception

    assert(a != nullptr);
    assert(b != nullptr);
    assert(a != b);         

    std::cout << "PASS: Basic allocation\n\n";

    std::cout<<" My memory in ALLOCATOR 1(256) "<< std::endl;
    alloc1.dump();
    std::cout<<"\n"<<std::endl;


    //test 2: free + reuse

    

        std::cout << "=== Free + Reuse ===\n";

    alloc1.deallocate(a);
    std::cout<<"Deallocating a..... " <<std::endl;

    void* c = alloc1.allocate(16);

    std::cout << "c (should reuse a): " << c << "\n";

    assert(c == a); // should reuse same block

    std::cout << "PASS: Reuse works\n\n";

    std::cout<<" My memory in ALLOCATOR 1(256) "<< std::endl;
    alloc1.dump();
    std::cout<<"\n"<<std::endl;

    //test 3: splitting

        std::cout << "=== Splitting Test ===\n";
        std::cout<<" Allocator 2(256) "<<std:: endl;

    FreeListAllocator alloc2(256);

    void* x = alloc2.allocate(32);  // 224 bytes because header = 32
    void* y = alloc2.allocate(16); //  160 bytes now, because size of previous block(32) - header(32).
                                    // so 224 - 32 - 32 = 160

    std::cout << "x: " << x << "\n";
    std::cout << "y: " << y << "\n";

    assert(x != y);

    std::cout << "PASS: Splitting works\n\n";

    std::cout<<" My memory in ALLOCATOR 2(256) "<< std::endl;
    alloc2.dump();
    std::cout<<"\n"<<std::endl;


    */

    //test 4: Coalescing
        //trying to allocate a used memory again and as well as which was splitted 

    //     std::cout << "=== Coalescing Test ===\n";
    //     std::cout<<"ALLOCATOR 3 (256)"<<std::endl;
    // FreeListAllocator alloc3(256);

   

    // void* p = alloc3.allocate(32);
   

    // void* q = alloc3.allocate(32);
  

    // void* r = alloc3.allocate(32);
   



    // // Layout: [p][q][r]

    // std::cout<<std:: endl;

    // std::cout<< "Layout: [p][q][r] "<<p <<"   "<< q << "   "<<r<<std::endl;

    // alloc3.deallocate(q);
   

    // alloc3.deallocate(p);
   

    // std::cout<<"Deallocated p and q"<<std::endl;

    // // Now p+q should be one big block

    // void* big = alloc3.allocate(60); // requires merged space

    // std::cout << "big: " << big << "\n";

    // assert(big == p); // should reuse merged region

    // std::cout << "PASS: Coalescing (prev) works\n\n";

    // alloc3.dump();
    // std::cout<<"\n"<<std::endl;


    // step 3 done 


    //=======================================STEP 4 Alignment===========================

    //test 1

    FreeListAllocator alloc3(256);

    void* a = alloc3.allocate(32, 8);
void* b = alloc3.allocate(32, 8);
void* c = alloc3.allocate(32, 8);

cout<<" allocated a , b , c "<<endl;
alloc3.dump();

alloc3.deallocate(b);
alloc3.deallocate(a);   // should merge A+B
alloc3.deallocate(c);   // should merge all

void* d = alloc3.allocate(96, 8);
alloc3.dump();
assert(d != nullptr);





cout<< "program looks good"<<endl;
    return 0;
}