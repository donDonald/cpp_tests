
# Task
Create a local multiplayer chess game in QT.\
There should be 3 screens.

### Screen_1
Once the application starts the Screen_1 appears.\
There should be at least the following elements on the screen: 
* Start button – starts a new game. Leads to the Screen_2
* Load button – allows user to load saved game. Leads to the Screen_3
* Empty chess board

### Screen_2
Chess pieces are on the board (in initial position).\
User should be able to move the pieces with mouse according to the standard chess rules.\
Turn by turn rule must be followed as well (white turn-black turn- white-…).\
It’s not mandatory to implement the following: castling, check, checkmate.

The following buttons should be accessible:
* Stop – application stops the game and leads to the Screen_1
* Save – saves the game history to a file

### Screen_3
Loads a game history and allows to play it step by step using two buttons: “prev” and “next”.\
The following items are available on the screen:
* Start button – starts a new game. Leads to the Screen_2
* Load button – allows user to load saved game. Leads to the Screen_3
* Buttons “prev” and “next”
* chess board with pieces

# Evaluation criteria:
We will not care on graphics quality and comments in the code.\
But it is important that your application is stable, without any crashes and memory leaks.\
Also we will look into the application architecture, classes hierarchy, exceptions handling, coding practices.\
Build system: qmake\
Technologies:
* QT+QML is mandatory
* C++11
* boost – up to you

Platform: Windows or Linux



# Solution
1. Was tested on top of Ububntu 14.04 LTS
2. Qt: 
   QMake version 2.01a
   Using Qt version 4.8.6 in /usr/lib/i386-linux-gnu
3. There are 2 projects:
   Model/tsrc/tsrc.pro - Model unit tests
   Ui/Ui.pro - the complete app
