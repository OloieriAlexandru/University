
# Advanced programming Lab 10

- Compulsory, all tasks
- Optional, all tasks
- Bonus, all tasks (for the last task I chose to create a GUI for the game)

## Server project packages:

- cmd: contains the classes for the server shell
- game: contains the models of the game (Game, Board)
- rmi: contains the RMI interface and RMI interface implementation
- server: contains the most important and complex classes of the App
- util: contains static methods used in differents parts of the application

## Client project packages

- controller: contains the controller class, which manages the communication with the server
- model: contains the board on which the game is played
- rmi: contains the RMI interface
- util: contains static methods used in differents parts of the application
- view: all the classes used for creating the Swing user interface

## Details about the application:

### MVC

The client app was built using the MVC design pattern. For creating the graphical user interface I used Java Swing.

### Multiple game rooms

The players can create game rooms and join them. A game room will be managed by a separate thread, which will mediate the players (the threads associated with the clients will sleep while the game is running).

### HTML Game Representation

After the game ends, a HTML file will be sent to a web server using jcraft.JSch class

## Demo: https://youtu.be/lfpODWL6Jc4
