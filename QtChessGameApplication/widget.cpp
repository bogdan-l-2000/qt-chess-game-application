#include "widget.h"
#include "ui_widget.h"

#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    //layout->setAlignment()
    this->board = new chessBoard();
    this->game = new chessGame(board);
    board->setGame(game);
    layout->addWidget(board);
    //board->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout * infoBox = new QHBoxLayout();
    layout->addLayout(infoBox);

    QVBoxLayout * turnBox = new QVBoxLayout();
    infoBox->addLayout(turnBox);

    QLabel * turnLabel = new QLabel("Turn: ");
    turnBox->addWidget(turnLabel);

    this->whosTurnLabel = new QLabel("White"); //white
    turnBox->addWidget(whosTurnLabel);
    connect(this->game, SIGNAL(turnChanged()), this, SLOT(turnLabel()));


    QVBoxLayout * promoteBox = new QVBoxLayout();
    infoBox->addLayout(promoteBox);

    QVBoxLayout * undoBox = new QVBoxLayout();
    infoBox->addLayout(undoBox);
    QPushButton * undoButton = new QPushButton("Undo");
    infoBox->addWidget(undoButton);
    connect(undoButton, &QPushButton::clicked, this->game, &chessGame::undoMove);

}

void Widget::turnLabel() {
    if (this->game->getWhosTurn() == WHITE) {
        this->whosTurnLabel->setText("White"); //white
    } else if (this->game->getWhosTurn() == BLACK) {
        this->whosTurnLabel->setText("Black"); //black
    }

}

Widget::~Widget()
{
    delete ui;
}


