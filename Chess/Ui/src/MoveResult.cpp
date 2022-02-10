#include "MoveResult.h"


MoveResult::MoveResult( QObject *parent )
    : QObject( parent )
{
}

MoveResult::Type MoveResult::type() const
{
    return m_type;
}

void MoveResult::setType( Type type )
{
    m_type = type;
}

QPoint MoveResult::from() const
{
    return m_from;
}

void MoveResult::setFrom( QPoint from )
{
    m_from = from;
}

QPoint MoveResult::to() const
{
    return m_to;
}

void MoveResult::setTo( QPoint to )
{
    m_to = to;
}
