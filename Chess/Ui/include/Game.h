#pragma once

#include <QObject>
#include <api/Game.h>
#include "Board.h"
#include "MoveResult.h"

class Game : public QObject
{
    Q_OBJECT
public:
    static Game* instance();

    explicit Game( QObject *parent = 0 );
    ~Game();
    Q_PROPERTY( Board* board READ board )
    Q_PROPERTY( MoveResult* moveResult READ moveResult )

signals:

public slots:

public:
    chess::api::GamePtr gameApi();
    Board* board() const;
    MoveResult* moveResult();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool load();
    Q_INVOKABLE bool save();
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool prev();
    Q_INVOKABLE bool move( int fromFile, int fromRank, int toFile, int toRank );
    Q_INVOKABLE QVariantList possibleMoves( int file, int rank );

private:
    chess::api::GamePtr     m_pGame;
    std::shared_ptr<Board>  m_pBoard;
    MoveResult              m_moveResult;
};
