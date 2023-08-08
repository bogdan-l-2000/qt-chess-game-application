#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "chessGame.h"
#include "chessPiece.h"

#include <QGridLayout>
#include <QList>
#include <QPushButton>
#include <QSignalMapper>
#include <QWidget>

const int DIMENSION = 8;

class chessGame;

class chessSquare : public QPushButton {

public:
    chessSquare(QWidget * parent = 0);

    bool operator==(const chessSquare &b);
    bool operator!=(const chessSquare &b);
    int getRank();
    int getFile();
    //char getColour();
    void setRank(int rank);
    void setFile(int file);
    void setColour(char colour);
    char getColour();

    void loadPiece(chessPiece * piece);
    char getPieceColour();
    chessPiece * removePiece();
    bool isEmpty();
    chessPiece * getPiece();

protected:

private:
    int rank;
    int file;
    chessPiece * piece;
    char colour;
    QIcon * img;
};


class chessBoard : public QWidget {

    Q_OBJECT
public:
    chessBoard(QWidget * parent = 0);
    chessSquare * boardLocation[DIMENSION][DIMENSION];
    void setGame(chessGame * game);

    void resetView(chessSquare * currSquare);
    void undoMove();
    Move * getLastMove();
    bool isEnPassantMove(chessSquare * start, chessSquare * dest);
    bool isbuttonSelected();
    chessSquare * getCurrent();

    bool isPromotionMove(chessSquare * start, chessSquare * dest);

    bool isCastlingMove(chessSquare * start, chessSquare * dest);

    bool isSideChecked(char colour);

    chessGame * getGame();

public slots:
    void showMoves(QWidget * wid);

protected:
    QGridLayout * board;
    QSignalMapper * signalMapper;

private:
    bool buttonSelected = false;
    chessSquare * current;
    chessGame * game;

    bool isWCheck = false;
    bool isBCheck = false;
};


#endif // CHESSBOARD_H
