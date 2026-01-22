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
}
