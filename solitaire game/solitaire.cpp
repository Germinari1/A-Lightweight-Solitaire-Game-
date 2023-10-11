//////////////////////////////
/*
 Program: CLI-based solitaire | solitaire.cpp
 version: 1.4; 10/11/2023
*/
//////////////////////////////

//include HEADER FILES
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <sstream>
#include <ctime>
#include <limits>
#include <cctype>

//GLOBAL DECLARATIONS
//define enums and structs used to represent cards, decks, and piles, a well as global named constants
enum suitType
{
    DIAMOND,
    CLUB,
    HEART,
    SPADE,
    DEFAULTS
};
enum rankType
{
    A,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    J,
    Q,
    K,
    DEFAULTR
};

const rankType ranks[] = {A,
                          TWO,
                          THREE,
                          FOUR,
                          FIVE,
                          SIX,
                          SEVEN,
                          EIGHT,
                          NINE,
                          TEN,
                          J,
                          Q,
                          K,
                          DEFAULTR};
const suitType suits[] = {DIAMOND,
                          CLUB,
                          HEART,
                          SPADE,
                          DEFAULTS};
const std::string rankStr[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", " "};
const std::string suitStr[] = {"♦", "♣", "♥", "♠", " "};
const int DECK_SIZE = 52;
const int NUM_PILES = 4;

struct card
{
    suitType s = DEFAULTS;
    rankType r = DEFAULTR;
};

struct deck
{
    card d[DECK_SIZE];
    bool used[DECK_SIZE];
    int next = -1;
    int remain = DECK_SIZE;
};

struct pile
{
    card p[DECK_SIZE];
    int topCard = -1;
};

//function PROTOTYPES
std::string printCard(card);
std::string printASCIICard(card theCard);
void setUpDeck(deck &, pile[]);
void shuffleDeck(deck &);
void setUpFoundations(deck &, pile[]);
std::string printASCIICards(pile[]);
void resetStream();
int displayMenu(bool, bool, bool);
void processChoice(bool &, bool, int, deck &, pile[], pile[], bool &);
void drawCard(deck &);
void playDrawnCard(deck &, pile[], pile[], bool &);
void playWaste(pile[], pile[]);
void displayWaste(const pile &);
void playOnFoundation(deck &, pile &, bool &);
void playOnWaste(deck &, pile &);
int calculateScore(deck &, pile[]);

//MAIN LOGIC/ENTRYPOINT OF THE PROGRAM
int main()
{
    //seed pseudorandom number generator
    srand(time(0));
    //ensure that program will run on windows operating system
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    //local declarations
    deck theDeck;
    pile foundations[NUM_PILES];
    pile wastes[NUM_PILES];
    setUpDeck(theDeck, foundations);
    shuffleDeck(theDeck);
    setUpFoundations(theDeck, foundations);
    bool finished = false;
    bool drawnCard = false;
    int choice;

    //main logic of the game
    while (!finished)
    {
        std::cout << "Foundation Piles:" << std::endl;
        std::cout << printASCIICards(foundations);
        std::cout << std::endl;
        std::cout << "Waste Piles:" << std::endl;
        std::cout << printASCIICards(wastes);
        std::cout << std::endl;
        if (drawnCard)
        {
            std::cout << "Drawn Card: " << std::endl
                      << printASCIICard(theDeck.d[theDeck.next]) << std::endl;
        }
        else
        {
            std::cout << "Remaining Cards (" << theDeck.remain << ")" << std::endl;
            card c;
            std::cout << printASCIICard(c);
        }
        bool isWaste, isFin;
        isWaste = false;
        for (int i = 0; i < NUM_PILES; i++)
        {
            if (wastes[i].topCard > -1)
            {
                isWaste = true;
            }
        }
        isFin = theDeck.remain <= 0;
        choice = displayMenu(drawnCard, isWaste, isFin);
        if (choice == -1)
        {
            finished = true;
            continue;
        }
        processChoice(drawnCard, isFin, choice, theDeck, foundations, wastes, finished);

        // finished = true;
    }

    int score = calculateScore(theDeck, wastes);
    if (score == 0)
    {
        std::cout << "You won! You scored 0 points." << std::endl;
    }
    else
    {
        std::cout << "Game Over! You scored " << score << " points." << std::endl;
    }

    return 0;
}

//function DEFINITIONS
std::string printCard(card theCard)
{
    /*
        Prints one card in the game
    */
    std::string output;
    output = rankStr[theCard.r] + suitStr[theCard.s];
    return output;
}

std::string printASCIICard(card theCard)
{
    /*
        Prints a card to the terminal using ASCII characters
    */
    std::ostringstream outStrStream;
    if (theCard.r != DEFAULTR && theCard.s != DEFAULTS)
    {
        outStrStream << "┌─────────┐ " << std::endl;
        outStrStream << std::setfill(' ') << std::left;
        outStrStream << "│ " << std::setw(8);
        outStrStream << rankStr[theCard.r];
        outStrStream << "│ " << std::endl;
        outStrStream << std::setfill(' ') << std::left;
        outStrStream << "│" << std::setw(9) << " "
                     << "│ " << std::endl;
        outStrStream << std::setfill(' ') << std::right;
        outStrStream << "│" << std::setw(7);
        outStrStream << suitStr[theCard.s];
        outStrStream << std::setw(4) << " "
                     << "│ " << std::endl;
        outStrStream << std::setfill(' ') << std::left;
        outStrStream << "│" << std::setw(9) << " "
                     << "│ " << std::endl;
        outStrStream << std::setfill(' ') << std::right;
        outStrStream << "│" << std::setw(8);
        outStrStream << rankStr[theCard.r];
        outStrStream << " │ " << std::endl;
        outStrStream << "└─────────┘ " << std::endl;
    }
    else
    {
        outStrStream << "┌─────────┐" << std::endl;
        outStrStream << "│░░░░░░░░░│" << std::endl;
        outStrStream << "│░░░░░░░░░│" << std::endl;
        outStrStream << "│░░░░░░░░░│" << std::endl;
        outStrStream << "│░░░░░░░░░│" << std::endl;
        outStrStream << "│░░░░░░░░░│" << std::endl;
        outStrStream << "└─────────┘" << std::endl;
    }
    return outStrStream.str();
}

void setUpDeck(deck &theDeck, pile foundations[])
{
    /*
        Fill the deck with cards
    */
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            theDeck.d[i * 13 + j].s = suits[i];
            theDeck.d[i * 13 + j].r = ranks[j];
            theDeck.used[i * 13 + j] = false;
            foundations[i].p[j].r = ranks[((i + 1) * (j + 1) - 1) % 13];
        }
        foundations[i].topCard = -1;
    }
}

void shuffleDeck(deck &theDeck)
{
    /*
        Suffles the deck using a pseudorandom number generator
    */
    card temp[DECK_SIZE];
    for (int i = 0; i < DECK_SIZE; i++)
    {
        int x;
        do
        {
            x = rand() % DECK_SIZE;
        } while (theDeck.used[x]);
        temp[i] = theDeck.d[x];
        theDeck.used[x] = true;
    }
    for (int i = 0; i < 52; i++)
    {
        theDeck.d[i] = temp[i];
        theDeck.used[i] = false;
    }
}

void setUpFoundations(deck &theDeck, pile foundations[])
{
    /*
        Sets up the foundation pile based on the current deck
    */
    for (int i = 0; i < NUM_PILES; i++)
    {
        for (int j = 0; j < DECK_SIZE; j++)
        {
            if (theDeck.d[j].r == foundations[i].p[0].r)
            {
                foundations[i].p[0] = theDeck.d[j];
                theDeck.used[j] = true;
                break;
            }
        }
        foundations[i].topCard++;
        theDeck.remain--;
    }
}

std::string printASCIICards(pile p[])
{
    std::ostringstream out;
    // ┌─────────┐
    for (int i = 0; i < 4; i++)
    {
        out << "  ┌─────────┐ ";
    }
    out << std::endl
        << " ";
    for (int i = 0; i < 4; i++)
    {
        out << std::setfill(' ') << std::left;
        out << " │ " << std::setw(8);
        if (p[i].topCard > -1)
        {
            out << rankStr[p[i].p[p[i].topCard].r];
        }
        else
        {
            out << " ";
        }
        out << "│  ";
    }
    out << std::endl;
    for (int i = 0; i < 4; i++)
    {
        out << std::setfill(' ') << std::left;
        out << "  │" << std::setw(9) << " "
            << "│ ";
    }
    out << std::endl;

    for (int i = 0; i < 4; i++)
    {
        out << std::setfill(' ') << std::right;
        out << "  │";
        if (p[i].topCard > -1)
        {
            out << std::setw(7);
            out << suitStr[p[i].p[p[i].topCard].s];
        }
        else
        {
            out << std::setw(5);
            out << " ";
        }
        out << std::setw(4) << " "
            << "│ ";
    }
    out << std::endl;
    for (int i = 0; i < 4; i++)
    {
        out << std::setfill(' ') << std::left;
        out << "  │" << std::setw(9) << " "
            << "│ ";
    }
    out << std::endl;
    for (int i = 0; i < 4; i++)
    {
        out << std::setfill(' ') << std::right;
        out << "  │" << std::setw(8);
        if (p[i].topCard > -1)
        {
            out << rankStr[p[i].p[p[i].topCard].r];
        }
        else
        {
            out << " ";
        }
        out << " │ ";
    }
    out << std::endl;

    for (int i = 0; i < 4; i++)
    {
        out << i + 1 << " └─────────┘ ";
        // out << setw(6) << "'" << "'  ";
    }
    out << std::endl;

    return out.str();
}

int displayMenu(bool drawn, bool waste, bool finished)
{
    /*
        Displays the menu of options and returns the option chosen by user
    */

    int choice;
    std::cout << "What would you like to do: (Enter -1 to quit)" << std::endl;
    if (!drawn && !finished)
    {
        std::cout << "1. Draw Card" << std::endl;
    }
    else if (drawn)
    {
        std::cout << "1. Play Drawn Card" << std::endl;
    }
    else if (finished)
    {
        std::cout << "1. Finish Game and Calculate Score" << std::endl;
    }
    if (waste)
    {
        std::cout << "2. Play Card from Waste Pile" << std::endl;
        std::cout << "3. Display an Entire Waste Pile" << std::endl;
    }

    std::cin >> choice;
    if (choice == -1)
    {
        return choice;
    }
    while (!std::cin || choice < 1 || (waste && choice > 3) || (choice > 1 && !waste))
    {
        if (!std::cin) // input failure check only time we reset stream
        {
            resetStream();
        }
        std::cout << "That is an invalid choice" << std::endl;
        std::cin >> choice;
    }
    return choice;
}

void processChoice(bool &drawn, bool isFin, int choice, deck &theDeck, pile f[], pile w[], bool &finished)
{
    /*
        Receives a user choice and takes the corresponding action
    */
    if (choice == 1)
    {
        if (drawn)
        {
            playDrawnCard(theDeck, f, w, drawn);
            // drawn = false;
        }
        else if (isFin)
        {
            finished = true;
            return;
        }
        else
        {
            drawCard(theDeck);
            drawn = true;
        }
    }
    else if (choice == 2)
    {
        playWaste(f, w);
    }
    else if (choice == 3)
    {
        int wp;
        std::cout << "Which waste pile (1-4) do you want to see? ";
        std::cin >> wp;
        while (!std::cin || wp < 1 || wp > 4)
        {
            if (!std::cin)
            {
                resetStream();
            }
            std::cout << "That is not a valid waste pile." << std::endl;
            std::cout << "Which waste pile (1-4) do you want to see? ";
            std::cin >> wp;
        }
        displayWaste(w[wp - 1]);
    }
    finished = false;
}

void drawCard(deck &theDeck)
{
    /*
        Draws a card from the deck
    */
    do
    {
        theDeck.next++;

    } while (theDeck.used[theDeck.next]);
    theDeck.remain--;
}

void playDrawnCard(deck &theDeck, pile foundations[], pile wastes[], bool &drawnCard)
{
    /*
        Based on the drawn card, make a legal move based on the solitaire game
    */
    char pileType;
    int p;
    std::cout << "Do you want to play on a:" << std::endl;
    std::cout << "F. Foundation Pile" << std::endl;
    std::cout << "W. Waste Pile" << std::endl;
    std::cin >> pileType;
    pileType = toupper(pileType);
    while (pileType != 'F' && pileType != 'W')
    {
        std::cout << "I do not understand. I am expecting F or W." << std::endl;
        std::cout << "Do you want to play on a:" << std::endl;
        std::cout << "F. Foundation Pile" << std::endl;
        std::cout << "W. Waste Pile" << std::endl;
        std::cin >> pileType;
        pileType = toupper(pileType);
    }
    std::string pileOut = pileType == 'F' ? "foundation" : "waste";
    std::cout << "Which " << pileOut << " pile (1-4) do you want to play on? ";
    std::cin >> p;
    while (!std::cin || p < 1 || p > 4)
    {
        if (!std::cin)
        {
            resetStream();
        }
        std::cout << "That is not a valid " << pileOut << " pile." << std::endl;
        std::cout << "Which " << pileOut << " pile (1-4) do you want to play on? ";
        std::cin >> p;
    }
    if (pileType == 'F')
    {
        playOnFoundation(theDeck, foundations[p - 1], drawnCard);
    }
    else
    {
        playOnWaste(theDeck, wastes[p - 1]);
        theDeck.used[theDeck.next] = true;
        drawnCard = false;
    }
}

void playWaste(pile foundations[], pile wastes[])
{
    /*
        Performs plays related to the waste pile
    */
    int w, f;
    std::cout << "Which waste pile (1-4) do you want to play from? ";
    std::cin >> w;
    while (!std::cin || w < 1 || w > 4)
    {
        if (!std::cin)
        {
            resetStream();
        }
        std::cout << "That is not a valid waste pile." << std::endl;
        std::cout << "Which waste pile (1-4) do you want to play from? ";
        std::cin >> w;
    }
    std::cout << "Which foundation pile (1-4) do you want to play on? ";
    std::cin >> f;
    while (!std::cin || f < 1 || f > 4)
    {
        if (!std::cin)
        {
            resetStream();
        }
        std::cout << "That is not a valid foundation pile." << std::endl;
        std::cout << "Which foundation pile (1-4) do you want to play on? ";
        std::cin >> f;
    }

    if (foundations[f - 1].p[foundations[f - 1].topCard + 1].r == wastes[w - 1].p[wastes[w - 1].topCard].r)
    {
        foundations[f - 1].p[foundations[f - 1].topCard + 1] = wastes[w - 1].p[wastes[w - 1].topCard];
        foundations[f - 1].topCard++;
        wastes[w - 1].topCard--;
    }
    else
    {
        std::cout << "That is an invalid play. The next card for that pile is a ";
        std::cout << rankStr[foundations[f - 1].p[foundations[f - 1].topCard + 1].r] << std::endl;
    }
}

void displayWaste(const pile &waste)
{
    /*
        Shows the waste pile
    */
    for (int i = waste.topCard; i >= 0; i--)
    {
        std::cout << printASCIICard(waste.p[i]) << std::endl;
    }
}

void playOnFoundation(deck &theDeck, pile &found, bool &drawn)
{
    /*
        Performs plays related to the foundation piles
    */

    if (found.p[found.topCard + 1].r == theDeck.d[theDeck.next].r)
    {
        found.p[found.topCard + 1] = theDeck.d[theDeck.next];
        theDeck.used[theDeck.next] = true;
        found.topCard++;
        drawn = false;
    }
    else
    {
        std::cout << "That is an invalid play. The next card for that pile is a ";
        std::cout << rankStr[found.p[found.topCard + 1].r] << std::endl;
    }
}

void playOnWaste(deck &theDeck, pile &waste)
{
    waste.p[++waste.topCard] = theDeck.d[theDeck.next];
}

int calculateScore(deck &theDeck, pile wastes[])
{
    /*
        Calculates the player`s score
    */
    int score = 0;
    for (int i = 0; i < NUM_PILES; i++)
    {
        score += wastes[i].topCard + 1;
    }
    score += theDeck.remain;

    return score;
}

void resetStream()
{
    /*
        Resets the input stream in case of input failture state.
    */
    std::cout << "You entered something that is not a number." << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}