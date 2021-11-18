#pragma once

#define TRACKON 0


#include "TupplePairUtilities.hpp"
#include "PmrUseInCustomTypes.hpp"
#include "Allocators/PmrMemoryTracker.hpp"

/* this is used to prevent using global new / delete overloads to be used if not neccessary
   but to try some of the examples change TRACKON to 1
*/
#if TRACKON 
//#include "AllocationMetrics.h"
#include "Allocators/tracknew.hpp"
#endif



inline void PmrMemoryResource_Basics()
{
	fmt::print("\n---------Pmr Memory Resource w/ Standart Alloc---------\n");

    //TrackNew::reset();
   // AllocationMetrics::reset();
   // AllocationMetrics::printAllocMetrics();
    std::vector<std::string> vecStrng;
    for (auto i = 0; i < 1000; ++i)
    {
       vecStrng.emplace_back("just a non-SSO string");
       
    }
    //AllocationMetrics::printAllocMetrics();
    //TrackNew::status();
   // std::cout << allocCount <<", size: "<<allocSize<<'\n';
  
}

inline void PmrMonotonicBuffer_Basics()
{
    fmt::print("\n---------------StackAllocator_MemoryPool-------------------------\n");
      
    static std::array<std::byte, 256> buffer;

    std::cout << &*(buffer.data()) << '\n';

    for (int num : {1000, 2000, 500, 2000, 3000, 50000, 1000})
    {
        std::printf("--check with %d elements\n", num);
        
#if TRACKON
        TrackNew::reset();
        // AllocationMetrics::reset();
#endif

        std::pmr::monotonic_buffer_resource pool{ buffer.data(), buffer.size() };
        std::pmr::vector<std::pmr::string> coll{ &pool };
        for (auto i = 0; i < num; ++i)
        {
            coll.emplace_back("just a noo00000000000ooooon-SSO string");

        } 
#if TRACKON
        // AllocationMetrics::printAllocMetrics();
        TrackNew::status();
#endif
        }
}

inline void PmrSynchronizedPool_Reason()
{
    fmt::print("\n---------------Example to Show SynchronizedPool_Reason-------------------------\n");
    /* Example shows the address of each item in map can  be spread out in the memory
        which is in-efficient and also cause memory fragmentation that is why using a memory source that is allocated
        will make sure the items are stored contigously ; see next pmr::asynch example for that
    */
    std::map<long, std::string>coll1;

    for (int i = 0; i < 10; ++i)
    {
        std::string s{ "Customer: " + std::to_string(i) };
        coll1.emplace(i, s);
    }

    for (const auto& elem: coll1)
    {
        static long long lastVal = 0;
        long long val = reinterpret_cast<long long>(&elem);
        fmt::print("diff: {}\n", (lastVal - val));
        lastVal = val;
    }
}


inline void PmrSynchronizedPool_Basics()
{
    fmt::print("\n--------------PmrSynchronizedPool_Basics-------------------\n");
    
    /*if not specified this will use a default memory resource and synchronize_pool is thread safe!*/
    std::pmr::synchronized_pool_resource pool;
    std::pmr::map<long, std::pmr::string>coll2{ &pool };

    for (int i = 0; i < 10; ++i)
    {
        std::string s{ "Customer: " + std::to_string(i) };
        coll2.emplace(i, s);
    }

    for (const auto& elem : coll2)
    {
        static long long lastVal = 0;
        long long val = reinterpret_cast<long long>(&elem);
        fmt::print("diff: {}\n", (lastVal - val));
        lastVal = val;
    }
}

inline void PmrMonotonicBuff_SynchPool()
{
    fmt::print("\n------Using Pmr MonotonicBuff andSynchPool---------\n");
#if TRACKON
    TrackNew::reset();
#endif

    /*allocated chunk of memory and start with 10k and no deallocation*/
    std::pmr::monotonic_buffer_resource keepAllocatedPool{ 10000};
    /* synch pool will use first allocated at  monotonic buffer until it is full
       if needed synch will ask more and monotonic_buffer will allocate more
    */
    std::pmr::synchronized_pool_resource pool{ &keepAllocatedPool };

    for (int j = 0; j < 100; ++j)
    {
       std::pmr::vector<std::pmr::string> coll3{ &pool };
       for (int i = 0; i < 100; ++i)
       {
           coll3.emplace_back("just a non-SSO string");
       }

      
    }        // deallocations from pmr::Vector are given back to back to pool but not deallocated
#if TRACKON
    TrackNew::status();
#endif
    // the main buffer has not deallocated anythng since it still exists;
} // now the lifetime ends and the buffer deallocated all the memory


inline void PmrMonotonicBuff_NoHeap()
{
    fmt::print("\n------Using Pmr MonotonicBuff That does not use Heap Ever---------\n");

    std::array<std::byte, 10000> buffer;

    /*null_memory_resource is a upstream that does not use heap allocation therefore
      if the buffer is exhausted it throw bad_alloc exception
    */
    std::pmr::monotonic_buffer_resource pool{ buffer.data(), buffer.size(), std::pmr::null_memory_resource()};

    std::pmr::unordered_map<long, std::pmr::string>coll4{ &pool };
    try
    {
        for (int i = 0; i < buffer.size(); ++i)
        {
            std::string s{ "Customer" + std::to_string(i) };
            coll4.emplace(i, s);
        }

    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "BAD ALLOCATION: " << e.what() << '\n';
    }

    std::printf("buffer size: %zu", buffer.size());
}


inline void PmrMonotonicSynch_Tracker()
{
    fmt::print("\n------Using Pmr Monotonic - Synch Buff and Custom Tracker Derived from Memory_Resource---------\n");
    {
        /* uses default memory from get_default_memory*/
        Tracker track1{ "keeppool:" };
        /*since track1 is derived std::memory_resource the constructor uses track1*/
        std::pmr::monotonic_buffer_resource keeppool{ 10000, &track1 };
        {
            Tracker track2{ "   syncpool:", &keeppool };
            std::pmr::synchronized_pool_resource pool{ &track2 };

            for (int j = 0; j < 100; ++j)
            {
                std::pmr::vector<std::pmr::string> coll5{ &pool };
                coll5.reserve(100);
                for (int i = 0; i < 100; ++i)
                {
                    coll5.emplace_back("just a non SSO string");
                }
                if (j == 2) std::printf("--- third iteration done\n");
            } // deallocations are given back to pool but not deallocated

            //nothing allocated
            std::printf("--- leave scope of pool\n");
        }
        std::printf("leave scope of keeppool\n");
    }
}

inline void PmrForCustomTypes()
{
    fmt::print("\n------Using Pmr For Custom Types---------\n");
    Tracker track1;
    std::pmr::vector<PmrCustomer>coll6{ &track1 };   // allocates using track1 which underthehood use get_default_resource()
    coll6.reserve(100);

    PmrCustomer c1{ "Peter, Paul & Mary"};         // allocates with get_default_resource() (Alternatively  track1 or another memory resource can be passed in the constructor as an argument
    coll6.push_back(c1);                           // allocates with vector's track1 allocator
    coll6.push_back(std::move(c1));                // copies the allocator since PmrCustomer and vector uses different allocators but still goes into move constructor to move the name 

    for (const auto& cust : coll6)
    {
        std::printf(" cust name : %s\n", cust.getNameAsString().c_str());
    }

}

inline void PmrString_RegularStrings()
{
    fmt::print("\n------Using Pmr String and Regular String---------\n");

    std::string s{ "Salim" };
    std::pmr::string t1{ s+"move on" };
   // std::pmr::string t2 = s;  // error; no copy constructor
    s = t1;
    std::string s2 = std::string(t1); // this is expensive
    s2 = t1 + t1;
    
    std::cout << "s2: " << s2 << '\n';

    std::cout << "size of string s: " << sizeof s << '\n';
    std::cout << "size of pmr string t1: " << sizeof t1 << '\n';

    std::string s3{ "Salim and Didem Pamukcu" };
    PmrCustomer c1{ std::pmr::string{s3} }; // implicitly converts s3 to string_view 

}






