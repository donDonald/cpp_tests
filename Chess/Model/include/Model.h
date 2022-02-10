#pragma once

#include "api/Types.h"
#include "BoardImpl.h"
#include "SideImpl.h"
#include "HistoryImpl.h"
#include "PieceRook.h"
#include "PieceKnight.h"
#include "PieceBishop.h"
#include "PieceQueen.h"
#include "PieceKing.h"
#include "PiecePawn.h"


namespace chess { namespace impl {

class Model
{

public:
    Model();

    const BoardImpl& board() const;
    BoardImpl& board();

    const HistoryImpl& history() const;
    HistoryImpl& history();

    const SideImpl& whites() const;
    SideImpl& whites();

    const SideImpl& blacks() const;
    SideImpl& blacks();

private:
    BoardImpl     m_board;
    HistoryImpl   m_history;
    SideImpl      m_whites;
    SideImpl      m_blacks;

};

} }
