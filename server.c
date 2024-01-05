#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct card {
  char color;
  int number;
};

struct card * create(char _color, int _number){
  struct card * tmp;
  tmp->color = _color;
  tmp->number = _number;
  return tmp;
}

void printCard(struct card * _card){
  char info[3];
  char cardColor = _card->color;
  info[0] = cardColor;
  //printf("info[0]: %c\n", info[0]);
  info[1] = _card->number + '0';
  //printf("info[1]: %c\n", info[1]);
  info[2] = '\0';
  //printf("info[2]: %c\n", info[2]);
  char * cardData = info;
  //printf("info: %s\n", info);
  printf("your card is: %s\n", cardData);
}


void randomCard(int n){
  srand(time(NULL));
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  while (n > 0){
    cC = rand() % 4;
    cN = rand() % 10;
    //printf("color: %c\nnumber: %d\n", cardColor[cC], cardNumber[cN]);
    printf("random card: %c%d\n", cardColor[cC], cardNumber[cN]);
    //struct card * tmp = create(cardColor[cC], cardNumber[cN]);
    //printCard(tmp);
    n--;
  }
}

int main() {

  struct card * a = create('v', 3);
  printCard(a);
  randomCard(2);
}
