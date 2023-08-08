#ifndef WIDGET_H
#define WIDGET_H

#include "moveTypes.h"
#include "chessGame.h"
#include "chessBoard.h"
#include "chessPiece.h"

#include <QLabel>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:

    void turnLabel();

private:
    Ui::Widget *ui;
    chessGame * game;
    chessBoard * board;
    QLabel * whosTurnLabel;
};
#endif // WIDGET_H
