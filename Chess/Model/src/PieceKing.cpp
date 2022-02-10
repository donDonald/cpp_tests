#include "include/PieceKing.h"

using namespace chess::api;

namespace chess { namespace impl {

PieceKing::PieceKing( const Color::Type& color )
    : PieceBase( color, Piece::TypeKing )
{
}

void PieceKing::possibleMoves( const Position& position,
                               std::vector<Position>& result ) const
{
    result.clear();
    result.push_back( Position( position.file(),
                                Rank::Type( position.rank() + 1 ) ) ); // Up
    result.push_back( Position( position.file(),
                                Rank::Type( position.rank() - 1 ) ) ); // Down
    result.push_back( Position( File::Type( position.file() - 1 ),
                                position.rank() ) ); // Left
    result.push_back( Position( File::Type( position.file() + 1 ),
                                position.rank() ) ); // Right
    result.push_back( Position( File::Type( position.file() - 1 ),
                                Rank::Type( position.rank() + 1 ) ) ); // Up&Left
    result.push_back( Position( File::Type( position.file() + 1 ),
                                Rank::Type( position.rank() + 1 ) ) ); // Up&Right
    result.push_back( Position( File::Type( position.file() - 1 ),
                                Rank::Type( position.rank() - 1 ) ) ); // Down&Left
    result.push_back( Position( File::Type( position.file() + 1 ),
                                Rank::Type( position.rank() - 1 ) ) ); // Down&Right
    clipBoard( result );
}

} }
