
# Advanced programming Lab 11

- Compulsory, all tasks
- Optional, all tasks
- Bonus, all tasks

## Spring server project packages:

- config: contains classes adnotated with @Configuration
- controllers: contains server controllers
- dtos: contains the DTOs used by the controllers
- exceptions: -
- middlewares: contains JWT related middlewares
- models: contains the entities that have to be persisted
- repositories: contains the repositories
- services: contains the server services
- utils: contains JWT related class and static methods used in differents parts of the application

## Server project packages:

- cmd: contains the classes for the server shell
- dto: contains DTOs used to communicate with the Spring server
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

### - How the Spring server was integrated with the application created in the previous week:

The game server sends HTTPS requests to the Spring server (so the Spring server offers services to the game server, they are not highly coupled).

## Demo: https://youtu.be/1YKTGuXywYo

(9:44 - 10:44 can be skipped)
