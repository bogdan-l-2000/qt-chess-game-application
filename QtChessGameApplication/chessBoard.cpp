#include "chessBoard.h"
#include "moveTypes.h"
#include <string.h>
#include <QTextStream>
#include <QPixmap>
#include <QFile>
#include <QSize>

/*
 * connect all buttons to first function: shows all the available moves.
 *
 * in the first function: disconnect the next button pressed and connect it to second function, which executes a move if available.
 *
 */


chessSquare::chessSquare(QWidget * parent) : QPushButton(parent) {
    this->piece = NULL;
    this->file = static_cast<int>(NULL);
    this->rank = static_cast<int>(NULL);
    this->colour = static_cast<char>(NULL);
}

int chessSquare::getFile() {
    return this->file;
}


int chessSquare::getRank() {
    return this->rank;

}

char chessSquare::getColour() {
    return this->colour;
}
//window.setWindowIcon(QIcon("web.png"));

void chessSquare::setFile(int file) {
    this->file = file;
}

void chessSquare::setRank(int rank) {
    this->rank = rank;
}

bool chessSquare::operator==(const chessSquare & b) {
    return (this->file == b.file) && (this->rank == b.rank);
}

bool chessSquare::operator!=(const chessSquare & b) {
    return (this->file != b.file) || (this->rank != b.rank);
}

chessPiece * chessSquare::getPiece() {
    return this->piece;
}

void chessSquare::setColour(char colour) {
    this->colour = colour;
    if (colour == WHITE) {
        this->setStyleSheet("background: white");
    } else {
        this->setStyleSheet("background: black");
    }
}

void chessSquare::loadPiece(chessPiece *piece) {
    this->piece = piece;
    if (this->colour == WHITE) {
        this->setIcon(piece->getWIcon());
        QSize imgSize(80, 80);
        this->setIconSize(imgSize);
    }
    else {
        this->setIcon(piece->getBIcon());
        QSize imgSize(80, 80);
        this->setIconSize(imgSize);
    }
}

chessPiece * chessSquare::removePiece() {
    this->setIcon(QIcon());
    chessPiece * tempPiece = this->piece;
    this->piece = NULL;
    return tempPiece;
}

bool chessSquare::isEmpty() {
    return (this->piece == NULL);
}

char chessSquare::getPieceColour() {
    if (this->isEmpty()) {
        return NONE;
    }
    return (this->piece)->getColour();
}

//Create chess board, loaded with the new pieces.
chessBoard::chessBoard(QWidget * parent) : QWidget(parent) {

    QGridLayout * board = new QGridLayout(this);
    board->setSpacing(0);
    board->setMargin(0);
    board->setVerticalSpacing(0);
    board->setHorizontalSpacing(0);
    board->setContentsMargins(0, 0, 0, 0);
    this->current = NULL;

    //this->board = new chessSquare[DIMENSION][DIMENSION];

    char curColour = WHITE;
    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {

            chessSquare * temp = new chessSquare();

//            temp->setFixedWidth(80);
//            temp->setFixedHeight(80);
//            temp->set


            temp->setRank(i);
            temp->setFile(j);
            temp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            board->addWidget(temp, i, j);
            boardLocation[i][j] = temp;
            signalMapper->setMapping(temp, temp);
            connect(temp, SIGNAL(clicked()), signalMapper, SLOT(map()));
            if (curColour == WHITE) {
                temp->setColour(WHITE);
                if (j < DIMENSION - 1) {
                    curColour = BLACK;
                }
            } else {
                temp->setColour(BLACK);
                if (j < DIMENSION - 1) {
                    curColour = WHITE;
                }
            }
            //Load black pieces
            chessPiece * tempPiece = NULL;
            if (i == 0) {
                if (j == 3) {
                    tempPiece = new Queen(BLACK, this);
                }
                if (j == 4) {
                    tempPiece = new King(BLACK, this);
                }
                if (j == 0 || j == 7) {
                    tempPiece = new Rook(BLACK, this);
                }
                if (j == 1 || j == 6) {
                    tempPiece = new Knight(BLACK, this);
                }
                if (j == 2 || j == 5) {
                    tempPiece = new Bishop(BLACK, this);
                }
            }
            //Load pawns
            if (i == 1) {
                tempPiece = new Pawn(BLACK, this);
            }
            if (i == 6) {
                tempPiece = new Pawn(WHITE, this);
            }
            //load white pieces
            if (i == 7) {

                if (j == 3) {
                    tempPiece = new Queen(WHITE, this);
                }
                if (j == 4) {
                    tempPiece = new King(WHITE, this);
                }
                if (j == 2 || j == 5) {
                    tempPiece = new Bishop(WHITE, this);
                }
                if (j == 1 || j == 6) {
                    tempPiece = new Knight(WHITE, this);
                }
                if (j == 0 || j == 7) {
                    tempPiece = new Rook(WHITE, this);
                }
            }
            if (tempPiece != NULL) {
                temp->loadPiece(tempPiece);
                tempPiece->setCurFile(j);
                tempPiece->setCurRank(i);
            }
        }
    }
    connect(signalMapper, SIGNAL(mapped(QWidget *)), this, SLOT(showMoves(QWidget *)));

}

//Set a game object to the board
void chessBoard::setGame(chessGame *game) {
    this->game = game;
}

chessGame * chessBoard::getGame() {
    return this->game;
}


//return whether a button is selected, and the game is in the process of a move
bool chessBoard::isbuttonSelected() {
    return this->buttonSelected;
}


chessSquare * chessBoard::getCurrent() {
    return this->current;
}

bool chessBoard::isSideChecked(char colour) {
    if (colour == WHITE) {
        return this->isWCheck;
    } else if (colour == BLACK) {
        return this->isBCheck;
    }
    return false;
}

//function for processing a button click. Different functionality on whether there has been another button selected.
//First button click shows moves, second button click makes the move if needed, or cancels the move.
void chessBoard::showMoves(QWidget * wid) {
    //current square selected
    chessSquare * currSquare = dynamic_cast<chessSquare *>(wid);
    chessPiece * currPiece = currSquare->getPiece();
    //Variable 'current' is the previous square selected

    QTextStream out(stdout);

    //No button selected, show the valid moves and set current button to this
    if (!buttonSelected) {
        buttonSelected = true;
        //Set border for selected square as it is the highlighted one
        if (currSquare->getColour() == WHITE) {
        currSquare->setStyleSheet("background: white; border:2px solid #2557ed");
        } else {
            currSquare->setStyleSheet("background: black; border:2px solid #2557ed");
        }
        //Show valid moves and highlight thme
        if (currPiece != NULL && currPiece->getColour() == (this->game)->getWhosTurn()) {
            current = currSquare;
            for (int i = 0; i < DIMENSION; i++) {
                for (int j = 0; j < DIMENSION; j++) {
                    if (this->boardLocation[i][j]->getPieceColour() == currPiece->getColour()) {
                        continue;
                    }
                    if (currPiece->isValidMove(currSquare, this->boardLocation[i][j])) {
                        this->boardLocation[i][j]->setStyleSheet("background: blue");
                    }
                }
            }
        } else {
            resetView(currSquare);
        }
    }
    else {
        //Valid move: into an empty spot or capturing a piece
        if ((currPiece == NULL || currPiece->getColour() == current->getPiece()->getOther()) && (current->getPiece())->isValidMove(current, currSquare)) {
            //En Passant move
            if (this->isEnPassantMove(current, currSquare)) {
                Move * enPMove = new EnPassantMove(current, currSquare, this, this->game);
                this->game->makeMove(enPMove);

            }
            //Promotion Move
            else if (this->isPromotionMove(current, currSquare)) {
                Move * ProMove = new PromotionMove(current, currSquare, this);
                this->game->makeMove(ProMove);
            }
            //Castling Move
            else if (this->isCastlingMove(current, currSquare)) {
                Move * CastMove = new CastlingMove(current, currSquare, this);
                this->game->makeMove(CastMove);
            }
            //Regular Move
            else {
                Move * move = new Move(current, currSquare, this);
                this->game->makeMove(move);

            }
            if (King * tempKing = dynamic_cast<King *>(currSquare->getPiece())) {
                this->game->setKingSquare(tempKing->getColour(), currSquare);
            }

            if (this->game->isChecked(currSquare->getPiece()->getOther())) {
                out << "Check!" << endl;
            }
            resetView(currSquare);
        } else {
            resetView(currSquare);
        }
    }

}

//Reset view from the highlighted buttons
void chessBoard::resetView(chessSquare * currSquare) {

    buttonSelected = false;
    if (currSquare->getColour() == WHITE) {
        currSquare->setStyleSheet("background: white");
    } else {
        currSquare->setStyleSheet("background: black");

    }
    if (current != NULL) {
        if (current->getColour() == WHITE) {
            current->setStyleSheet("background: white");
        } else {
            current->setStyleSheet("background: black");
    }
    }
    current = NULL;

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (this->boardLocation[i][j]->getColour() == WHITE) {
                this->boardLocation[i][j]->setStyleSheet("background: white");
            } else {
                this->boardLocation[i][j]->setStyleSheet("background: black");
            }
        }
    }
}

//return the last move of the game
Move * chessBoard::getLastMove() {
    return this->game->getLastMove();
}

//undo a move
void chessBoard::undoMove() {
    this->game->undoMove();
}

//returns whether a move is an En Passant move as it is treated differently from a regular move
bool chessBoard::isEnPassantMove(chessSquare * start, chessSquare * dest) {
    if (start->getPieceColour() == WHITE && start->getRank() == 3 && abs(dest->getFile() - start->getFile()) == 1) {
        if (this->game->getLastMove()->getDestFile() == dest->getFile() && this->getLastMove()->getDestRank() == 3) {
            if (Pawn * capturingPawn = dynamic_cast<Pawn *>(start->getPiece())) {
                    if (Pawn * capturedPawn = dynamic_cast<Pawn *>(getLastMove()->movedPiece())) {
                        return true;
                    }
                }
            }
        return false;
    } else if (start->getPieceColour() == BLACK && start->getRank() == 4 && abs(dest->getFile() - start->getFile()) == 1) {
        if (this->game->getLastMove()->getDestFile() == dest->getFile() && this->getLastMove()->getDestRank() == 4) {
            if (Pawn * capturingPawn = dynamic_cast<Pawn *>(start->getPiece())) {
                    if (Pawn * capturedPawn = dynamic_cast<Pawn *>(getLastMove()->movedPiece())) {
                        return true;
                    }
                }
            }
    }
    return false;
}

bool chessBoard::isPromotionMove(chessSquare *start, chessSquare *dest) {
    if (start->getPieceColour() == WHITE && start->getRank() == 1 && dest->getRank() == 0 && (abs(dest->getFile() - start->getFile()) <= 1)) {
        if (Pawn * promotingPawn = dynamic_cast<Pawn *>(start->getPiece())) {
            return true;
        }
        return false;
    } else if (start->getPieceColour() == BLACK && start->getRank() == 6 && dest->getRank() == 7 && (abs(dest->getFile() - start->getFile()) <= 1)){
        if (Pawn * promotingPawn = dynamic_cast<Pawn *>(start->getPiece())) {
            return true;
        }
        return false;
    }
    return false;
}

bool chessBoard::isCastlingMove(chessSquare * start, chessSquare * dest) {

    //Current is original position of the King
    if (start->getFile() == 4) {
        if (King * wKing = dynamic_cast<King *>(start->getPiece())) {

            //King moved already
            if (wKing->didMove()) {
                return false;
            }
            //Wrong square to select castling
            if (abs(dest->getFile() - start->getFile()) != 2 || dest->getRank() != start->getRank()) {
                return false;
            }

            if (wKing->isInCheckCurrent(dest)) {
                return false;
            }
            if (dest->getFile() > start->getFile()) {
                if (Rook * rRook = dynamic_cast<Rook *>(this->boardLocation[start->getRank()][7]->getPiece())) {
                    if (rRook->didMove()) {
                        return false;
                    }
                    if (this->boardLocation[start->getRank()][5]->isEmpty() && this->boardLocation[start->getRank()][6]->isEmpty()) {
                        return true;
                    }
                }
                return false;
            }
            else if (dest->getFile() < start->getFile()) {
                if (Rook * rRook = dynamic_cast<Rook *>(this->boardLocation[start->getRank()][0]->getPiece())) {
                    if (rRook->didMove()) {
                        return false;
                    }
                    if (this->boardLocation[start->getRank()][1]->isEmpty() && this->boardLocation[start->getRank()][2]->isEmpty() && this->boardLocation[start->getRank()][3]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
