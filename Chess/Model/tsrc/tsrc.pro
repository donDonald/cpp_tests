#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T15:41:49
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = tst_tsrctest
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

INCLUDEPATH += ..
INCLUDEPATH += ..

TEMPLATE = app

HEADERS += \
    ../api/Types.h \
    ../api/Piece.h \
    ../api/Board.h \
    ../api/Side.h \
    ../api/History.h \
    ../api/Game.h \
    ../include/Types.h \
    ../include/PieceKing.h \
    ../include/PieceQueen.h \
    ../include/PieceRook.h \
    ../include/PieceKnight.h \
    ../include/PieceBishop.h \
    ../include/PiecePawn.h \
    ../include/Model.h \
    ../include/GameImpl.h \
    ../include/BoardImpl.h \
    ../include/HistoryImpl.h \
    ../include/PieceBase.h \
    ../include/SideImpl.h \
    ../api/Move.h

SOURCES += \
    ../src/Model.cpp \
    ../src/PieceBishop.cpp \
    ../src/PieceRook.cpp \
    ../src/PieceQueen.cpp \
    ../src/PiecePawn.cpp \
    ../src/PieceKnight.cpp \
    ../src/PieceKing.cpp \
    ../src/GameImpl.cpp \
    ../src/BoardImpl.cpp \
    ../src/HistoryImpl.cpp \
    ../src/PieceBase.cpp \
    ../src/SideImpl.cpp \
    tst_src.cpp \

SOURCES +=
DEFINES += SRCDIR=\\\"$$PWD/\\\"
