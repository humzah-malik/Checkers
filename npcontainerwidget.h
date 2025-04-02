#ifndef NPCONTAINERWIDGET_H
#define NPCONTAINERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "arrowbutton.h"

class NpContainerWidget : public QWidget {
    Q_OBJECT

private:
    QLabel *nPlayersTitle;
    QLabel *nPlayersLabel;
    ArrowButton *leftButton;
    ArrowButton *rightButton;

    int nPlayers = 1;

    void setupUI();

public:
    explicit NpContainerWidget(QWidget *parent=nullptr);

    int getNPlayers() const { return nPlayers; }

signals:
    void changeNpSignaled(int newNumber);

private slots:
    void changeNPlayers(int direction);
};

#endif