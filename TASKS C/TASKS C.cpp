/*
Honors Peer-graded Assignment: Week 2 Honors Assignment,
Poker,
Nickita Kyselyov,
06.08.2022

EXAMPLE OF OUT:
Deck:
(0-00) (1-01) (2-02) (3-03) (0-04) (1-05) (2-06) (3-07) (0-08) (1-09) (2-10) (3-11) (0-12)
(1-00) (2-01) (3-02) (0-03) (1-04) (2-05) (3-06) (0-07) (1-08) (2-09) (3-10) (0-11) (1-12)
(2-00) (3-01) (0-02) (1-03) (2-04) (3-05) (0-06) (1-07) (2-08) (3-09) (0-10) (1-11) (2-12)
(3-00) (0-01) (1-02) (2-03) (3-04) (0-05) (1-06) (2-07) (3-08) (0-09) (1-10) (2-11) (3-12)

Deck after shuffle:
(0-03) (1-10) (1-00) (0-07) (0-04) (1-07) (2-07) (1-12) (2-04) (2-09) (3-00) (1-04) (3-05)
(3-01) (2-12) (3-12) (3-09) (3-07) (2-06) (3-06) (3-03) (2-08) (1-01) (2-10) (2-00) (2-11)
(1-05) (0-12) (3-08) (0-08) (1-09) (0-11) (1-02) (0-01) (0-05) (2-03) (0-02) (0-00) (1-11)
(2-01) (3-11) (1-08) (1-06) (0-10) (3-02) (3-04) (3-10) (0-09) (2-02) (1-03) (2-05) (0-06)

            Hand Combinations Probabilities
        No pair:      1051136 0.21022720
       One pair:      2364645 0.47292900
       Two pair:      1200513 0.24010260
Three of a kind:       246256 0.04925120
     Full house:       129025 0.02580500
 Four of a kind:         8425 0.00168500
          Total:      5000000 1.00000000
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
#define PAIR 2

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
_Bool onlyOnePair(Card stub[]);
_Bool onlyNoPair(Card stub[]);

char* hand(int n);

int main(void) {
    srand(time(NULL));
    const int total = 5000000;

    //Init and print deck
    Card deck[DECK_SIZE];
    fillPokerDeck(&deck);
    printDeck(deck, DECK_SIZE, "Deck");

    //Shuffle and print deck
    shufflePokerDeck(&deck, DECK_SIZE);
    printDeck(deck, DECK_SIZE, "Deck after shuffle");

    int strengths[LEN_OF_TABLE] = { 0 };
    /*0-no pair, 1-1 pair, 2-2 pair, 3-3 of a kind , 4-full house , 5-4 of a kind;
    priority: index in array*/

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
        else if (onlyOnePair(stub))
            strengths[1]++;
        else if (onlyNoPair(stub))
            strengths[0]++;
    }

    printf("%16s %12s %9s\n", "Hand", "Combinations", "Probabilities");
    int totalNum = 0;
    double totalProb = 0.;
    for (int i = 0; i < LEN_OF_TABLE; i++) {
        double prob = (double)strengths[i] / total;
        printf("%16s %12d %.8lf\n", hand(i), strengths[i], prob);
        totalNum += strengths[i];
        totalProb += prob;
    }
    printf("%16s %12d %.8lf\n", hand(-1), totalNum, totalProb);

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
    printf("%s:", str);
    for (int i = 0; i < size; i++) {
        if (i % PIPS_NUM == 0) printf("\n");
        printf("(%d-%02d) ", deck[i].suit, deck[i].pips);
    }
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
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] >= (PAIR << 1)) return 1;
    return 0;
}

_Bool fullHouses(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++)
        if (count[i] >= (PAIR + 1)) {
            count[i] = 0;
            for (int j = 0; j < PIPS_NUM; j++) if (count[j] >= PAIR) return 1;
        }
    return 0;
}

_Bool threeOfAKind(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] >= (PAIR + 1 )) return 1;
    return 0;
}

_Bool onlyTwoPair(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++)
        if (count[i] >= PAIR) {
            count[i] = 0;
            for (int j = 0; j < PIPS_NUM; j++) if (count[j] >= PAIR) return 1;
        }
    return 0;
}
 
_Bool onlyOnePair(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] >= PAIR) return 1;
    return 0;
}

_Bool onlyNoPair(Card stub[]) {
    short count[PIPS_NUM] = { 0 }; //for counting pips
    for (int i = 0; i < STUB_SIZE; i++) count[stub[i].pips]++;
    for (int i = 0; i < PIPS_NUM; i++) if (count[i] < PAIR) return 1;
    return 0;
}

char* hand(int n) {
    switch (n) {
        case 0: return "No pair:";
        case 1: return "One pair:";
        case 2: return "Two pair:";
        case 3: return "Three of a kind:";
        case 4: return "Full house:";
        case 5: return "Four of a kind:";
        default: return "Total:";
    }
}