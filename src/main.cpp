#include <cassert>
#include <thread>
#include <iostream>
#include <map>
#include <array>
#include "buffer.hpp"

int main()
{
    constexpr int LOOPS{2000};
    constexpr int NUM_PRODUCERS{3};
    Buffer<int> buffer{10000};
    std::array<std::thread, NUM_PRODUCERS> producers{};

    std::map<int, int> vals{};

    for (std::size_t i{0}; i < LOOPS; ++i)
    {
        vals[i] = NUM_PRODUCERS;
    }

    for (std::size_t i{0}; i < NUM_PRODUCERS; ++i)
    {
        producers[i] = (std::thread{[&buffer](){
        for (std::size_t i{0}; i < LOOPS; ++i){
        readd:
        auto result = buffer.Add(i);
        if (!result){
            goto readd;
        }
        }}});
    }

    std::thread consumer = std::thread{[&](){
    for (std::size_t i{0}; i < LOOPS*3; ++i)
    {
        reconsume:
        auto result = buffer.Pop();
        if (result == std::nullopt)
        {
            goto reconsume;
        }
        else if (vals[*result] == 0)
        {
            std::cout << *result << " seen more than 3 times.\n";
            assert(false);
        }
        else
        {
            vals[*result] -= 1;
        }
    }
    }};

    for (std::thread& thread : producers)
    {
        thread.join();
    }

    consumer.join();

    for (std::size_t i{0}; i < vals.size(); ++i)
    {
        assert(vals[i] == 0);
    }

    std::cout << "Work complete\n";
    return 0;
}