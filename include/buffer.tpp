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
    if (m_head == m_tail && m_full)
        return false;

    m_contents[m_head] = item;
    m_head = ShiftIndex(m_head);
    if (m_head == m_tail)
        m_full = true;
    return true;
}

template <typename DataType>
std::optional<DataType> Buffer<DataType>::Pop()
{
    if (m_tail == m_head && !m_full)
        return std::nullopt;

    auto item = m_contents[m_tail];

    m_tail = ShiftIndex(m_tail);
    if (m_head == m_tail)
        m_full = false;
    return item;
}

template<typename DataType>
std::size_t Buffer<DataType>::ShiftIndex(std::size_t start)
{
    return (start + 1) % m_capacity;
}


