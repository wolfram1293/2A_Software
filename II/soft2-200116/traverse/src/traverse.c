#include <stdio.h>
#include "traverse.h"

void traverse(const Node *n)
{
  if (n == NULL) return;
  printf("value = %d\n", n->value);

  // 以下に木を操作する手順を実装する
  // どうすればよいか？
  // 表示は printf("value = %d\n", n->value); のような形で良い
  if(n->left!=NULL){
    traverse(n->left);
  }
  if(n->right!=NULL){
    traverse(n->right);
  }


}
