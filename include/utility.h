/**
 * @file utility.h
 * @author DaveDeDave --> https://github.com/DaveDeDave
 * @date August 9, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/* QUEUE - START */

typedef struct queuenode queuenode;

/**
 * @struct queuenode
 * @brief This structure represents a node in a queue.
 * @see queue
 * @var queuenode::value
 * The value of the node.
 * @var queuenode::priority
 * The priority of the node.
 * @var queuenode::next
 * The next node.
 */
struct queuenode {
  int value;
  int priority;
  struct queuenode *next;
};

typedef struct queue queue;

/**
 * @struct queue
 * @brief This structure represents a queue.
 * @var queue::first
 * A pointer to the first element of the queue.
 * @var queue::last
 * A pointer to the last element of the queue.
 * @var queue::size
 * Number of elements in the queue.
 */
struct queue {
  queuenode *first;
  queuenode *last;
  int size;
};

/**
 * @brief Initializes the queue structure.
 *
 * @return the queue.
 */
queue * initializeQueue();

/**
 * @brief Performs an enqueue operation.
 *
 * @param q the queue.
 * @param value the value to enqueue.
 */
void enqueue(queue *q, int value);

/**
 * @brief Performs an enqueue operation with a given priority.
 *
 * @param q the queue.
 * @param value the value to enqueue.
 * @param priority the priority of the enqueued value.
 */
void enqueuePriority(queue *q, int value, int priority);

/** 
 * @brief Performs a dequeue operation.
 *
 * @param q the queue.
 * @return the dequeued value.
 */
int dequeue(queue *q);

/**
 * @brief Performs a priority dequeue operation. It dequeue the value with minimun priority value.
 *
 * @param q the queue.
 * @return the dequeued value.
 */
int dequeuePriority(queue *q);

/**
 * @brief Edits the priority of the given value with the new priority.
 *
 * @param q the queue.
 * @param value the value of which to edit the priority.
 * @param newPriority the new priority.
 */
void editPriority(queue *q, int value, int newPriority);

/**
 * @brief Destroys the queue structure.
 *
 * @param q the queue.
 */
void destroyQueue(queue *q);

/* QUEUE - END */


/* LIST OF LINKED-LIST - START */

typedef struct listnode listnode;

/**
 * @struct listnode
 * @brief This structure represents a node in a linked-list.
* @see lists
 * @var listnode::value
 * The value of the node.
 * @var listnode::next
 * The next node.
 */
struct listnode {
  int value;
  struct listnode *next;
};

typedef struct lists lists;

/**
 * @struct lists
 * @brief This structure represents a list of linked-lists.
 * @var lists::linkedLists
 * The linked-lists.
 * @var lists::size
 * The number of linked-lists.
 */
struct lists {
  listnode **linkedLists;
  int size;
};

/**
 * @brief Initializes the list of linked-list structure.
 *
 * @return the list of linked-list.
 */
lists * initializeLists();

/**
 * @brief Adds a linked-list to the list of linked-lists.
 *
 * @param l the list of linked-lists.
 * @param linkedList the linked-list to add.
 */
void addList(lists *l, listnode *linkedList);

/**
 * @brief Adds a node to a linked-list of the list.
 *
 * @param l the list of linked-lists.
 * @param value the value to add to the linked-list.
 * @param index the index of the linked-list where to add the value.
 */
void addNode(lists *l, int value, int index);

/**
 * @brief Checks if the specified linked-list of the list contains the input value.
 *
 * @param l the list of linked-lists.
 * @param value the value to search in the linked-list.
 * @param index the index of the linked-list where to search the value.
 */
bool containsNode(lists *l, int value, int index);

/**
 * @brief Destroys a linked-list of the list of linked-lists.
 *
 * @param node node of the linked-list.
 */
void destroyLinkedList(listnode *node);

/**
 * @brief Destroys the list of linked-lists.
 *
 * @param l the list of linked-lists.
 */
void destroyLists(lists *l);

/* LIST OF LINKED-LIST - END */


/* K-UNION-FIND - START */

typedef struct k_union_find k_union_find;

/**
 * @struct k_union_find
 * @brief This structure represents a union find used in the kruskal algorithm to find the minimum spanning tree of a graph.
 * @var k_union_find::parent
 * The parent of each node.
 * @var k_union_find::rank
 * The depth of each node-tree.
 * @var k_union_find::size
 * The number of node.
 */
struct k_union_find {
  int *parent;
  int *rank;
  int size;
};

/**
 * @brief Initializes the k-union-find structure.
 *
 * @param size the number of makeSet in the k-union-find.
 * @return the k-union-find.
 */
k_union_find *initializeKUnionFind(int size);

/**
 * @brief Performs the union of two element in the k-union-find by rank.
 *
 * @param kunion the k-union-find.
 * @param a the first element.
 * @param b the second element.
 */
void k_union(k_union_find *kunion, int a, int b);

/**
 * @brief Gets the parent of the given element in the k-union-find.
 *
 * @param kunion the k-union-find.
 * @param a the element.
 * @return the parent of the given element.
 */
int k_find(k_union_find *kunion, int a);

/**
 * @brief Destroys the k-union-find structure.
 *
 * @param kunion the k-union-find.
 */
void destroyKUnionFind(k_union_find *kunion);

/* K-UNION-FIND - END */


/* COMPARE FUNCTIONS - START */

/**
 * @brief Compare function used in getStronglyConnectedComponents and getTopologicalSort to get the descending order of an array.
 *
 * @see qsort
 */
int compareFunUtil1(const void *a, const void *b);

/**
 * @brief Compare function used in getKruskalMST to get the ascending order of an array.
 *
 * @see qsort
 */
int compareFunUtil2(const void *a, const void *b);

/* COMPARE FUNCTIONS - END */