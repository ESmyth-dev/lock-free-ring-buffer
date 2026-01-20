template <typename DataType>
class Producer
{
    public:
    DataType Produce();

    private:
    std::size_t position;
};