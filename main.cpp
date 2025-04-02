/**
 * @file main.cpp
 * @brief Entry point for launching the Checkers game application.
 *
 * Initializes the Qt application, loads the custom font, and shows the main window.
 */

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include "mainwindow.h"
// #include "checkersmenu.h"
// #include "checkersmanager.h"

int main(int argc, char *argv[])
{
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Load custom font (Russo One)
    QFontDatabase::addApplicationFont(":/Fonts/RussoOne-Regular.ttf");

    // Create and display the main game window
    MainWindow window;
    window.show();

    qDebug() << "Game window opened.";

    // Optional legacy code for using CheckersMenu or CheckersManager
    /*
    CheckersMenu *menu = new CheckersMenu();
    menu->setWindowTitle("Checkers Game");
    menu->resize(1200, 800);
    menu->show();

    CheckersManager *manager = new CheckersManager();
    QObject::connect(menu, &CheckersMenu::startGameSignaled, manager, &CheckersManager::onStart);
    */

    // Run the Qt event loop
    return app.exec();
}