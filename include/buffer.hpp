#pragma once

#include <vector>
#include <utility>
#include <atomic>
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
    std::atomic<std::size_t> m_writeHead;
    std::atomic<std::size_t> m_readHead;
    std::atomic<std::size_t> m_tail;
    std::vector<std::pair<std::atomic_bool,DataType>> m_contents;

    std::size_t ShiftIndex(std::size_t start);

};

#include "buffer.tpp"