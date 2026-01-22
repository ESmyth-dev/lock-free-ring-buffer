#pragma once

#include <vector>
#include <optional>
#include <cstddef>

template <typename DataType>
class Buffer
{
    public:
    Buffer(std::size_t capacity);

    bool Add(DataType item);

    std::optional<DataType> Pop();
    
    std::size_t m_capacity;

    private:
    std::size_t m_head;
    std::size_t m_tail;
    bool m_full{false};
    std::vector<DataType> m_contents;

    std::size_t ShiftIndex(std::size_t start);

};

#include "buffer.tpp"