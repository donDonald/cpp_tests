#include "include/Model.h"

using namespace chess::api;

namespace chess { namespace impl {

Model::Model()
    : m_whites( Color::White, board(), history(), board().eatenPieces() )
    , m_blacks( Color::Black, board(), history(), board().eatenPieces() )
{
}

const BoardImpl& Model::board() const
{
    return m_board;
}

BoardImpl& Model::board()
{
    return m_board;
}

const HistoryImpl& Model::history() const
{
    return m_history;
}

HistoryImpl& Model::history()
{
    return m_history;
}

const SideImpl& Model::whites() const
{
    return m_whites;
}

SideImpl& Model::whites()
{
    return m_whites;
}

const SideImpl& Model::blacks() const
{
    return m_blacks;
}

SideImpl& Model::blacks()
{
    return m_blacks;
}

} }
