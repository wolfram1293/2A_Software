#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const int maxlen = 1000;

struct node 
{
  char *str;
  struct node *next;
};

typedef struct node Node;


Node *push_front(Node *begin, const char *str)
{

  Node *p = (Node *)malloc(sizeof(Node));
  char *s = (char *)malloc(strlen(str) + 1);
  strcpy(s, str);

  *p = (Node){.str = s , .next = begin};

  return p; 
}


Node *pop_front(Node *begin)
{
  assert(begin != NULL);
  Node *p = begin->next;

  free(begin->str);
  free(begin);

  return p;
}

Node *push_back(Node *begin, const char *str)
{
  if (begin == NULL) { 
    return push_front(begin, str);
  }

  Node *p = begin;
  while (p->next != NULL) {
    p = p->next;
  }

  Node *q = (Node *)malloc(sizeof(Node));
  char *s = (char *)malloc(strlen(str) + 1);
  strcpy(s, str);

  *q = (Node){.str = s, .next = NULL};
  p->next = q;

  return begin;
}

Node *pop_back(Node *begin)
{
  assert(begin != NULL); 
  Node *p = begin;
  if(p->next== NULL){
    free(p->str);
    free(p);
    return NULL;
  }else{
    while (p->next->next != NULL) {
      p = p->next;
    }
    free(p->next->str);
    free(p->next);
    p->next=NULL;
  }
  return begin;
}


Node *remove_all(Node *begin)
{
  while ((begin = pop_front(begin))) 
    ; // Repeat pop_front() until the list becomes empty
  return begin;
}

int main()
{
  Node *begin = NULL; 


  char buf[maxlen];
  while (fgets(buf, maxlen, stdin)) {
    begin = push_front(begin, buf);
    //begin = push_back(begin, buf); // Try this instead of push_front()
  }

  //begin = pop_front(begin);  // What will happen if you do this?
  //begin = pop_back(begin);   // What will happen if you do this?
 
  //begin = remove_all(begin); // What will happen if you do this?

  for (const Node *p = begin; p != NULL; p = p->next) {
    printf("%s", p->str);
  }
  
  return EXIT_SUCCESS;
}
