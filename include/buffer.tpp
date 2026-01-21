#include <optional>
#include "buffer.hpp"

template <typename DataType>
Buffer<DataType>::Buffer(std::size_t capacity)
: m_capacity{capacity}
, m_head{}
, m_tail{}
, m_contents(capacity)
{
    m_contents.reserve(capacity);
}

template <typename DataType>
bool Buffer<DataType>::Add(DataType item)
{
    if (ShiftIndex(m_head) == m_tail )
        return false;

    m_contents[m_head] = item;
    m_head = ShiftIndex(m_head);
    return true;
}

template <typename DataType>
std::optional<DataType> Buffer<DataType>::Pop()
{
    if (m_head == m_tail)
        return std::nullopt;

    auto item = m_contents[m_tail];

    m_tail = ShiftIndex(m_tail);

    return item;
}

template<typename DataType>
std::size_t Buffer<DataType>::ShiftIndex(std::size_t start)
{
    return (start + 1) % m_capacity;
}


