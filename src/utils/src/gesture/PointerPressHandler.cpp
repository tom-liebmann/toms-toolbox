#include <ttb/utils/gesture/PointerPressHandler.hpp>

#include <ttb/math/vector_operations.hpp>
#include <ttb/utils/gesture/events.hpp>

#include <sstream>


namespace ttb
{
    PointerPressHandler::PointerPressHandler( EventCallback eventCallback, float moveTolerance )
        : m_eventCallback{ std::move( eventCallback ) }, m_moveTolerance{ moveTolerance }
    {
    }

    bool PointerPressHandler::onEvent( Event const& event )
    {
        using namespace ttb::event;
        switch( event.type() )
        {
            case type::POINTER_DOWN:
                return onPointerDown( static_cast< events::PointerDown const& >( event ) );
            case type::POINTER_UP:
                return onPointerUp( static_cast< events::PointerUp const& >( event ) );
            case type::POINTER_MOVE:
                return onPointerMove( static_cast< events::PointerMove const& >( event ) );
            default:
                return false;
        }
    }

    bool PointerPressHandler::onPointerDown( events::PointerDown const& event )
    {
        switch( m_state )
        {
            case State::IDLE:
            {
                m_activePointers.insert(
                    { event.pointerId(), PointerInfo{ { event.x(), event.y() } } } );
                m_state = State::PRESS;

                // send press event
                {
                    auto const e = events::PointerPressStart{ event.pointerType(),
                                                              event.pointerId(),
                                                              { event.x(), event.y() } };
                    m_eventCallback( e );
                }

                return false;
            }

            case State::PRESS:
            {
                // abort currently active press event
                {
                    auto const pointerId = m_activePointers.begin()->first;

                    auto const e = events::PointerPressAbort{ pointerId };
                    m_eventCallback( e );
                }

                m_activePointers.insert(
                    { event.pointerId(), PointerInfo{ { event.x(), event.y() } } } );
                m_state = State::TRANSFORM;

                return false;
            }

            case State::DRAG:
            {
                // abort currently active drag event
                {
                    auto const& dragEndPosition = m_activePointers.begin()->second.position;
                    auto const e = events::DragEnd{ dragEndPosition };
                    m_eventCallback( e );
                }

                m_activePointers.insert(
                    { event.pointerId(), PointerInfo{ { event.x(), event.y() } } } );
                m_state = State::TRANSFORM;

                return false;
            }

            case State::TRANSFORM:
            {
                m_activePointers.insert(
                    { event.pointerId(), PointerInfo{ { event.x(), event.y() } } } );
                m_state = State::NONE;

                return false;
            }

            case State::NONE:
            {
                return false;
            }
        }

        return false;
    }

    bool PointerPressHandler::onPointerUp( events::PointerUp const& event )
    {
        switch( m_state )
        {
            case State::IDLE:
            {
                return false;
            }

            case State::NONE:
            {
                m_activePointers.erase( event.pointerId() );

                if( m_activePointers.empty() )
                {
                    m_state = State::IDLE;
                }

                return false;
            }

            case State::TRANSFORM:
            {
                m_activePointers.erase( event.pointerId() );
                m_state = State::NONE;

                return false;
            }

            case State::DRAG:
            {
                m_activePointers.clear();
                m_state = State::IDLE;

                auto const e = events::DragEnd{ { event.x(), event.y() } };
                m_eventCallback( e );
                return false;
            }

            case State::PRESS:
            {
                m_activePointers.clear();
                m_state = State::IDLE;

                auto const e =
                    events::PointerPressEnd{ event.pointerId(), { event.x(), event.y() } };
                m_eventCallback( e );
                return false;
            }
        }

        return false;
    }

    bool PointerPressHandler::onPointerMove( events::PointerMove const& event )
    {
        switch( m_state )
        {
            case State::IDLE:
            case State::NONE:
            {
                return false;
            }

            case State::TRANSFORM:
            {
                auto& info1 = m_activePointers.begin()->second;
                auto& info2 = std::next( m_activePointers.begin() )->second;

                auto const p1old = info1.position;
                auto const p2old = info2.position;

                m_activePointers[ event.pointerId() ].position = { event.x(), event.y() };

                auto const p1new = info1.position;
                auto const p2new = info2.position;

                auto const diffOld = p2old - p1old;
                auto const diffNew = p2new - p1new;

                auto const lenOld = ttb::norm( diffOld );
                auto const lenNew = ttb::norm( diffNew );

                if( lenOld > 1e-5 && lenNew > 1e-5 )
                {
                    float const zoomFactor = lenOld / lenNew;

                    // TODO: Compute and pass in rotation
                    m_eventCallback( events::Zoom{ zoomFactor } );
                }

                return false;
            }

            case State::DRAG:
            {
                auto& info = m_activePointers[ event.pointerId() ];
                info.position = { event.x(), event.y() };

                m_eventCallback( events::DragMove{ { event.x(), event.y() } } );
                return false;
            }

            case State::PRESS:
            {
                auto& info = m_activePointers[ event.pointerId() ];
                auto const eventPosition = ttb::Vector< float, 2 >{ event.x(), event.y() };

                if( ttb::norm2( eventPosition - info.position ) >
                    std::pow( m_moveTolerance, 2.0f ) )
                {
                    m_state = State::DRAG;

                    // abort active press
                    m_eventCallback( events::PointerPressAbort{ event.pointerId() } );

                    // start drag
                    {
                        // start event
                        m_eventCallback( events::DragStart{ info.position } );

                        info.position = eventPosition;

                        // move event
                        m_eventCallback( events::DragMove{ eventPosition } );
                    }
                }

                return false;
            }
        }

        return false;
    }
}
