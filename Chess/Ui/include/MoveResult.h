#pragma once

#include <QObject>
#include <QPoint>

class MoveResult : public QObject
{
    Q_OBJECT

public:
    explicit MoveResult( QObject *parent = 0 );

    enum Type {
        Move,
        Eat
    };
    Q_ENUMS(Type)

    Q_PROPERTY(Type type READ type )
    Q_PROPERTY(QPoint from READ from )
    Q_PROPERTY(QPoint to READ to )

signals:

public slots:

public:    
    Type type() const;
    void setType( Type type );

    QPoint from() const;
    void setFrom( QPoint from );

    QPoint to() const;
    void setTo( QPoint to );

private:
    Type   m_type;
    QPoint m_from;
    QPoint m_to;

};
