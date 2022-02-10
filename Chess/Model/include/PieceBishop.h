#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PieceBishop : public PieceBase
{
public:
    PieceBishop( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;

};

} }
