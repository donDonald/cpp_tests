#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PieceKing : public PieceBase
{
public:
    PieceKing( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;

};

} }
