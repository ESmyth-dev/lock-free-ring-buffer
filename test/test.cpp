#include <thread>
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
    constexpr int LOOPS{1000000};
    Buffer<int> buffer{1000};
    
    SECTION("Single producer, single consumer")
    {
        std::thread t1{[&buffer](){for (std::size_t i{0}; i < LOOPS; ++i){
        readd:
        auto result = buffer.Add(i);
        if (!result){
            goto readd;
        }
        }}};
    std::thread t2{[&buffer](){for (std::size_t i{0}; i < LOOPS; ++i){
        retry:
        auto result = buffer.Pop();
        if (result == std::nullopt)
        {
            goto retry;
        }
        REQUIRE(*result==i);}}};

    t1.join();
    t2.join();
    }
    
}