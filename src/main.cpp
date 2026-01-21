#include <cassert>
#include "buffer.hpp"

int main()
{
    Buffer<int> buffer{10};

    buffer.Add(4);
    buffer.Add(3);

    assert(buffer.Pop() == 4);

    buffer.Add(10);

    assert(buffer.Pop() == 3);

    return 0;
}