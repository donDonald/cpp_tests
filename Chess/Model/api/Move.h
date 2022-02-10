#pragma once

#include "api/Types.h"
#include "api/Piece.h"

namespace chess { namespace api {

class Move
{
public:
    typedef bool EatenType;

    Move() {}
    Move( const Position& from,
          const Position& to,
          EatenType eaten )
        : m_from( from )
        , m_to( to )
        , m_eaten( eaten ) {}

    const Position& from() const { return m_from; }
    const Position& to() const { return m_to; }
    EatenType eaten() const { return m_eaten; }

private:
    Position  m_from;
    Position  m_to;
    EatenType m_eaten;

};

} }
