#ifndef MOVE_H
#define MOVE_H

#include "chessGame.h"

class chessSquare;
class chessPiece;
class chessBoard;

class Move {
public:
    Move(chessSquare * start, chessSquare * dest, chessBoard * board);
    virtual void execute();

    virtual void undo();

    int getStartRank();
    int getStartFile();
    int getDestRank();
    int getDestFile();

    void setCapturedPiece(chessPiece * piece);

    void setEnPassantMove();

    void setPromotionMove();

    bool getisPromotion();

    bool getIsEnPassant();

    chessPiece * movedPiece();

    chessPiece * capturedPiece();

    chessPiece * getPromotedPiece();

    void setPromotedPiece(chessPiece * piece);

protected:
    chessSquare * start;

    chessSquare * dest;

    chessPiece * captured;

    bool firstMove = false;

    bool isEnPassantMove = false;

    bool promotion = false;

    chessPiece * promotedPiece;

    chessBoard * board;
};


#endif // MOVE_H
