# Checkers Game — CS3307 Final Project

A retro-themed Checkers game written in C++ using the Qt framework. It features a slick UI, two-player and AI modes, custom undo logic, and sound effects.

---

## Project Info

- **Course**: CS3307 — Object-Oriented Design and Analysis
- **University**: Western University
- **Group**: 36  
- **Group Members**: Humzah Zahid Malik, Lok Hui, Krish Bhavin Patel

---

## Game Description

Checkers is a classic two-player board game that we’ve reimagined with a modern UI and extra features. Players can compete head-to-head in a 1v1 match or against an AI opponent. The game supports advanced features such as:
- AI with minimax + alpha-beta pruning
- Undo functionality
- Real-time sound effects
- Highlighted legal moves
- Dynamic GUI using Qt Widgets

This project was created as part of the final group assignment for CS3307: Object-Oriented Design and Analysis.

---

## 🛠️ Build & Run Instructions

### Requirements (for all platforms)
- Qt 6+ (Widgets + Multimedia modules)
- C++17-compatible compiler
- Git (for cloning the repo)
- Doxygen (for documentation generation)

---

### macOS

#### Install Qt
bash
brew install qt

#### Build
/opt/homebrew/opt/qt/bin/qmake checkers.pro
make

#### Run
./Checkers.app/Contents/MacOS/Checkers

### Windows
#### Build
qmake checkers.pro
mingw32-make
#### Run
Checkers.exe

### Documentation
To generate the code documentation using Doxygen:
doxygen Doxyfile
The generated HTML docs will be located in the docs/html directory.
