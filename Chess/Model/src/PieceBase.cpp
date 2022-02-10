#include "include/PieceBase.h"
#include <cassert>
#include <algorithm>

using namespace chess::api;

namespace chess { namespace impl {

PieceBase::PieceBase( const Color::Type& color,
                      const Type& type )
    : Piece( type, color )
    , m_touchCount( 0 )
{
}

unsigned int PieceBase::touchCount() const
{
    return m_touchCount;
}

void PieceBase::touch()
{
    ++m_touchCount;
}

void PieceBase::untouch()
{
    assert( m_touchCount > 0 );
    --m_touchCount;
}

bool isPositionInvalid( const Position& position )
{
    int file = position.file();
    int rank = position.rank();
    return ( file < 0 || file > 7 ) || ( rank < 0 || rank > 7 );
}

void PieceBase::clipBoard( std::vector<Position>& steps ) const
{
    steps.erase( std::remove_if( steps.begin(), steps.end(), isPositionInvalid ), steps.end() );
}

void PieceBase::possibleEats( const Position& position,
                              std::vector<Position>& result ) const
{
    return possibleMoves( position, result );
}

bool PieceBase::canJumpOver() const
{
    return false;
}

} }

