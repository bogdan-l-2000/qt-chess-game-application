#include "move.h"

#include <QTextStream>

class chessSquare;
class chessBoard;

/*
 * Move: a base class representation of a move done in Chess.
 * Contains a starting square, a destination square, and a chessboard.
 * Can contain a captured piece or a promoted piece.
 */


/*
 * Initializes a Move object with a start square, destination square, and Chessboard
 */
Move::Move(chessSquare * start, chessSquare * dest, chessBoard * board)
{
    this->start = start;
    this->dest = dest;
    this->board = board;
}


bool isValidMove() {


    return false;
}

void Move::setCapturedPiece(chessPiece *piece) {
    if (this->captured != NULL) {
        return;
    }
    this->captured = piece;

}

chessPiece * Move::capturedPiece() {
    return this->captured;
}

int Move::getStartRank() {
    return this->start->getRank();
}

int Move::getStartFile() {
    return this->start->getFile();
}

int Move::getDestRank() {
    return this->dest->getRank();
}

int Move::getDestFile() {
    return this->dest->getFile();
}

void Move::setEnPassantMove() {
    this->isEnPassantMove = true;
}

bool Move::getIsEnPassant() {
    return this->isEnPassantMove;
}

void Move::setPromotionMove() {
    this->promotion = true;
}

bool Move::getisPromotion() {
    return this->promotion;
}

chessPiece * Move::getPromotedPiece() {
    return this->promotedPiece;
}

void Move::setPromotedPiece(chessPiece * piece) {
    this->promotedPiece = piece;
}


void Move::execute() {
    //QTextStream out(stdout);
    chessPiece * tempPiece = (this->start)->removePiece();
    if (!tempPiece->didMove()) {
        tempPiece->moved();
        this->firstMove = true;
    }
    tempPiece->setCurFile(dest->getFile());
    tempPiece->setCurRank(dest->getRank());

    //out << tempPiece << endl;
    this->captured = this->dest->getPiece();
    (this->dest)->loadPiece(tempPiece);
    //out << this->dest->getPiece() << endl;
    if (this->captured != NULL) {
        if (this->captured->getColour() == WHITE) {
            this->board->getGame()->getPlayerWhite()->removePiece(this->captured);
        } else {
            this->board->getGame()->getPlayerBlack()->removePiece(this->captured);
        }
    }

    if (King * tempKing = dynamic_cast<King *>(tempPiece)) {
        this->board->getGame()->setKingSquare(tempKing->getColour(), this->dest);
    }

}

chessPiece * Move::movedPiece() {
    return this->dest->getPiece();
}


void Move::undo() {
    chessPiece * tempPiece = (this->dest)->removePiece();

    //Need to cover if it has been its first move.
    if (this->firstMove) {
        tempPiece->unMoved();
    }
    //if (!this->promotion) {
        (this->start)->loadPiece(tempPiece);
    /*} else {
        (this->start)->loadPiece(this->promotedPiece);
    }*/
    tempPiece->setCurFile(start->getFile());
    tempPiece->setCurRank(start->getRank());

    if (this->captured != NULL) {// && !this->isEnPassantMove) {
        (this->dest)->loadPiece(this->captured);
        if (this->captured->getColour() == WHITE) {
            this->board->getGame()->getPlayerWhite()->addPiece(this->captured);
        } else {
            this->board->getGame()->getPlayerBlack()->addPiece(this->captured);
        }
    }

    if (King * tempKing = dynamic_cast<King *>(tempPiece)) {
        this->board->getGame()->setKingSquare(tempKing->getColour(), this->start);
    }

}
