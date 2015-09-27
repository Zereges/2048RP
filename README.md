# 2048 Game --- Client ↔ Server approach
### INTRODUCTION
2048 is a logic game played usually on 4x4 board. The board contains
blocks of various values. In each turn, the player choses the direction
(up, down, left, right) in which all blocks on the board will move.
During the turn, a collision of two blocks can happen. If they are
of same value, they will be merged together to form single block
of doubled value. After the turn, a random spot on the board is chosen
and a new block with value of 2 (rarely 4) will spawn on that spot.
At the beginning, there are two blocks of values 2 (rarely 4).
The player is winner if he manages to combine blocks into a block with
value of 2048. If there is no direction, in which there would be a movement
of at least one block, then the player is declared as a loser.

This is implementation consisting of two parts:
- **Server**: Forming a place to play. Clients are connected to the server and are playing the game with datas on the server. Server also stores all information about players and their statistics such as how many times each player won.
- **Client**: Has to connect to the server in order to play. After being connected to the server, player may play the game regularly.

---
# Server part
### PREREQUISITIES
- **C++11** compiler (one of *G++*, *Clang++* or *VC++* is recommended)
- **MySQL** database

### LIBRARIES
- [**Boost**](http://www.boost.org/) for networking support.
- [**MySQL Connector/C++**](https://dev.mysql.com/downloads/connector/cpp/) for database access.

### DOWNLOAD
Source code can be obtained by cloning this repository or by downloading *tar.gz* or *zip* archive from [releases](https://github.com/Zereges/2048RP/releases). For building, see **Build** section.<br>  
Release for Windows can be downloaded from the same link.<br>  
Release for Linux can not be downloaded.

### BUILD
##### Windows
**VisualStudio**: Open `2048RP.sln` in VS, right click *2048Server* in *Solution Explorer* and click *Build*.<br>  
For other IDEs or compilators, try to use included `Makefile`.

#### Linux
**G++**: Invoke *make* from `2048RP` directory by calling `make`.<br>  
For other IDEs or compilator, edit `Makefile` properly.

### INSTALATION
First requirement after build is to create/edit `2048.conf` file in folder, where `2048Server.exe` is located and add configuration like this

    host = host, where to connect to database
    user = database user
    pass = database password
    db = 2048 database
    
Another requirement is to create database with name according to what is configured in conf file loaded by the app. Then, execute `2048Server/sql/2048.sql` dump into that database.

In order to allow user registration, you can provide access to `2048Server/web/` where is simple registration form and stats form. You need to edit `2048Server/web/config.php` accordingly.

### RUNNING THE PROGRAM
Server can be run by executing `2048Server.exe [conf_file]` or `./server [conf_file]`<br>  
Where `conf_file` is path to configuration file (defaulted to `2048.conf`).

Program runs by itself, no additional actions are required. If program outputs no error messages, the server should be running.

---
# Client part
### PREREQUISITIES
- At least Windows 7 system
- Visual Studio

### LIBRARIES
- [**Boost**](http://www.boost.org/) for networking support.
- [**SDL2**](https://www.libsdl.org/) for graphical output.
- [**SDL_TTF**](https://www.libsdl.org/projects/SDL_ttf/) for writing text in app.

### DOWNLOAD
Source code can be obtained by cloning this repository or by downloading *tar.gz* or *zip* archive from [releases](https://github.com/Zereges/2048RP/releases). For building, see **Build** section.<br>  
Release can be downloaded from the same link.<br>  

### BUILD
##### Windows
**VisualStudio**: Open `2048RP.sln` in VS, right click *2048Client* in *Solution Explorer* and click *Build*.

### INSTALATION
There is no need to perform any additional steps in order to get this application running.

### RUNNING THE PROGRAM
Client can be run by executing `2048Client.exe [host [port]]`<br>  
Where `host` is host where the server is running (defaults to server.ekirei.cz which may be online when requested).<br>  
Where `port` is port on which server is listening (defaults to 8881)

### PROGRAM CONTROL
After program startup, you will be prompted to enter username and password according to what is saved on server (you probably registred using registration form provided by server.<br>  
For server.ekirei.cz, the address is http://server.ekirei.cz/2048/.

After entering your correct username and password, you will be authenticated by the server and game window will open for you.
In the top left corner of the game window, there is an indicator showing current score. If there is a "W" symbol after numeric value of the score, it means, that the player managed to win this game and is only hunting higher score. In the top right corner, one can click "Show Stats" button, which will pop stats window showing interesting statistics about the play, such as Total Moves or Highest Score. The statistics are preserved during multiple runs of the program (saved in Stats.dat file). User can click "Switch to Global/Current Stats" in the stats window to see statstics regarding current game, or global statistics of all previous playthroughs.

The game is controlled using keyboard. By pressing directional arrows, one can perform turn in given direction. Pressing "R" button restarts current game progress. You can close the game by clicking top right cross, pressing Alt+F4 or Ctrl+Q.

Stats window is not yet implemented, but one can look at their, or someone others stat on registration url by entering desired name into. They are refreshed once given player quits the application.

---
### DOCUMENTATION
Programmer's documentation can be generated using [Doxygen](http://www.stack.nl/~dimitri/doxygen/) with given `Doxyfile`. Resulting documentation will be in `./doc` folder. It is also available online on [this link](http://www.zereges.cz/2048RP/doc/).
