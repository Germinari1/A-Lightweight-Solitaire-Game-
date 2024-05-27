## A Lightweight Solitaire Game

This is a command-line interface (CLI) implementation of the classic Solitaire card game, written in C++. The game follows the standard Solitaire rules and allows players to draw cards, move cards between the foundation piles and waste piles, and ultimately try to complete the game by stacking all cards in the correct order on the foundation piles. With its simplicity, this version of the game can run in pretty much any device that supports a c++ compiler and basic CLI functionalities.

## Gameplay

The game starts by setting up the deck of cards, shuffling it, and placing one card on each of the four foundation piles. The remaining cards are then available for drawing. The game loop continues until the player either wins by successfully moving all cards to the foundation piles or chooses to quit.

During each turn, the player is presented with a menu of available actions:

1. Draw a card (if no card has been drawn and the deck is not empty)
2. Play the drawn card (if a card has been drawn)
3. Play a card from a waste pile onto a foundation pile
4. Display the cards in a specific waste pile

The player can also choose to quit the game at any time by entering -1.

## How to Run

To run the game, follow these steps:

1. Open a terminal or command prompt.
2. Navigate to the directory containing the `solitaire.cpp` file.
3. Compile the C++ source code using a C++ compiler.
4. Run the compiled executable.

For example, if you're using the GCC compiler on a Unix-based system, you can compile and run the program with the following commands:
```txt
g++ solitaire.cpp -o solitaire
./solitaire
```
## Score Calculation

At the end of the game, the player's score is calculated based on the number of cards remaining in the waste piles and the deck. The goal is to achieve a score of 0, which means all cards have been moved to the foundation piles, resulting in a win.

## Dependencies

This program has no external dependencies and only uses the standard C++ libraries.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.
