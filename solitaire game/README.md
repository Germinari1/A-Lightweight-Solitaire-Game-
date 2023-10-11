# Boringness-Avoider Comand-Line Solitaire Game
### Video Demo:  https://youtu.be/mPoubwWq9ys
### Description:
This project implements a command line version of the classic solitaire card game. The game allows a single player to play through a full game of solitaire.

#### Overview
This solitaire game is implemented in C++ and runs in a CLI terminal. It utilizes structs and enums concepts to represent a deck of cards, the discard piles, and the foundation piles. The game logic allows drawing cards, playing cards to foundations and discards, and scoring at the end.

The project contains the following files:
1. main.cpp: This contains the main game logic, input/output, all the necessary structures, functions, and runs the game.

In this file, the structs and enums that represent decks and cards promote encapsulation and a more efficient manipulation of the game components, while ASCII characters are used to display the cards in the terminal (giving a graphic feature to the game). Furthermore, The game starts by initializing a full deck of 52 cards and shuffling. The deck is split into the 4 foundation piles, 4 discard piles, and a draw pile. On each turn, the user can choose to draw a card, play a card to foundations or discards, or view the discard piles. Play continues until no moves are left or the deck is depleted. A score is displayed based on cards left over.

Moreover, The logic in the main file is also highly modularized. The different tasks within the game (like setting up and suffling the deck, drawing cards, and processing moves) are distributed into functions that perform well-defined computations and are, due to the mentioned modularization, more individually testable.

It is also valid to mention that, altought the program is command-line based, there are some graphical elements that make the gameplay more visual and engaging. Thera are functions that print a simnplified representation of a card on the terminal by using ASCII characters.

Several design decisions were made and are interesting to note:

1. Used structures for encapsulation and logical organization
2. Deck shuffle uses a simple suffle through a seeded pseudo-random number generator
3. Card graphics rendered using ASCII art for portability and simplicity
4. Input validation on user choices to handle bad input correctly

#### How to Build
This project requires a C++ compiler and standard C++11.

To build the executable:
```txt
g++ main.cpp card.cpp deck.cpp pile.cpp game.cpp utils.cpp -o solitaire
```
Then run with:
```txt
./solitaire
```
Then, the game can be played interactively through the terminal.