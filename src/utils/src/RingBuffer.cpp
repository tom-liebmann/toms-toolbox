#include <ttb/utils/RingBuffer.hpp>


namespace ttb
{
    RingBuffer::RingBuffer( size_t capacity ) : m_buffer( capacity )
    {
    }

    size_t RingBuffer::capacity() const
    {
        return m_buffer.size();
    }

    size_t RingBuffer::size() const
    {
        return m_used;
    }

    size_t RingBuffer::emptySpace() const
    {
        return capacity() - size();
    }

    bool RingBuffer::empty() const
    {
        return m_used == 0;
    }

    bool RingBuffer::full() const
    {
        return emptySpace() == 0;
    }

    size_t RingBuffer::blockSize() const
    {
        if( m_used == 0 )
        {
            return 0;
        }

        if( m_start >= m_end )
        {
            return m_buffer.size() - m_start;
        }
        else
        {
            return m_end - m_start;
        }
    }

    void RingBuffer::clear()
    {
        m_start = 0;
        m_end = 0;
        m_used = 0;
    }

    bool RingBuffer::pushAll( void const* data, size_t dataSize )
    {
        if( dataSize <= emptySpace() )
        {
            (void)push( data, dataSize );
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t RingBuffer::push( void const* data, size_t dataSize )
    {
        if( dataSize == 0 || full() )
        {
            return 0;
        }

        auto const dataPtr = reinterpret_cast< uint8_t const* >( data );

        if( m_start <= m_end )
        {
            size_t const blockSize = std::min( dataSize, m_buffer.size() - m_end );

            std::copy( dataPtr, dataPtr + blockSize, m_buffer.data() + m_end );
            m_end = ( m_end + blockSize ) % m_buffer.size();
            m_used += blockSize;

            if( blockSize < dataSize )
            {
                return blockSize + push( dataPtr + blockSize, dataSize - blockSize );
            }
            else
            {
                return blockSize;
            }
        }
        else
        {
            size_t const blockSize = std::min( dataSize, m_start - m_end );

            std::copy( dataPtr, dataPtr + blockSize, m_buffer.data() + m_end );
            m_end += blockSize;
            m_used += blockSize;

            return blockSize;
        }
    }

    void RingBuffer::pop( size_t dataSize )
    {
        m_start = ( m_start + dataSize ) % m_buffer.size();
        m_used -= dataSize;

        if( m_start == m_end )
        {
            m_start = 0;
            m_end = 0;
        }
    }

    bool RingBuffer::popAll( void* dst, size_t dstSize )
    {
        if( dstSize <= size() )
        {
            (void)pop( dst, dstSize );
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t RingBuffer::pop( void* dst, size_t dstSize )
    {
        if( dstSize == 0 || empty() )
        {
            return 0;
        }

        auto const dstPtr = reinterpret_cast< uint8_t* >( dst );

        if( m_start >= m_end )
        {
            size_t const blockSize = std::min( dstSize, m_buffer.size() - m_start );

            std::copy( data(), data() + blockSize, dstPtr );
            m_start = ( m_start + blockSize ) % m_buffer.size();
            m_used -= blockSize;

            if( m_used == 0 )
            {
                m_start = 0;
                m_end = 0;
            }

            if( blockSize < dstSize )
            {
                return blockSize + pop( dstPtr + blockSize, dstSize - blockSize );
            }
            else
            {
                return blockSize;
            }
        }
        else
        {
            size_t const blockSize = std::min( dstSize, m_end - m_start );

            std::copy( data(), data() + blockSize, dstPtr );
            m_start += blockSize;
            m_used -= blockSize;

            if( m_used == 0 )
            {
                m_start = 0;
                m_end = 0;
            }

            return blockSize;
        }
    }

    uint8_t* RingBuffer::data()
    {
        return m_buffer.data() + m_start;
    }

    uint8_t const* RingBuffer::data() const
    {
        return m_buffer.data() + m_start;
    }
}
