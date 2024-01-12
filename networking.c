#include "networking.h"
#include <stdio.h>
#include <time.h>

int printCards(struct card * hand) {
  printf("printing cards\n");
  int count = 0;
  while (hand != NULL) {
    printf("%c%d\n", hand->color, hand->number);
    count++;
    hand = hand->next;
  }
  printf("total: %d cards\n\n", count);
  return count;
}

struct card * create(char _color, int num){
  struct card * tmp = (struct card *) malloc(sizeof(struct card));
  tmp->color = _color;
  tmp->number = num;
  tmp->next = NULL;
  return tmp;
}

void add(struct card * head, char _color, int num){
  struct card * temp;
  struct card * new = create(_color, num);
  temp = head;
  while(temp != NULL && temp->next != NULL){
    temp = temp->next;
  }
  temp->next = new;
}

struct card * draw(struct card * head){
  srand(time(NULL));
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  cC = rand() % 4;
  cN = rand() % 10;
  add(head, cardColor[cC], cardNumber[cN]);
  return head;
}
