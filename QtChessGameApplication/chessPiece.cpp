#include "chessPiece.h"

#include <QTextStream>

chessPiece::chessPiece(char colour, chessBoard * board)
{
    this->colour = colour;
    this->board = board;
}

QIcon chessPiece::getBIcon() {
    return *(this->bIcon);
}

QIcon chessPiece::getWIcon() {
    return *(this->wIcon);
}

char chessPiece::getColour() {
    return this->colour;
}


char chessPiece::getOther() {
    if (this->colour == WHITE) {
        return BLACK;
    } else if (this->colour == BLACK) {
        return WHITE;
    } else {
        return NONE;
    }
}

void chessPiece::moved() {
    this->hasMoved = true;
}

void chessPiece::unMoved() {
    this->hasMoved = false;
}

bool chessPiece::didMove() {
    return this->hasMoved;
}

void chessPiece::setCurFile(int file) {
    this->curFile = file;
}

void chessPiece::setCurRank(int rank) {
    this->curRank = rank;
}

int chessPiece::getCurFile() {
    return this->curFile;
}

int chessPiece::getCurRank() {
    return this->curRank;
}

bool chessPiece::isLShape(chessSquare *start, chessSquare *dest) {
    if (!dest->isEmpty() && (dest->getPieceColour() == this->getColour())) {
        return false;
    }
    if (abs(start->getFile() - dest->getFile()) == 2 && abs(start->getRank() - dest->getRank()) == 1) {
        if (this->isKingChecked(start, dest)) {
            return false;
        }
        return true;
    } else if (abs(start->getFile() - dest->getFile()) == 1 && abs(start->getRank() - dest->getRank()) == 2) {
        if (this->isKingChecked(start, dest)) {
            return false;
        }
        return true;
    }
    return false;
}

bool chessPiece::isDiagonal(chessSquare * start, chessSquare * dest, chessBoard  *cBoard) {

    QTextStream out (stdout);
    if (abs(start->getFile() - dest->getFile()) != abs(start->getRank() - dest->getRank())) {
        return false;
    }
    if (start->getFile() > dest->getFile() && start->getRank() > dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, -1, -1, cBoard);
    } else if (start->getFile() > dest->getFile() && start->getRank() < dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, 1, -1, cBoard);
    } else if (start->getFile() < dest->getFile() && start->getRank() > dest->getRank()) {
        return this->isAvailableDirectionHelper(start, dest, -1, 1, cBoard);
    } else if (start->getFile() < dest->getFile() && start->getRank() < dest->getRank()) {
        return this->isAvailableDirectionHelper(start, dest, 1, 1, cBoard);
    }
    return false;
}

bool chessPiece::isStraight(chessSquare *start, chessSquare *dest, chessBoard *cBoard) {
    if (start->getRank() != dest->getRank() && start->getFile() != dest->getFile()) {
        return false;
    }
    if (start->getFile() == dest->getFile() && start->getRank() < dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, 1, 0, cBoard);
    } else if (start->getFile() == dest->getFile() && start->getRank() > dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, -1, 0, cBoard);
    } else if (start->getFile() < dest->getFile() && start->getRank() == dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, 0, 1, cBoard);
    } else if (start->getFile() > dest->getFile() && start->getRank() == dest->getRank()) {
        return isAvailableDirectionHelper(start, dest, 0, -1, cBoard);
    }
    return false;
}


bool chessPiece::isAvailableDirectionHelper(chessSquare * start, chessSquare * dest, int rankDir, int fileDir, chessBoard * cBoard) {

    //QTextStream out(stdout);
    //Create a temporary chessSquare that can act as an iterator. It is one in the direction from the start
    chessSquare * mid = cBoard->boardLocation[start->getRank() + rankDir][start->getFile() + fileDir];

    if (!mid->isEmpty() && mid->getPieceColour() == this->colour) {
        return false;
    }
    //initial values for rank and file boundaries. In the case of straight directions, this is so it is never equal to the boundary.
    int rankBound = start->getRank() + 1;
    int fileBound = start->getFile() + 1;

    //decreasing rank
    if (rankDir == -1) {
        rankBound = 0;
    }

    //decreasing file
    if (fileDir == -1) {
        fileBound = 0;
    }

    //increasing rank
    if (rankDir == 1) {
        rankBound = DIMENSION - 1;
    }

    //increasing file
    if (fileDir == 1) {
        fileBound = DIMENSION - 1;
    }

    //Main loop. mid has to become the destination.
    //If the square contains a piece it must stop.
    //If the boundaries are reached it must stop.
    while (mid->isEmpty() && mid->getFile() != fileBound && mid->getRank() != rankBound && ((*mid).getFile() != dest->getFile() || (*mid).getRank() != dest->getRank())) {
        mid = cBoard->boardLocation[mid->getRank() + rankDir][mid->getFile() + fileDir];
    }
    if ((*mid).getFile() != dest->getFile() || (*mid).getRank() != dest->getRank() || (!(*mid).isEmpty() && mid->getPieceColour() == this->colour)) {
        return false;
    }
    if (this->isKingChecked(start, dest)) {
        return false;
    }
    return true;
}

bool chessPiece::isKingChecked(chessSquare *start, chessSquare *dest) {
    Move * tempMove = new Move(start, dest, this->board);
    tempMove->execute();
    if (King * tempKing = dynamic_cast<King *>(this->board->getGame()->getKingSquare(this->colour)->getPiece())) {
        if (tempKing->isInCheckCurrent(this->board->getGame()->getKingSquare(this->colour))) {
            tempMove->undo();
            return true;
        }
    }
    tempMove->undo();
    return false;
}


Pawn::Pawn(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessPawnBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessPawnBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessPawnWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessPawnWhitewBackground.png");
    }
}



bool Pawn::isValidMove(chessSquare *start, chessSquare *dest) {
    //QTextStream out(stdout);

    if (this->colour == WHITE) {
        //capturing
        if (dest->getFile() != start->getFile()) {
            if (abs(start->getFile() - dest->getFile()) == 1 && start->getRank() - 1 == dest->getRank()) {

                //normal capture
                if (dest->getPieceColour() == this->getOther()) {
                    if (this->isKingChecked(start, dest)) {
                        return false;
                    }
                    return true;
                }
                if (start->getRank() != 3) {
                    return false;
                }
                //TODO: En passant capture
                Move * lastMove = this->board->getLastMove();
                if (lastMove->getStartRank() == 1 && lastMove->getDestRank() == 3 && lastMove->getDestFile() == dest->getFile()) {
                    if (Pawn * newPiece = dynamic_cast<Pawn *>(lastMove->movedPiece())) {
                        if (this->isKingChecked(start, dest)) {
                            return false;
                        }
                       return true;
                    }
                }
            }
            return false;
        }
        //normal forward move
        if (dest->getRank() == start->getRank() - 1 && dest->isEmpty()) {
            if (this->isKingChecked(start, dest)) {
                return false;
            }
            return true;
        }
        //double move at the beginning
        if (dest->getRank() == start->getRank() - 2 && !this->hasMoved) {
            if (this->board->boardLocation[start->getRank() - 1][start->getFile()]->isEmpty() && dest->isEmpty()) {
                if (this->isKingChecked(start, dest)) {
                    return false;
                }
                return true;
            }
        }
        return false;

    } else if (this->colour == BLACK) {

        //capturing
        if (dest->getFile() != start->getFile()) {
            if (abs(start->getFile() - dest->getFile()) == 1 && dest->getRank() == start->getRank() + 1) {

                //normal capture
                if (dest->getPieceColour() == this->getOther()) {
                    if (this->isKingChecked(start, dest)) {
                        return false;
                    }
                   return true;
                }
            if (start->getRank() != 4) {
                return false;
            }
            //TODO: En passant capture
            Move * lastMove = this->board->getLastMove();
            if (lastMove->getStartRank() == 6 && lastMove->getDestRank() == 4 && lastMove->getDestFile() == dest->getFile()) {
                if (Pawn * newPiece = dynamic_cast<Pawn *>(lastMove->movedPiece())) {
                    if (this->isKingChecked(start, dest)) {
                        return false;
                    }
                   return true;
                }
            }
        }
            return false;
        }

        //normal forward move
        if (dest->getRank() == start->getRank() + 1 && dest->isEmpty()) {
            if (this->isKingChecked(start, dest)) {
                return false;
            }
            return true;
        }
        //double move at the beginning
        if (dest->getRank() == start->getRank() + 2 && !this->hasMoved) {

            if (this->board->boardLocation[start->getRank() + 1][start->getFile()]->isEmpty() && dest->isEmpty()) {
                if (this->isKingChecked(start, dest)) {
                    return false;
                }
                return true;
            }
        }
        return false;
    }
    return false;
}

Knight::Knight(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessKnightBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessKnightBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessKnightWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessKnightWhitewBackground.png");
    }
}

bool Knight::isValidMove(chessSquare * start, chessSquare * dest) {

    return chessPiece::isLShape(start, dest);
}


Bishop::Bishop(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessBishopBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessBishopBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessBishopWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessBishopWhitewBackground.png");
    }
}

bool Bishop::isValidMove(chessSquare *start, chessSquare *dest) {
    return chessPiece::isDiagonal(start, dest, this->board);
}

Rook::Rook(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessRookBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessRookBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessRookWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessRookWhitewBackground.png");
    }
}

bool Rook::isValidMove(chessSquare *start, chessSquare *dest) {
    return chessPiece::isStraight(start, dest, this->board);
}

Queen::Queen(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessQueenBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessQueenBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessQueenWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessQueenWhitewBackground.png");
    }
}

bool Queen::isValidMove(chessSquare *start, chessSquare *dest) {
    return chessPiece::isStraight(start, dest, this->board) || chessPiece::isDiagonal(start, dest, this->board);
}

King::King(char colour, chessBoard * board) : chessPiece(colour, board) {
    if (this->colour == BLACK) {
        this->bIcon = new QIcon(":/images/ChessKingBlackbBackground.png");
        this->wIcon = new QIcon(":/images/ChessKingBlackwBackground.png");
    } else {
        this->bIcon = new QIcon(":/images/ChessKingWhitebBackground.png");
        this->wIcon = new QIcon(":/images/ChessKingWhitewBackground.png");
    }
}

//incomplete
bool King::isValidMove(chessSquare *start, chessSquare *dest) {
    if (!dest->isEmpty() && dest->getPieceColour() == this->colour) {
        return false;
    }

    if (abs(start->getFile() - dest->getFile()) <= 1 && abs(start->getRank() - dest->getRank()) <= 1) {
        bool result = true;
        Move * testMove = new Move(start, dest, this->board);
        testMove->execute();
        if (this->isInCheckCurrent(dest)) {
            result = false;
        }
        testMove->undo();
        return result;
    }
    if (this->board->isCastlingMove(start, dest)) {
        return true;
    }
    return false;
}


bool King::isInCheckCurrent(chessSquare * current) {
    //Temporary. Another idea is to iterate through all of the pieces, which should be much faster.
    if (this->colour == WHITE) {
        if (this->board->getGame()->isBChecked()) {
            return false;
        }
        //chessSquare * currSquare = this->board->boardLocation[this->curRank][this->curFile];
        for (chessPiece * piece : this->board->getGame()->getPlayerBlack()->pieces) {
            chessSquare * tempSquare = this->board->boardLocation[piece->getCurRank()][piece->getCurFile()];
            if (piece->isValidMove(tempSquare, current)) {
                return true;
            }
        }
        return false;
    }
    if (this->colour == BLACK) {
        if (this->board->getGame()->isWChecked()) {
            return false;
        }
        //chessSquare * currSquare = this->board->boardLocation[this->curRank][this->curFile];
        for (chessPiece * piece : this->board->getGame()->getPlayerWhite()->pieces) {
            chessSquare * tempSquare = this->board->boardLocation[piece->getCurRank()][piece->getCurFile()];
            if (piece->isValidMove(tempSquare, current)) {
                return true;
            }
        }
        return false;
    }
    return false;
}

