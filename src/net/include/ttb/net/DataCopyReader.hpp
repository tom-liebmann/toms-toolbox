#pragma once


namespace ttb
{
    class DataCopyReader : public DataReader
    {
    public:
        DataCopyReader( DataReader& reader );

        // Override: DataReader
        virtual size_t available() const override;
        virtual size_t read( void* data, size_t size ) override;

    private:
        std::vector< uint8_t > m_buffer;
        size_t m_offset;
    };
}
