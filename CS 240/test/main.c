#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int reverse(int source[], int dest[], int n) {
  int *end = dest + n - 1;
  int *start = source;
  int sum = 0;
  for (int x = 0; x < n; x++) {
    sum += *start;
    *end = *start;
    end--;
    start++;
  }
}

typedef struct single_node {
  int number;
  struct single_node *next;
} node_t;

void push(node_t **head, node_t *pre) {
  assert(head != NULL);
  assert(pre != NULL);

  pre->next = *head;
  *head = pre;
}

node_t *pop(node_t **head) {
  assert(head != NULL);
  assert(*head != NULL);

  node_t *temp = *head;
  *head = (*head)->next;
  temp->next = NULL;
  return temp;
}

typedef struct double_node{
  char *name;
  int age;
  struct double_node *next;
  struct double_node *prev;
} node2;

node2 * create(char *name2, int age2) {
  node2 *new = malloc(sizeof(node2));
  new->name = name2;
  new->age = age2;
  new->next = NULL;
  new->prev = NULL;
  return new;
}

int main(int argc, char **argv) {
  int start[] = {1, 2, 3, 4, 5};
  int end[] = {0, 0, 0, 0, 0};
  reverse(start, end, 5);

  for (int x = 0; x < (sizeof(end) / sizeof(int)); x++) {
    printf("%d ", end[x]);
  }
  printf("\n");

  node_t *head = malloc(sizeof(node_t));
  head->number = 1;
  head->next = NULL;

  node_t *pre = malloc(sizeof(node_t));
  pre->number = 2;
  pre->next = NULL;

  push(&head, pre);

  node_t *current = head;
  printf("current: ");
  while (current != NULL) {
    printf("%d ", current->number);
    current = current->next;
  }
  printf("\n");

  node_t *temp = pop(&head);
  printf("removed %d\n", temp->number);

  printf("current: ");
  current = head;
  while (current != NULL) {
    printf("%d ", current->number);
    current = current->next;
  }

}
