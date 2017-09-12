#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <vector>

#include "avl.h"

int main(){
  int N;
  scanf("%d",&N);
  vector<int> check(N);
  AVL *beachline = new AVL();
  for(int i = 0; i < N; i++){
    int k = (rand() % N) + 1;
    check[i] = k;
    beachline->insert(new Node(k));
  }
  if(beachline->is_avl()) printf("The tree is AVL after %d insertions\n",N);
  for(int i = 0; i < N; i++){
    Node *ptr = beachline->find(check[i]);
    if(ptr){
      beachline->remove(ptr);
    }
    else{
      printf("The node %d with the key: %d was not found, somethin went wrong!\n",i,check[i]);
      break;
    }
  }
  beachline->print();
  delete beachline;
  return 0;
}
