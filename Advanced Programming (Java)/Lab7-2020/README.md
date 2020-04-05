
# Advanced programming Lab 7

- Compulsory, all tasks
- Optional, all tasks
- Bonus, the Clique Game

## Project packages:

- common: contains static methods used in differents parts of the application (it's created in order to avoid duplicate code)
- game: contains the timekeeper, synchronizer classes, and also the abstract classes Game and Player (each type of game then overrides the methods of those classes)
- game.arithmetic: contains all the logic for the Arithmetic Progression Game
- game.clique: contains all the logic for the Clique game
- shell: contains the logic for the main shell of the program

## Details about the application:

### The synchronization of the players: 

The "Synchronizer" class has a list of n objects (where n = the number of players"), and each player has a reference to exactly one of those objects. When the game starts each player calls the "wait()" method of his object and then, during the game, the synchronizer notifies, in order, one player that his turn has come. The player executes his specific logic (random/smart/manual) and then waits for his next turn.
                                  
## Demo: https://youtu.be/ByrvoheX2yQ
