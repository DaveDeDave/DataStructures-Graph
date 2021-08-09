/**
 * @file utility.c
 * @author DaveDeDave --> https://github.com/DaveDeDave
 * @date August 9, 2021
 */

#include "../include/utility.h"


/* QUEUE - START */

queue * initializeQueue() {
  queue *q = (queue *) malloc(sizeof(queue));
  q->first = NULL;
  q->last = NULL;
  q->size = 0;

  return q;
}

void enqueue(queue *q, int value) {
  queuenode *el = (queuenode *) malloc(sizeof(queuenode));
  el->value = value;
  el->priority = 0;
  el->next = NULL;
  
  if(q->size == 0) {
    q->first = el;
    q->last = el;
  } else {
    q->last->next = el;
    q->last = el;
  }

  q->size += 1;
}

void enqueuePriority(queue *q, int value, int priority) {
  enqueue(q, value);
  q->last->priority = priority;
}

int dequeue(queue *q) {
  if(q->size <= 0) return -9999; // gestire meglio

  queuenode *temp = q->first;
  int value = q->first->value;

  q->first = q->first->next;
  free(temp);
  q->size -= 1;

  return value;
}

int dequeuePriority(queue *q) {
  if(q->size <= 0) return -9999; // gestire meglio

  queuenode *temp = q->first;
  queuenode *prev = NULL;
  queuenode *minPrev = NULL;
  queuenode *min = q->first;
  int value;

  while(temp != NULL) {
    if(temp->priority < min->priority) {
      minPrev = prev;
      min = temp;
    }
    prev = temp;
    temp = temp->next;
  }

  value = min->value;
  if(minPrev == NULL) {
    q->first = q->first->next;
  } else {
    minPrev->next = min->next;
  }

  free(min);
  q->size -= 1;

  return value;
}

void editPriority(queue *q, int value, int newPriority) {
  queuenode *temp = q->first;

  while(temp != NULL) {
    if(temp->value == value) {
      temp->priority = newPriority;
      return;
    }
    temp = temp->next;
  }
}

void destroyQueue(queue *q) {
    queuenode *temp = NULL;

    while(q->first != NULL) {
        temp = q->first;
        q->first = q->first->next;
        free(temp);
    }
    free(q);
}

/* QUEUE - END */


/* LIST OF LINKED-LIST - START */

lists * initializeLists() {
  lists *l = (lists *) malloc(sizeof(lists));
  l->linkedLists = NULL;
  l->size = 0;
  
  return l;
}

void addList(lists *l, listnode *linkedList) {
  l->size += 1;

  if(l->linkedLists == NULL) {
    l->linkedLists = (listnode **) malloc(sizeof(listnode *) * l->size);
  } else {
    l->linkedLists = (listnode **) realloc(l->linkedLists, sizeof(listnode *) * l->size);
  }

  l->linkedLists[l->size-1] = linkedList;
}

void addNode(lists *l, int value, int index) {
  if(index < 0 || index >= l->size) return;

  listnode *node = (listnode *) malloc(sizeof(listnode));
  node->value = value;
  node->next = l->linkedLists[index];
  l->linkedLists[index] = node;
}

bool containsNode(lists *l, int value, int index) {
  listnode *node = l->linkedLists[index];

  while(node != NULL) {
    if(node->value == value) {
      return true;
    }
    node = node->next;
  }

  return false;
}

void destroyLinkedList(listnode *node) {
  if(node == NULL) return;

  destroyLinkedList(node->next);
  free(node);
}

void destroyLists(lists *l) {
  for(int i = 0; i < l->size; i++) {
    destroyLinkedList(l->linkedLists[i]);
  }
  free(l->linkedLists);
  free(l);
}

/* LIST OF LINKED-LIST - END */


/* K-UNION-FIND - START */

k_union_find *initializeKUnionFind(int size) {
  k_union_find *kunion = (k_union_find *) malloc(sizeof(k_union_find));
  kunion->parent = (int *) malloc(sizeof(int) * size);
  kunion->rank = (int *) malloc(sizeof(int) * size);
  kunion->size = size;

  for(int i = 0; i < size; i++) {
    kunion->parent[i] = i;
    kunion->rank[i] = 0;
  }

  return kunion;
}

void k_union(k_union_find *kunion, int a, int b) {
  int x = k_find(kunion, a);
  int y = k_find(kunion, b);

  if(kunion->rank[x] > kunion->rank[y]) {
    kunion->parent[y] = x;
  } else {
    kunion->parent[x] = y;
  }

  if(kunion->rank[x] == kunion->rank[y]) {
    kunion->rank[y] += 1;
  }
}

int k_find(k_union_find *kunion, int a) {
  while(kunion->parent[a] != a) {
    a = kunion->parent[a];
  }

  return a;
}

void destroyKUnionFind(k_union_find *kunion) {
  free(kunion->parent);
  free(kunion->rank);
  free(kunion);
}

/* K-UNION-FIND - END */


/* COMPARE FUNCTIONS - START */

int compareFunUtil1(const void *a, const void *b) {
  int *aa = (int *) a;
  int *bb = (int *) b;
  
  return (aa[1] < bb[1]) - (bb[1] < aa[1]);
}

int compareFunUtil2(const void *a, const void *b) {
  int *aa = (int *) a;
  int *bb = (int *) b;
  
  return (aa[2] > bb[2]) - (bb[2] > aa[2]);
}

/* COMPARE FUNCTIONS - END */