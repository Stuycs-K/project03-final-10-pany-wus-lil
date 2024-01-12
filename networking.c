#include "networking.h"
#include <stdio.h>
#include <time.h>

int printCards(struct card * hand) {
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

bool search(struct card * head, char _color, int num){
  struct card * current = head;
  while (current){
    if (current->color == _color && current->number == num){
      return true;
    }
    current = current->next;
  }
  return false;
}

bool removeCard(struct card ** head, char _color, int num){
  struct card * temp;
  struct card * current = * head;
  if (search(* head, _color, num)){
    if (current && current->color == _color && current->number == num) { // removing first node
      temp = current;
      current = current->next;
      free(temp);
      * head = current;
      return true;
    } else {
      while (current){
        if (current->next != NULL && current->next->color == _color && current->next->number == num){
          temp = current->next;
          current->next = temp->next;
          free(temp);
        }
        current = current->next;
      }
      return true;
    }
  } else return false;
}

struct card * makeHand(int n){
  srand(time(NULL));
  struct card * head = (struct card *) malloc(sizeof(struct card));
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  while (n > 0){
    cC = rand() % 4;
    cN = rand() % 10;
    //printf("color: %c\nnumber: %d\n", cardColor[cC], cardNumber[cN]);
    //printf("card: %c%d\n", cardColor[cC], cardNumber[cN]);
    add(head, cardColor[cC], cardNumber[cN]);
    //printCard(tmp);
    n--;
  }
  removeCard(&head, 0, 0);
  return head;
}
