# ♟️ Checkers Game — CS3307 Final Project

A retro-themed Checkers game written in C++ using the Qt framework. Features a slick UI, two-player and AI modes, custom undo logic, and sound effects.

## Build & Run Instructions

### Requirements (Both Platforms)
- Qt 6+ (Widgets + Multimedia modules)
- C++17-compatible compiler
- Git (optional, for cloning)
- Doxygen (for documentation generation)

#### Install Qt
```bash
brew install qt

$ qmake checkers.pro

$ make

$
### macOS

#### Build
/opt/homebrew/opt/qt/bin/qmake checkers.pro
make

or

qmake checkers.pro
make

#### Run
./Checkers.app/Contents/MacOS/Checkers 
or
./Checkers

### Windows

#### Build
qmake checkers.pro
mingw32-make       # or `nmake` if using MSVC

#### Run
Checkers.exe