#ifndef MOVETYPES_H
#define MOVETYPES_H

#include "move.h"

class chessGame;
class chessPiece;

class PromotionMove: public Move {

public:
    PromotionMove(chessSquare * start, chessSquare * dest, chessBoard * board);

    void execute();

    void undo();

private:
    chessPiece * promotedPiece;
};


class EnPassantMove: public Move {

public:

    EnPassantMove(chessSquare * start, chessSquare * dest, chessBoard * board, chessGame * game);

    void execute();

    void undo();

private:
    chessGame * game;
};


class CastlingMove: public Move {
public:
    CastlingMove(chessSquare * start, chessSquare * dest, chessBoard * board);


    void execute();
    void undo();

private:

};


#endif // MOVETYPES_H
