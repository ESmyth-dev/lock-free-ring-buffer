#include <cassert>
#include <thread>
#include <iostream>
#include "buffer.hpp"

int main()
{
    constexpr int LOOPS{200};
    Buffer<int> buffer{1000};
    
    std::thread t1{[&buffer](){
    for (std::size_t i{0}; i < LOOPS; ++i){
    readd:
    auto result = buffer.Add(i);
    if (!result){
        goto readd;
    }
    }}};
    std::thread t2{[&buffer](){
    for (std::size_t i{0}; i < LOOPS; ++i){
    readd2:
    auto result = buffer.Add(i);
    if (!result){
        goto readd2;
    }
    }}};

    t1.join();
    t2.join();

    for (std::size_t i{0}; i < LOOPS*2; ++i)
    {
        auto result = buffer.Pop();
        if (result == std::nullopt)
        {
            std::cout << i << '\n';
            assert(false);
        }
    }

    return 0;
}