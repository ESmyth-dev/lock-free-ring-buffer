#include <catch2/catch_test_macros.hpp>

#include "buffer.hpp"



TEST_CASE("Correct behaviour when adding to/removing from the buffer", "[buffer]")
{
    Buffer<int> buffer{10};

    SECTION("Does not overflow when adding to a full buffer")
    {
        for(std::size_t i{0}; i<buffer.m_capacity; ++i)
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
        for(std::size_t i{0}; i<buffer.m_capacity; ++i)
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
