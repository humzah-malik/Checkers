QT += core widgets
QT += core gui widgets
QT += multimedia

CONFIG += c++17

TEMPLATE = app
TARGET = Checkers

RESOURCES += resources.qrc

SOURCES += \
    main.cpp \
    checkersmenu.cpp\
    npcontainerwidget.cpp\
    arrowbutton.cpp\
    checkersboard.cpp\
    piece.cpp\
    checkersmanager.cpp\
    AI.cpp\
    MiniMaxAlgo.cpp\
    Player.cpp\
    mainwindow.cpp\
    gamepage.cpp\
    frontpage.cpp\
    pvppage.cpp\
    pvaipage.cpp\
    instructionspage.cpp\
    settingsdialog.cpp\
    gamedescriptionpage.cpp

HEADERS += \
    checkersmenu.h\
    npcontainerwidget.h\
    arrowbutton.h\
    checkersboard.h\
    piece.h\
    checkersmanager.h\
    AI.h\
    MiniMaxAlgo.h\
    Player.h\
    mainwindow.h\
    gamepage.h\
    frontpage.h\
    pvppage.h\
    pvaipage.h\
    instructionspage.h\
    settingsdialog.h\
    gamedescriptionpage.h\
    stylehelpers.h