#include "Game.h"
#include <QFileDialog>

using namespace chess;

static Game* gGame = 0;
Game* Game::instance()
{
    return gGame;
}

Game::Game( QObject *parent )
    : QObject( parent )
{
    Q_ASSERT( !gGame );
    gGame = this;
}

Game::~Game()
{
    Q_ASSERT( gGame );
    gGame = 0;
}

chess::api::GamePtr Game::gameApi()
{
    return m_pGame;
}

Board* Game::board() const
{
    return m_pBoard.get();
}

MoveResult* Game::moveResult()
{
    api::Move move;
    if( m_pGame->moveResult( move ) )
    {
        MoveResult::Type type = move.eaten() ? MoveResult::Eat : MoveResult::Move;
        m_moveResult.setType( type );
        m_moveResult.setFrom( QPoint( move.from().file(), move.from().rank() ) );
        m_moveResult.setTo( QPoint( move.to().file(), move.to().rank() ) );
        return &m_moveResult;
    }
    return NULL;
}

void Game::start()
{
    stop();
    m_pGame = api::Game::create();
    m_pBoard.reset( new Board() );
    m_pGame->start();
}

void Game::stop()
{
    m_pGame.reset();
}

bool Game::load()
{
    start();
    QString fileName = QFileDialog::getOpenFileName( 0, "Loading the game" );
    std::string sFileName = fileName.toStdString();
    if( !sFileName.empty() )
    {
        m_pGame->load( sFileName );
    }
    return !sFileName.empty();
}

bool Game::save()
{
    QString fileName = QFileDialog::getSaveFileName( 0, "Saving the game" );
    std::string sFileName = fileName.toStdString();
    if( !sFileName.empty() )
    {
        m_pGame->save( sFileName );
    }
    return !sFileName.empty();
}

bool Game::next()
{
    return m_pGame->moveForward();
}

bool Game::prev()
{
    return m_pGame->moveBack();
}

bool Game::move( int fromFile, int fromRank, int toFile, int toRank )
{
    return m_pGame->move(
                api::Position( api::File::Type( fromFile ), api::Rank::Type( fromRank ) ),
                api::Position( api::File::Type( toFile ), api::Rank::Type( toRank ) ) );
}

QVariantList Game::possibleMoves( int file, int rank )
{
    std::vector<api::Position> moves;
    m_pGame->possibleMoves(
                api::Position( api::File::Type( file ),
                               api::Rank::Type( rank ) ),
                moves );

    QVariantList positions;

    for( std::vector<api::Position>::const_iterator i = moves.begin();
         i != moves.end(); ++i )
    {
        positions.append( QVariant( QPoint( i->file(), i->rank() ) ) );
    }

    return positions;
}
