#include "moveTypes.h"

#include <QTextStream>

PromotionMove::PromotionMove(chessSquare * start, chessSquare * dest, chessBoard * board) : Move(start, dest, board) {

}


void PromotionMove::execute() {
    chessPiece * tempPiece = (this->start)->removePiece();
    if (tempPiece->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->removePiece(tempPiece);
    } else {
        this->board->getGame()->getPlayerBlack()->removePiece(tempPiece);
    }
    if (!tempPiece->didMove()) {
        tempPiece->moved();
        this->firstMove = true;
    }

    this->captured = this->dest->getPiece();
    if (this->captured != NULL) {
        if (this->captured->getColour() == WHITE) {
            this->board->getGame()->getPlayerWhite()->removePiece(this->captured);
        } else {
            this->board->getGame()->getPlayerBlack()->removePiece(this->captured);
        }
    }
    this->promotedPiece = tempPiece;
    Queen * newPiece = new Queen(tempPiece->getColour(), this->board);
    (this->dest)->loadPiece(newPiece);
    if (newPiece->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->addPiece(newPiece);
    } else {
        this->board->getGame()->getPlayerBlack()->addPiece(newPiece);
    }
    newPiece->setCurRank(dest->getRank());
    newPiece->setCurFile(dest->getFile());
}

void PromotionMove::undo() {
    chessPiece * tempPiece = (this->dest)->removePiece();
    if (tempPiece->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->removePiece(tempPiece);
    } else {
        this->board->getGame()->getPlayerBlack()->removePiece(tempPiece);
    }

    //Need to cover if it has been its first move.
    if (this->firstMove) {
        tempPiece->unMoved();
    }

    (this->start)->loadPiece(this->promotedPiece);
    if (this->promotedPiece->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->addPiece(this->promotedPiece);
    } else {
        this->board->getGame()->getPlayerBlack()->addPiece(this->promotedPiece);
    }

    if (this->captured != NULL) {
        (this->dest)->loadPiece(this->captured);
        if (this->captured->getColour() == WHITE) {
            this->board->getGame()->getPlayerWhite()->addPiece(this->captured);
        } else {
            this->board->getGame()->getPlayerBlack()->addPiece(this->captured);
        }
    }


}

EnPassantMove::EnPassantMove(chessSquare * start, chessSquare * dest, chessBoard * board, chessGame * game) : Move(start, dest, board) {
    this->game = game;
}

void EnPassantMove::execute() {
    chessPiece * tempPiece = (this->start)->removePiece();
    if (!tempPiece->didMove()) {
        tempPiece->moved();
        this->firstMove = true;
    }
    QTextStream out(stdout);
    //out << tempPiece << endl;
    (this->dest)->loadPiece(tempPiece);
    chessPiece * capturedPiece = NULL;
    if (tempPiece->getColour() == WHITE) {
        capturedPiece = this->board->boardLocation[dest->getRank() + 1][dest->getFile()]->removePiece();
    } else if (tempPiece->getColour() == BLACK) {
        capturedPiece = this->board->boardLocation[dest->getRank() - 1][dest->getFile()]->removePiece();

    }
    tempPiece->setCurFile(dest->getFile());
    tempPiece->setCurRank(dest->getRank());
    out << capturedPiece << endl;
    this->captured = capturedPiece;
    if (this->captured->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->removePiece(this->captured);
    } else {
        this->board->getGame()->getPlayerBlack()->removePiece(this->captured);
    }
}

void EnPassantMove::undo() {
    chessPiece * tempPiece = (this->dest)->removePiece();

    //Need to cover if it has been its first move.
    if (this->firstMove) {
        tempPiece->unMoved();
    }
    if (tempPiece->getColour() == WHITE) {
        this->board->boardLocation[dest->getRank() + 1][dest->getFile()]->loadPiece(captured);
    } else {
        this->board->boardLocation[dest->getRank() - 1][dest->getFile()]->loadPiece(captured);

    }
    tempPiece->setCurFile(start->getFile());
    tempPiece->setCurRank(start->getRank());
    this->start->loadPiece(tempPiece);
    if (this->captured->getColour() == WHITE) {
        this->board->getGame()->getPlayerWhite()->addPiece(this->captured);
    } else {
        this->board->getGame()->getPlayerBlack()->addPiece(this->captured);
    }
}

CastlingMove::CastlingMove(chessSquare * start, chessSquare * dest, chessBoard * board) : Move(start, dest, board) {
}


void CastlingMove::execute() {
    chessPiece * tempKing = start->removePiece();
    //QTextStream out(stdout);

    if (dest->getFile() > start->getFile()) {
        chessPiece * tempRook = board->boardLocation[start->getRank()][7]->removePiece();

        board->boardLocation[start->getRank()][6]->loadPiece(tempKing);
        board->boardLocation[start->getRank()][5]->loadPiece(tempRook);
        tempKing->setCurFile(6);
        tempRook->setCurFile(5);
        //out << tempRook->getCurFile() << endl;
        //out << tempRook->getCurRank() << endl;
    }

    if (dest->getFile() < start->getFile()) {
        chessPiece * tempRook = board->boardLocation[start->getRank()][0]->removePiece();

        board->boardLocation[start->getRank()][2]->loadPiece(tempKing);
        board->boardLocation[start->getRank()][3]->loadPiece(tempRook);
        tempKing->setCurFile(2);
        tempKing->setCurFile(3);
    }
}

void CastlingMove::undo() {
    if (dest->getFile() > start->getFile()) {
        chessPiece * tempKing = board->boardLocation[start->getRank()][6]->removePiece();
        chessPiece * tempRook = board->boardLocation[start->getRank()][5]->removePiece();

        start->loadPiece(tempKing);
        tempKing->setCurFile(start->getFile());
        board->boardLocation[start->getRank()][7]->loadPiece(tempRook);
        tempRook->setCurFile(7);
    }
    if (dest->getFile() < start->getFile()) {
        chessPiece * tempKing = board->boardLocation[start->getRank()][2]->removePiece();
        chessPiece * tempRook = board->boardLocation[start->getRank()][3]->removePiece();

        start->loadPiece(tempKing);
        tempKing->setCurFile(start->getFile());

        board->boardLocation[start->getRank()][0]->loadPiece(tempRook);
        tempRook->setCurFile(0);

    }
}


