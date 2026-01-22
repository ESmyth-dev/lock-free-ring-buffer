#include <cassert>
#include "buffer.hpp"

int main()
{
    Buffer<int> buffer{10};

    for (std::size_t i{0}; i < 10; ++i)
    {
        buffer.Add(2);
    }

    assert(buffer.Add(4)==false);
    buffer.Add(3);

    assert(buffer.Pop() == 4);

    buffer.Add(10);

    assert(buffer.Pop() == 3);

    return 0;
}