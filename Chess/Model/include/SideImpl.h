#pragma once

#include "api/Types.h"
#include "api/Move.h"
#include "api/Piece.h"
#include "api/Side.h"
#include "BoardImpl.h"
#include "HistoryImpl.h"

namespace chess { namespace impl {

class SideImpl : public api::Side
{
public:
    SideImpl( const api::Color::Type& color,
              BoardImpl& board,
              HistoryImpl& history,
              api::Board::EatenPieces& eaten );

public: // From Side
    virtual void possibleMoves(
            const api::Position& position,
            std::vector<api::Position>& moves );

public: // Own business logic
    bool move( const api::Position& from,
               const api::Position& to );
    bool moveBack();
    bool moveResult( api::Move& move ) const;

private:
    api::Color::Type         m_color;
    BoardImpl&               m_board;
    HistoryImpl&             m_history;
    api::Board::EatenPieces& m_eaten;
    bool                     m_hasChanged;
    api::Move                m_moveResult;

};

} }
