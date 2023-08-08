#include "chessGame.h"

#include "QTextStream"


chessGame::chessGame(chessBoard * board) : QObject()
{
    this->board = board;
    this->isBCheck = false;
    this->isWCheck = false;
    this->whosTurn = WHITE;
    this->whitePlayer = new chessPlayer(WHITE, this->board);
    this->blackPlayer = new chessPlayer(BLACK, this->board);
    this->wKingSquare = this->board->boardLocation[7][4];
    this->wKingPrevSquare = this->board->boardLocation[7][4];
    this->bKingSquare = this->board->boardLocation[0][4];
    this->bKingPrevSquare = this->board->boardLocation[0][4];
}

//return who makes the move this turn
char chessGame::getWhosTurn() {

    return this->whosTurn;
}

//Make a move. Executed a move with the given start and destination squares.
void chessGame::makeMove(Move * move) {//chessSquare * start, chessSquare * dest, bool promotionMove) {

    //Move * move = new Move(start, dest);
    //if (promotionMove) {
    move->setPromotionMove();
    //}
    move->execute();
    this->moveList.append(move);

    if (this->whosTurn == WHITE) {
        this->whosTurn = BLACK;
    } else if (this->whosTurn == BLACK) {
        this->whosTurn = WHITE;
    }

    emit turnChanged();
}



//return the last move of the game
Move * chessGame::getLastMove() {
    return this->moveList.last();
}

//Undo a move. En Passant moves are treated differently
void chessGame::undoMove() {
    //QTextStream out(stdout);
    if (this->moveList.size() > 0) {

        this->moveList.last()->undo();
        this->moveList.removeLast();
        if (this->whosTurn == WHITE) {
            this->whosTurn = BLACK;
            //out << "turn black" << endl;
            this->isChecked(BLACK);
        } else if (this->whosTurn == BLACK) {
            this->whosTurn = WHITE;
            //out << "turn white" << endl;
            this->isChecked(WHITE);
        }
        chessSquare * current = this->board->getCurrent();
        //out << current << endl;
        if (current != NULL) {
            this->board->resetView(current);
        }
    }

    emit turnChanged();
}


//return the White player
chessPlayer * chessGame::getPlayerWhite() {
    return this->whitePlayer;
}

//return the Black player
chessPlayer * chessGame::getPlayerBlack() {
    return this->blackPlayer;
}

//chess Player creation
chessPlayer::chessPlayer(char colour, chessBoard * board) {
    this->colour = colour;
    this->board = board;
    if (colour == WHITE) {
        for (int i = 6; i < DIMENSION; i++) {
            for (int j = 0; j < DIMENSION; j++) {
                if (this->board->boardLocation[i][j]->getPieceColour() == WHITE) {
                    this->pieces.append(this->board->boardLocation[i][j]->getPiece());
                }
            }
        }
    }
    if (colour == BLACK) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < DIMENSION; j++) {
                if (this->board->boardLocation[i][j]->getPieceColour() == BLACK) {
                    this->pieces.append(this->board->boardLocation[i][j]->getPiece());
                }
            }
        }
    }
}

//add a piece to the list of pieces
void chessPlayer::addPiece(chessPiece * piece) {
    (this->pieces).append(piece);
}

//Not implemented yet. Need to understand what to do.

void chessPlayer::removePiece(chessPiece *piece) {
    int i = 0;
    for (chessPiece * cPiece : this->pieces) {
        if (piece == cPiece) {
            this->pieces.removeAt(i);
            return;
        }
        i++;
    }
}

void chessGame::setKingSquare(char colour, chessSquare *square) {
    if (colour == WHITE) {
        this->wKingSquare = square;
    } else if (colour == BLACK) {
        this->bKingSquare = square;
    }
}

void chessGame::setKingPrevSquare(char colour, chessSquare *square) {
    if (colour == WHITE) {
        this->wKingPrevSquare = square;
    } else if (colour == BLACK) {
        this->bKingPrevSquare = square;
    }
}

chessSquare * chessGame::getKingSquare(char colour) {
    if (colour == WHITE) {
        return this->wKingSquare;
    } else {
        return this->bKingSquare;
    }
}

chessSquare * chessGame::getKingPrevSquare(char colour) {
    if (colour == WHITE) {
        return this->wKingPrevSquare;
    } else {
        return this->bKingPrevSquare;
    }
}

bool chessGame::isWChecked() {
    return this->isWCheck;
    //return this->isChecked(WHITE);
}

bool chessGame::isBChecked() {
    return this->isBCheck;
    //return this->isChecked(BLACK);
}

bool chessGame::isChecked(char colour) {
    //QTextStream out(stdout);
    chessSquare * kingSquare = this->getKingSquare(colour);
    //out << this->getKingSquare(BLACK)->getFile() << endl;
    //out << this->getKingSquare(BLACK)->getRank() << endl;
    if (King * tempKing = static_cast<King *>(kingSquare->getPiece())) {
        if (tempKing->isInCheckCurrent(kingSquare)) {
            if (colour == WHITE) {
                this->isWCheck = true;
            } else if (colour == BLACK) {
                this->isBCheck = true;
            }
            //out << "Checked!" << endl;
            return true;
        }
    }
    if (colour == WHITE) {
        this->isWCheck = false;
    } else if (colour == BLACK) {
        this->isBCheck = false;
    }
    return false;
}




