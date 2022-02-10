#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQuick/QQuickPaintedItem>

#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "MoveResult.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Game>("chess.api", 1, 0, "Game");
    qmlRegisterType<Board>("chess.api", 1, 0, "Board");
    qmlRegisterType<Piece>("chess.api", 1, 0, "Piece");
    qmlRegisterType<MoveResult>("chess.api", 1, 0, "MoveResult");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}

