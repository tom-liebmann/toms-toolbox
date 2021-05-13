#pragma once


template < typename... TAttributes >
class BatchElement
{
};


template < typename TElement >
class ElementBatcher
{
public:
    class Handle;

    ElementBatcher( ttb::VertexBuffer& vertexBuffer, ttb::IndexBuffer& indexBuffer );

    Handle createElement();

    void update();

private:
    /** Maps each element index to the location in the packed geometry buffers. */
    std::vector< size_t > m_elementLocations;

    /** Maps each packed location to an element index. */
    std::vector< size_t > m_locationElements;
};


template < typename TElement >
class ElementBatcher< TElement >::Handle
{
public:
private:
    Handle( size_t index );

    size_t m_index;
};
