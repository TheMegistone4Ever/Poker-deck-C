/*
Honors Peer-graded Assignment: Week 2 Honors Assignment,
Poker,
Nickita Kyselyov,
06.08.2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STUB_SIZE 7
// traditionally stub size is 5 in poker
#define SUITS_NUM 4
#define PIPS_NUM 13
#define DECK_SIZE 52
#define LEN_OF_TABLE 6

typedef enum suit { Clubs, Hearts, Spades, Diamonds } Suit;
typedef struct card { Suit suit; short pips; } Card;
/* pips: 0-Ace, 1-2, 2-3, 3-4, 4-5, 5-6, 6-7, 7-8, 8-9, 9-10, 10-Jack, 11-Queen, 12-King */

void fillPokerDeck(Card deck[]);
void shufflePokerDeck(Card deck[], int size);
void printDeck(Card deck[], int size, char* str);
void dealOut7CardOnHand(Card stub[], int size, Card deck[], int deckSize);

_Bool fourOfAKind(Card stub[]);
_Bool fullHouses(Card stub[]);
_Bool threeOfAKind(Card stub[]);
_Bool onlyTwoPair(Card stub[]);





int main(void) {
    srand(time(NULL));
    const int total = // 150000000;
                        1000;

    //Init and print deck
    Card deck[DECK_SIZE];
    fillPokerDeck(&deck);
    printDeck(deck, DECK_SIZE, "Deck");

    //Shuffle and print deck
    shufflePokerDeck(&deck, DECK_SIZE);
    printDeck(deck, DECK_SIZE, "Deck after shuffle");

    int strengths[LEN_OF_TABLE] = { 0 };
    /*0-no pair, 1-1 pair, 2-2 pair, 3-3 of a kind , 4-full house , 5-4 of a kind; priority: index in array*/

    Card stub[STUB_SIZE];
    for (int i = 0; i < total; i++, shufflePokerDeck(&deck, DECK_SIZE)) {
        dealOut7CardOnHand(&stub, STUB_SIZE, deck, DECK_SIZE);
        if (fourOfAKind(stub))
            strengths[5]++;
        else if (fullHouses(stub))
            strengths[4]++;
        else if (threeOfAKind(stub))
            strengths[3]++;
        else if (onlyTwoPair(stub))
            strengths[2]++;
        // else if (onlyOnePair(stub))
        //     strengths[1]++;
        // else if (onlyNoPair(stub))
        //     strengths[0]++;
    }

    printf("%12s %12s %9s\n", "Hand", "Combinations", "Probabilities");
    for (int i = 0; i < LEN_OF_TABLE; i++) {
        printf("%12d %12d %.8lf\n", i, strengths[i], (double)strengths[i] / total);
    }

    return 0;
}

void fillPokerDeck(Card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        Card card = { i % SUITS_NUM, i % PIPS_NUM };
        deck[i] = card;
    }
}

void shufflePokerDeck(Card deck[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        Card tmp = deck[j];
        deck[j] = deck[i];
        deck[i] = tmp;
    }
}

void printDeck(Card deck[], int size, char* str) {
    printf("%s:\n", str);
    for (int i = 0; i < size; i++) printf("(%d-%d) ", deck[i].suit, deck[i].pips);
    printf("\n\n");
}

void dealOut7CardOnHand(Card stub[], int size, Card deck[], int deckSize) {
    /*
    Since we need to distribute 7 cards, we will take the first 7
    (index 0 - 6), and not random 7 cards from the deck, because during
    the game, cards are dealt that are on top of the deck to all players
    in turn, one card at a time, and not random from decks every time
    */
    if (deckSize < STUB_SIZE) return;
    for (int i = 0; i < STUB_SIZE; i++) stub[i] = deck[i];
}

_Bool fourOfAKind(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] >= 4) return 1;
    return 0;
}

_Bool fullHouses(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++)
        if (count[i] >= 3) {
            count[i] = 0;
            for (int j = 0; j < PIPS_NUM; j++) if (count[j] >= 2) return 1;
        }
    return 0;
}

_Bool threeOfAKind(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] >= 3) return 1;
    return 0;
}

_Bool onlyTwoPair(Card stub[]) {

}
 