#pragma once

#include <QObject>

class Piece : public QObject
{
    Q_OBJECT

public:    
    explicit Piece( QObject *parent = 0 );
    explicit Piece( int type, int color, QObject *parent = 0 );
    explicit Piece( const Piece& piece, QObject *parent = 0 );
    Q_PROPERTY(int type READ type )
    Q_PROPERTY(int color READ color )

signals:

public slots:

public:
    int type() const;
    void setType( int type );

    int color() const;
    void setColor( int color );

private:
    int m_type;
    int m_color;

};
