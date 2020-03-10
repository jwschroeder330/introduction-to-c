#include<stdio.h>
#include<stdlib.h>
#include "input.h"
#include <string.h>
#include <assert.h>

/**
 * Reads the input from f
 * 
 * File has one hand per line (deck_t)
 * 
 * Allocates a deck_t for each hand and places it 
 * into an array of pointers to deck_ts, the return value
 */
deck_t * hand_from_string(const char * str, future_cards_t * fc);
deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc) {
  deck_t ** hands = malloc(sizeof(hands));
  fc->decks = NULL;
  fc->n_decks = 0;
  size_t sz = 0;
  ssize_t len = 0;
  char * line = NULL;
  while((len = getline(&line, &sz, f)) >= 0) {
    (*n_hands) ++;
    deck_t * hand = hand_from_string(line, fc);
    if(hand->n_cards < 5) {
      perror("Poker hand should have at least 5 cards");
      EXIT_FAILURE;
    }
    hands = realloc(hands, sizeof(hands) * (*n_hands));
    hands[(*n_hands) - 1] = hand;
  }
  free(line);
  return hands;
}

void addCardFromString(const char * cardString, deck_t * hand, future_cards_t * fc);
deck_t * hand_from_string(const char * str, future_cards_t * fc) {
  deck_t * hand = malloc(sizeof(*hand));
  int strLength = strlen(str);
  int newCard = 1;  // 1 = create a new card, 0 = continue working on existing card
  char * cardString = malloc(sizeof(*cardString));
  size_t cardStringLen = 0;
  for(int i = 0; i < strLength; i++) {
    char c = str[i];
    switch(c) {
      // string terminator or newline = end of input
    case '\0':
      return hand;
      break;
    case '\n':
      return hand;
      break;
      // we should be on an existing card before we hit a space
    case ' ':
      assert(newCard == 0);
      // create a new card and add to the hand
      addCardFromString(cardString, hand, fc);
      // remove the cardString and start anew
      free(cardString);
      cardString = malloc(sizeof(*cardString));
      cardStringLen = 0;
      newCard = 1;
      break;
      // by default, add the char to the string
    default:
      cardString[cardStringLen] = str[i];
      cardStringLen ++;
      break;
    }
  }
  free(cardString);
  return hand;
}


int toPower(int x, int y) {
  while(y - 1 > 0) {
    x = (x * x);
    y --;
  }
  return x;
}


void addCardFromString(const char * cardString, deck_t * hand, future_cards_t * fc) {
  int strLen = strlen(cardString);
  assert(strLen > 0);
  if(cardString[0] == '?') {
    // get the value to the right of ?
    int unknownValue = 0;
    for(int i = (strLen - 1); i > 0; i --) {
      if(i - 1 == 0) {
	unknownValue += (cardString[i] - '0');
      } else {
	unknownValue += ((cardString[i] - '0') * toPower(10, i));
      }
     }
    printf("%s, %d", cardString, unknownValue);
    card_t * ptr = add_empty_card(hand);
    add_future_card(fc, unknownValue, ptr); 
  } else {
    // normal cards should have two characters
    assert(strLen == 2);
    printf("%c %c | %s", cardString[0], cardString[1], cardString);
    card_t card = card_from_letters(cardString[0], cardString[1]);
    add_card_to(hand, card);
  }
}
