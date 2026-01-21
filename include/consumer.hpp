template <typename DataType>
class Consumer
{
    public:
    DataType Consume();

    private:
    std::size_t position;
};