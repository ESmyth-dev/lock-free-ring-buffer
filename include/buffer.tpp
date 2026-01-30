#include <optional>
#include "buffer.hpp"

template <typename DataType>
Buffer<DataType>::Buffer(std::size_t capacity)
: m_capacity{capacity}
, m_writeHead{}
, m_readHead{}
, m_tail{m_capacity-1}
, m_contents(capacity)
{
    m_contents.reserve(capacity);
}

template <typename DataType>
bool Buffer<DataType>::Add(DataType item)
{
    auto oldHead = m_writeHead.load(std::memory_order::acquire);
    if (oldHead == m_tail.load(std::memory_order::relaxed))
        return false;
    auto newHead = ShiftIndex(oldHead);
    auto result = m_writeHead.compare_exchange_strong(oldHead, newHead, std::memory_order::release);
    if(!result)
        return false;

    m_contents[oldHead] = item;

    while(m_readHead.load() != oldHead)
    {
        std::this_thread::yield();
    }

    m_readHead.store(newHead);

    return true;
}

template <typename DataType>
std::optional<DataType> Buffer<DataType>::Pop()
{
    auto oldTail = m_tail.load(std::memory_order::acquire);
    auto newTail = ShiftIndex(oldTail);
    if (newTail == m_readHead.load(std::memory_order::relaxed))
        return std::nullopt;

    auto item = m_contents[newTail];
    auto result = m_tail.compare_exchange_strong(oldTail, newTail, std::memory_order::release);
    if (result)
        return item;
    
    return std::nullopt;
}

template<typename DataType>
std::size_t Buffer<DataType>::ShiftIndex(std::size_t start)
{
    return (start + 1) % m_capacity;
}


