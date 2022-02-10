#pragma once

#include <memory>
#include "api/Types.h"
#include "api/Piece.h"

namespace chess { namespace impl {

class PieceBase : public api::Piece
{
public:
    PieceBase( const api::Color::Type& color,
               const Type& type = TypeUnknown );

public: // Own business logic
    unsigned int touchCount() const;
    void touch();
    void untouch();

    void clipBoard( std::vector<api::Position>& steps ) const;

    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const = 0;
    virtual void possibleEats( const api::Position& position,
                               std::vector<api::Position>& result ) const;
    virtual bool canJumpOver() const;

private:
    unsigned int      m_touchCount;

};

typedef std::shared_ptr<PieceBase> PieceBasePtr;

} }

