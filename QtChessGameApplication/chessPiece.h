#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "chessBoard.h"
#include "move.h"

#include <cmath>
#include <QIcon>
#include <QList>


const static char WHITE = 'w';
const static char BLACK = 'b';
const static char NONE = 'n';

class chessBoard;
class chessSquare;
class Move;

class chessPiece : public QObject
{

    Q_OBJECT
public:
    chessPiece(char colour, chessBoard * board);

    virtual bool isValidMove(chessSquare * start, chessSquare * dest) = 0;

    char getColour();

    QIcon getWIcon();
    QIcon getBIcon();

    bool isDiagonal(chessSquare * start, chessSquare * dest, chessBoard * cBoard);
    bool isLShape(chessSquare * start, chessSquare * dest);
    bool isStraight(chessSquare * start, chessSquare * dest, chessBoard * cBoard);
    bool isAvailableDirectionHelper(chessSquare * start, chessSquare * dest, int rankDir, int fileDir, chessBoard * cBoard);

    char getOther();

    void moved();
    void unMoved();

    bool didMove();

    bool isKingChecked(chessSquare * start, chessSquare * dest);

    int getCurRank();
    int getCurFile();

    void setCurRank(int rank);
    void setCurFile(int file);

protected:
    char colour;
    bool isCaptured = false;
    QList<Move> moves;
    QIcon * wIcon;
    QIcon * bIcon;
    chessBoard * board;
    bool hasMoved = false;
    int curRank;
    int curFile;
};


class Pawn : public chessPiece {

    Q_OBJECT
public:
    Pawn(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);

signals:
    void EnPassantCapture();

private:

};


class Knight : public chessPiece {

public:
    Knight(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);
};


class Bishop : public chessPiece {

public:
    Bishop(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);
};


class Rook : public chessPiece {

public:
    Rook(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);

private:

};


class Queen : public chessPiece {

public:
    Queen(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);
};


class King : public chessPiece {

public:
    King(char colour, chessBoard * board);
    bool isValidMove(chessSquare * start, chessSquare * dest);
    bool isAbleToCastle(int direction);
    //bool isInCheckMove(chessSquare * square);
    bool isInCheckCurrent(chessSquare * current);

private:
    bool inCheck = false;
    bool canCastle = false;
};


#endif // CHESSPIECE_H
