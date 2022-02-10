#include "Piece.h"

Piece::Piece( QObject *parent )
    : QObject( parent )
{
}

Piece::Piece( int type, int color, QObject *parent )
    : QObject( parent )
    , m_type( type )
    , m_color( color )
{
}

Piece::Piece( const Piece& piece, QObject *parent )
    : QObject( parent )
    , m_type( piece.type() )
    , m_color( piece.color() )
{
}

int Piece::type() const
{
    return m_type;
}

void Piece::setType( int type )
{
    m_type = type;
}

int Piece::color() const
{
    return m_color;
}

void Piece::setColor( int color )
{
    m_color = color;
}
