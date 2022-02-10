#pragma once

#include <list>
#include <memory>

/**
 * Collects basic chess types.
 */

namespace chess { namespace api {


static const unsigned int KBoardMaxSize = 8;

typedef int Distance;
static const Distance KDistanceInfinite = 100;

/**
 * @brief Represents one of the chess colors, either white or black.
 */
class Color
{
public: // Types
    enum Type
    {
        White = 0,
        Black
    };
};

/**
 * @brief Represents moves directions.
 */
class Direction
{
public: // Types
    enum Type
    {
        Fwd = 0,
        Back,
        Left,
        Right,
        FwdLeft,
        FwdRight,
        BackLeft,
        BackRight,
        Undefined
    };
};

/**
 * @brief Horizontal position on the board( called files ).
 */
class File
{
public: // Types
    enum Type
    {
        fA = 0,
        fB,
        fC,
        fD,
        fE,
        fF,
        fG,
        fH
    };
};

/**
 * @brief Vertical position on the board( called ranks ).
 */
class Rank
{
public: // Types
    enum Type
    {
        r1 = 0,
        r2,
        r3,
        r4,
        r5,
        r6,
        r7,
        r8
    };
};

/**
 * @brief Position on the board.
 */
class Position
{
public:
    virtual ~Position() {}
    Position()
        : m_file( File::fA )
        , m_rank( Rank::r8 ) {}
    Position( const File::Type& file, const Rank::Type& rank )
        : m_file( file )
        , m_rank( rank ) {}
    Position( const Position& position )
        : m_file( position.m_file )
        , m_rank( position.m_rank ) {}

    bool operator<( const Position& rhs ) const
    {
        return ( ( m_rank*8 + m_file ) < ( rhs.m_rank*8 + rhs.m_file ) );
    }

    bool operator==( const Position& rhs ) const
    {
        return ( ( m_rank == rhs.m_rank ) && ( m_file == rhs.m_file ) );
    }

    const File::Type& file() const { return m_file; }
    void setFile( const File::Type& file ){ m_file = file; }
    const Rank::Type& rank() const { return m_rank; }
    void setRank( const Rank::Type& rank ) { m_rank = rank; }

private:
    File::Type   m_file;
    Rank::Type   m_rank;

};

} }
