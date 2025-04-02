
#include "npcontainerwidget.h"

NpContainerWidget::NpContainerWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void NpContainerWidget::setupUI() {

    nPlayersTitle = new QLabel("Player", this);
    // nPlayersTitle->setAlignment(Qt::AlignCenter);
    nPlayersTitle->setStyleSheet ("font-size: 64px; text-align: center;");

    leftButton = new ArrowButton("-", this);
    rightButton = new ArrowButton("+", this);

    nPlayersLabel = new QLabel(QString::number(nPlayers), this);
    nPlayersLabel->setStyleSheet("font-size: 32px;");
    nPlayersLabel->setFixedWidth(160);
    nPlayersLabel->setAlignment(Qt::AlignCenter);
    nPlayersLabel->adjustSize();

    QHBoxLayout *nPlayersBox = new QHBoxLayout();

    nPlayersBox->setSpacing(10);
    
    nPlayersBox->addStretch();
    nPlayersBox->addWidget(leftButton);
    // nPlayersBox->addStretch();
    nPlayersBox->addWidget(nPlayersLabel);
    // nPlayersBox->addStretch();
    nPlayersBox->addWidget(rightButton);
    nPlayersBox->addStretch();

    QWidget *nPlayersWidget = new QWidget(this);
    nPlayersWidget->setLayout(nPlayersBox);
    nPlayersWidget->setFixedSize(300, 80);
    
    QVBoxLayout *npContainer = new QVBoxLayout();
    // npContainer->setSpacing(5);
    npContainer->addStretch(1);
    npContainer->addWidget(nPlayersTitle, 0, Qt::AlignCenter);
    npContainer->addWidget(nPlayersWidget, 0, Qt::AlignCenter);
    npContainer->addStretch(1);

    setLayout(npContainer);

    connect(leftButton, &QPushButton::clicked, this, [this]() {changeNPlayers(-1);});
    connect(rightButton, &QPushButton::clicked, this, [this]() {changeNPlayers(1);});
    
}

void NpContainerWidget::changeNPlayers(int direction) {
    int newNumber = qBound(1, nPlayers + direction, 4);
    if (newNumber != nPlayers) {
        nPlayers = newNumber;
        nPlayersLabel->setText(QString::number(nPlayers));
        emit changeNpSignaled(nPlayers);
    }
}