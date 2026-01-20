#include <vector>
#include <cstddef>
#include <consumer.hpp>
#include <producer.hpp>

template <typename DataType>
class Buffer
{
    public:
    Buffer();
    

    private:
    std::size_t capacity;
    std::size_t size;
    std::vector<DataType> contents;
};