#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PieceRook : public PieceBase
{
public:
    PieceRook( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;

};

} }


