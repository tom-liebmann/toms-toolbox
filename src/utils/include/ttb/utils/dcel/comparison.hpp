#pragma once

#include "Dcel.hpp"

#include <type_traits>


namespace ttb::dcel
{
    template < typename TType >
    struct is_dcel_vertex_handle : std::false_type
    {
    };

    template < typename DcelType >
    struct is_dcel_vertex_handle< VertexHandle< DcelType > > : std::true_type
    {
    };

    template < typename DcelType >
    struct is_dcel_vertex_handle< ConstVertexHandle< DcelType > > : std::true_type
    {
    };

    template < typename TType >
    inline constexpr bool is_dcel_vertex_handle_v = is_dcel_vertex_handle< TType >::value;

    template < typename TType >
    concept DcelVertexHandle = is_dcel_vertex_handle_v< TType >;


    template < typename TType >
    struct is_dcel_edge_handle : std::false_type
    {
    };

    template < typename DcelType >
    struct is_dcel_edge_handle< EdgeHandle< DcelType > > : std::true_type
    {
    };

    template < typename DcelType >
    struct is_dcel_edge_handle< ConstEdgeHandle< DcelType > > : std::true_type
    {
    };

    template < typename TType >
    inline constexpr bool is_dcel_edge_handle_v = is_dcel_edge_handle< TType >::value;

    template < typename TType >
    concept DcelEdgeHandle = is_dcel_edge_handle_v< TType >;


    template < typename TType >
    struct is_dcel_face_handle : std::false_type
    {
    };

    template < typename DcelType >
    struct is_dcel_face_handle< FaceHandle< DcelType > > : std::true_type
    {
    };

    template < typename DcelType >
    struct is_dcel_face_handle< ConstFaceHandle< DcelType > > : std::true_type
    {
    };

    template < typename TType >
    inline constexpr bool is_dcel_face_handle_v = is_dcel_face_handle< TType >::value;

    template < typename TType >
    concept DcelFaceHandle = is_dcel_face_handle_v< TType >;


    template < DcelVertexHandle TType1, DcelVertexHandle TType2 >
    auto operator==( TType1 const& lhs, TType2 const& rhs ) -> bool
    {
        return lhs.getId() == rhs.getId();
    }

    template < DcelEdgeHandle TType1, DcelEdgeHandle TType2 >
    auto operator==( TType1 const& lhs, TType2 const& rhs ) -> bool
    {
        return lhs.getId() == rhs.getId();
    }

    template < DcelFaceHandle TType1, DcelFaceHandle TType2 >
    auto operator==( TType1 const& lhs, TType2 const& rhs ) -> bool
    {
        return lhs.getId() == rhs.getId();
    }
}
