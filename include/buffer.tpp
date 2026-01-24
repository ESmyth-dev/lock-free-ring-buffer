#include <optional>
#include "buffer.hpp"

template <typename DataType>
Buffer<DataType>::Buffer(std::size_t capacity)
: m_capacity{capacity}
, m_head{}
, m_tail{m_capacity-1}
, m_contents(capacity)
{
    m_contents.reserve(capacity);
}

template <typename DataType>
bool Buffer<DataType>::Add(DataType item)
{
    auto head = m_head.load(std::memory_order::relaxed);
    if (head == m_tail.load(std::memory_order::relaxed))
        return false;

    m_contents[head] = item;
    head = ShiftIndex(head);
    m_head.store(head, std::memory_order::release);
    return true;
}

template <typename DataType>
std::optional<DataType> Buffer<DataType>::Pop()
{
    auto tail = m_tail.load(std::memory_order::relaxed);
    tail = ShiftIndex(tail);
    if (tail == m_head.load(std::memory_order::relaxed))
        return std::nullopt;

    auto item = m_contents[tail];
    m_tail.store(tail, std::memory_order::release);

    return item;
}

template<typename DataType>
std::size_t Buffer<DataType>::ShiftIndex(std::size_t start)
{
    return (start + 1) % m_capacity;
}


