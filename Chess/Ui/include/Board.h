#pragma once

#include <QObject>
#include <QPoint>
#include <QVariantList>
#include <api/Board.h>
#include "Piece.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board( QObject *parent = 0 );

signals:

public slots:

public:
    Q_INVOKABLE Piece* piece( int file, int rank );
    Q_INVOKABLE QVariantList eatenPieces();

private:
    Piece m_result;

};

