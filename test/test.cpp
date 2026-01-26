#include <thread>
#include <array>
#include <map>
#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "buffer.hpp"



TEST_CASE("Correct behaviour when adding to/removing from the buffer", "[buffer]")
{
    Buffer<int> buffer{10};

    SECTION("Does not overflow when adding to a full buffer")
    {
        for(std::size_t i{0}; i<buffer.m_capacity-1; ++i)
        {
            REQUIRE(buffer.Add(i));
        }

        REQUIRE_FALSE(buffer.Add(10));
    }

    SECTION("Buffer returns nothing when popping from empty")
    {
        REQUIRE(buffer.Pop() == std::nullopt);

        buffer.Add(10);
        buffer.Pop();

        REQUIRE(buffer.Pop() == std::nullopt);
    }

    SECTION("Allows removing from and then adding to a full buffer")
    {
        for(std::size_t i{0}; i<buffer.m_capacity-1; ++i)
        {
            REQUIRE(buffer.Add(i));
        }

        REQUIRE(buffer.Pop() != std::nullopt);

        REQUIRE(buffer.Add(1));
    }

    SECTION("Items are removed from the buffer in the order they are added")
    {
        for(std::size_t i{0}; i<5; ++i)
        {
            REQUIRE(buffer.Add(i));
        }

        REQUIRE(buffer.Pop() == 0);
        REQUIRE(buffer.Pop() == 1);
        REQUIRE(buffer.Pop() == 2);
        REQUIRE(buffer.Pop() == 3);
        REQUIRE(buffer.Add(1));
        REQUIRE(buffer.Pop() == 4);
        REQUIRE(buffer.Pop() == 1);
    }
}

TEST_CASE("Correct behaviour when running concurrently", "[buffer]")
{   
    SECTION("Single producer, single consumer")
    {
        constexpr int LOOPS{1000000};
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
            retry:
            auto result = buffer.Pop();
            if (result == std::nullopt)
            {
                goto retry;
            }
            REQUIRE(*result==i);
        }}};

        t1.join();
        t2.join();
    }

    SECTION("Multi producer, single deferred consumer")
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

        for (std::thread& thread : producers)
        {
            thread.join();
        }

        for (std::size_t i{0}; i < LOOPS*3; ++i)
        {
            auto result = buffer.Pop();
            REQUIRE_FALSE(result == std::nullopt);
            REQUIRE_FALSE(vals[*result] == 0);
            vals[*result] -= 1;
        }

        for (std::size_t i{0}; i < vals.size(); ++i)
        {
            assert(vals[i] == 0);
        }
    }
    
}