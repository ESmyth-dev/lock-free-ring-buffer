#include <cassert>
#include <thread>
#include <iostream>
#include "buffer.hpp"

int main()
{
    constexpr int LOOPS{1000000};
    Buffer<int> buffer{1000};
    

    std::thread t1{[&buffer](){for (std::size_t i{0}; i < LOOPS; ++i){buffer.Add(i);}}};
    std::thread t2{[&buffer](){for (std::size_t i{0}; i < LOOPS; ++i){
        retry:
        auto result = buffer.Pop();
        if (result == std::nullopt)
        {
            goto retry;
        }
        assert(*result==i);}}};

    t1.join();
    t2.join();
    return 0;
}