#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chessBoard.h"
#include "chessPiece.h"
#include "move.h"

#include <QList>
#include <QObject>

class chessSquare;
class chessBoard;
class chessPiece;
class Move;

class chessPlayer {

public:
    chessPlayer(char colour, chessBoard * board);
    void addPiece(chessPiece * piece);
    void removePiece(chessPiece * piece);
    bool isInCheck(chessSquare * square);
    QList<chessPiece *> pieces;

private:
    char colour;
    bool inCheck = false;
    //chessSquare * kingsSquare;
    chessBoard * board;
};

class chessGame : public QObject {

    Q_OBJECT
public:
    chessGame(chessBoard * board);

    char getWhosTurn();
    void makeMove(Move * move);//chessSquare * start, chessSquare * dest, bool promotionMove);

    chessPlayer * getPlayerWhite();
    chessPlayer * getPlayerBlack();

    Move * getLastMove();
    void undoMove();
    bool isChecked(char colour);

    void setKingSquare(char colour, chessSquare * square);
    void setKingPrevSquare(char colour, chessSquare * square);

    chessSquare * getKingSquare(char colour);
    chessSquare * getKingPrevSquare(char colour);

    bool isWChecked();
    bool isBChecked();

signals:
    void turnChanged();

private:
    bool isWCheck = false; //white side is checked
    bool isBCheck = false; //black side is checked
    chessBoard * board;
    char whosTurn;
    chessPlayer * whitePlayer;
    chessPlayer * blackPlayer;
    QList<Move *> moveList;
    bool isGameOver = false;
    chessSquare * wKingSquare;
    chessSquare * wKingPrevSquare;
    chessSquare * bKingSquare;
    chessSquare * bKingPrevSquare;
};


#endif // CHESSGAME_H
