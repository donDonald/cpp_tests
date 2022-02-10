TEMPLATE = app

QT += qml quick gui widgets
CONFIG += c++11

INCLUDEPATH += $$PWD/../Model
INCLUDEPATH += $$PWD/include

SOURCES += \
    src/main.cpp \
    src/Game.cpp \
    src/Board.cpp \
    src/Piece.cpp \
    src/MoveResult.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/Game.h \
    include/Board.h \
    include/Piece.h \
    include/MoveResult.h

SOURCES += \
    ../Model/src/GameImpl.cpp \
    ../Model/src/BoardImpl.cpp \
    ../Model/src/SideImpl.cpp \
    ../Model/src/HistoryImpl.cpp \
    ../Model/src/Model.cpp \
    ../Model/src/PieceBishop.cpp \
    ../Model/src/PieceRook.cpp \
    ../Model/src/PieceQueen.cpp \
    ../Model/src/PiecePawn.cpp \
    ../Model/src/PieceKnight.cpp \
    ../Model/src/PieceKing.cpp \
    ../Model/src/PieceBase.cpp
