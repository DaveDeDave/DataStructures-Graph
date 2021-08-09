/**
 * @file graph.c
 * @author DaveDeDave --> https://github.com/DaveDeDave
 * @date August 9, 2021
 */

#include "../include/graph.h"


/* INITIALIZATION - START */

graph * initializeGraph(bool directed) {
  graph *g = (graph *) malloc(sizeof(graph));
  g->size = 0;
  g->directed = directed;
  g->adjList = NULL;

  return g;
}

void printGraph(graph *g) {
  edgenode *edges = NULL;

  printf("Graph info\n\n");
  printf("Directed: %s\n", g->directed ? "true" : "false");
  printf("Number of vertices: %d\n", g->size);
  printf("Number of edges: %d\n\n", getNumberOfEdges(g));

  for(int i = 0; i < g->size; i++) {
    edges = g->adjList[i];
    while(edges != NULL) {
      if(g->directed) {
        printf("%d --> %d (Weight %d)\n", i, edges->edge, edges->weight);
      } else if(!g->directed && edges->edge > i) {
        printf("%d <--> %d (Weight %d)\n", i, edges->edge, edges->weight);
      }
      edges = edges->next;
    }
  }
  printf("\n");
}

visitInfo * initializeVisit(graph *g) {
  visitInfo *visit = (visitInfo *) malloc(sizeof(visitInfo));
  visit->startTime = (int *) malloc(sizeof(int) * g->size);
  visit->endTime = (int *) malloc(sizeof(int) * g->size);
  visit->parent = (int *) malloc(sizeof(int) * g->size);
  visit->distance = (int *) malloc(sizeof(int) * g->size);
  visit->color = (enum color *) malloc(sizeof(enum color) * g->size);
  for(int i = 0; i < g->size; i++) {
    visit->startTime[i] = -1;
    visit->endTime[i] = -1;
    visit->parent[i] = -1;
    visit->distance[i] = MAX_DISTANCE;
    visit->color[i] = WHITE;
  }

  return visit;
}

/* INITIALIZATION - END */


/* BASIC GRAPH OPERATIONS - START */

bool addVertex(graph *g, int n) {
  if(n <= 0) return false; 
  
  g->size += n;
  if(g->adjList == NULL) {
    g->adjList = (edgenode **) malloc(sizeof(edgenode *) * g->size);
    for(int i = 0; i < g->size; i++) {
      g->adjList[i] = NULL;
    }
  } else {
    g->adjList = (edgenode **) realloc(g->adjList, sizeof(edgenode *) * g->size);
    for(int i = g->size - n; i < g->size; i++) {
      g->adjList[i] = NULL;
    }
  }

  return true;
}

bool containsVertex(graph *g, int vertex) {
  if(vertex >= 0 && vertex < g->size) {
    return true;
  } else {
    return false;
  }
}

bool removeVertex(graph *g, int vertex) {
  if(!containsVertex(g, vertex)) return false;

  edgenode *edges = NULL;
  edgenode *predecessor = NULL;
  edgenode *temp = NULL;

  destroyAdjList(g->adjList[vertex]);
  for(int i = vertex; i < g->size-1; i++) {
    g->adjList[i] = g->adjList[i+1];
  }
  g->adjList[g->size-1] = NULL;

  g->size -= 1;
  g->adjList = (edgenode **) realloc(g->adjList, sizeof(edgenode *) * g->size);

  for(int i = 0; i < g->size; i++) {
    edges = g->adjList[i];
    predecessor = NULL;
    while(edges != NULL) {
      if(edges->edge == vertex) {
        if(predecessor == NULL) {
          temp = edges;
          g->adjList[i] = edges->next;
          edges = edges->next;
          free(temp);
        } else {
          temp = edges;
          predecessor->next = edges->next;
          edges = edges->next;
          free(temp);
        }
      } else if(edges->edge > vertex) {
        edges->edge -= 1;
        predecessor = edges;
        edges = edges->next;
      } else {
        predecessor = edges;
        edges = edges->next;
      }
    }
  }

  return true;
}

bool addEdge(graph *g, int source, int destination) {
  if(!containsVertex(g, source) || !containsVertex(g, destination) || containsEdge(g, source, destination)) return false;

  edgenode *node = NULL;

  node = (edgenode *) malloc(sizeof(edgenode));
  node->edge = destination;
  node->weight = DEFAULT_EDGE_WEIGHT;
  node->next = g->adjList[source];
  g->adjList[source] = node;

  if(!g->directed) {
    node = (edgenode *) malloc(sizeof(edgenode));
    node->edge = source;
    node->weight = DEFAULT_EDGE_WEIGHT;
    node->next = g->adjList[destination];
    g->adjList[destination] = node;
  }

  return true;
}

bool addEdgeWeight(graph *g, int source, int destination, int weight) {
  bool added = addEdge(g, source, destination);

  if(added) {
    g->adjList[source]->weight = weight;
    if(!g->directed) {
      g->adjList[destination]->weight = weight;
    }

    return true;
  }

  return false;
}

bool setEdgeWeight(graph *g, int source, int destination, int weight) {
  edgenode *adj = g->adjList[source];
  bool find = false;

  while(adj != NULL && !find) {
    if(adj->edge == destination) {
      adj->weight = weight;
      find = true;
    }
    adj = adj->next;
  }

  if(!g->directed) {
    adj = g->adjList[destination];

    while(adj != NULL && !find) {
      if(adj->edge == source) {
        adj->weight = weight;
        find = true;
      }
      adj = adj->next;
    } 
  }

  return true;
}

int getEdgeWeight(graph *g, int source, int destination) {
  if(!containsEdge(g, source, destination)) return 0;

  edgenode *edges = g->adjList[source];
  bool find = false;

  while(edges != NULL && !find) {
    if(edges->edge == destination) {
      find = true;
    } else {
      edges = edges->next;
    }
  }

  return edges->weight;
}

bool containsEdge(graph *g, int source, int destination) {
  if(!containsVertex(g, source) || !containsVertex(g, destination)) return false;

  edgenode *edges = g->adjList[source];

  while(edges != NULL) {
    if(edges->edge == destination) {
      return true;
    }
    edges = edges->next;
  }

  return false;
}

bool removeEdge(graph *g, int source, int destination) {
  if(!containsEdge(g, source, destination)) return false;

  edgenode *edges = g->adjList[source];
  edgenode *predecessor = NULL;
  edgenode *temp = NULL;
  int removed = 0;

  while(edges != NULL && removed == 0) {
    if(edges->edge == destination) {
      if(predecessor == NULL) {
        temp = edges;
        g->adjList[source] = edges->next;
        free(temp);
      } else {
        temp = edges;
        predecessor->next = edges->next;
        free(temp);
      }
      removed = 1;
    } else {
      predecessor = edges;
      edges = edges->next;
    }
  }

  if(!g->directed) {
    edges = g->adjList[destination];
    predecessor = NULL;
    temp = NULL;
    removed = 0;

    while(edges != NULL && removed == 0) {
      if(edges->edge == source) {
        if(predecessor == NULL) {
          temp = edges;
          g->adjList[destination] = edges->next;
          free(temp);
        } else {
          temp = edges;
          predecessor->next = edges->next;
          free(temp);
        }
        removed = 1;
      } else {
        edges = edges->next;
      }
    }
  }

  return true;
}

int getNumberOfEdges(graph *g) {
  edgenode *adj = NULL;
  int nedges = 0;

  for(int v = 0; v < g->size; v++) {
    adj = g->adjList[v];
    while(adj != NULL) {
      nedges++;
      adj = adj->next;
    }
  }

  return g->directed ? nedges : nedges / 2;
}

edgenode * getAdjacent(graph *g, int vertex) {
  if(!containsVertex(g, vertex)) return NULL;

  return g->adjList[vertex];
}

bool isCyclic(graph *g) {
  visitInfo *visit = initializeVisit(g);
  for(int i = 0; i < g->size; i++) {
    if(visit->color[i] == WHITE) {
      if(isCyclicImpl(g, i, visit) == true) {
        destroyVisit(visit);
        return true;
      }
    }
  }

  destroyVisit(visit);
  return false;
}

bool isCyclicImpl(graph *g, int vertex, visitInfo *visit) {
  visit->color[vertex] = GRAY;
  edgenode *adj = getAdjacent(g, vertex);

  while(adj != NULL) {
    if(visit->color[adj->edge] == WHITE) {
      visit->parent[adj->edge] = vertex;
      if(isCyclicImpl(g, adj->edge, visit) == true) {
        return true;
      }
    } else if(g->directed && visit->color[adj->edge] == GRAY) {
      return true;
    } else if(!g->directed && visit->color[adj->edge] == GRAY && visit->parent[vertex] != adj->edge) {
      return true;
    }
    adj = adj->next;
  }

  visit->color[vertex] = BLACK;
  return false;
}

bool isConnected(graph *g) {
  if(g->directed) return false;

  visitInfo *visit = NULL;
  
  visit = getDFS(g, 0);
  for(int i = 0; i < g->size; i++) {
    if(visit->color[i] != BLACK) {
      destroyVisit(visit);
      return false;
    }
  }

  destroyVisit(visit);
  return true;
}

bool isStronglyConnected(graph *g) {
  if(!g->directed) return false;

  lists *stronglyConnectedComponents = getStronglyConnectedComponents(g);

  if(stronglyConnectedComponents != NULL) {
    for(int i = 0; i < g->size; i++) {
      if(containsNode(stronglyConnectedComponents, i, 0) == false) {
        destroyLists(stronglyConnectedComponents);
        return false;
      }
    }

    destroyLists(stronglyConnectedComponents);
    return true;
  }
  
  return false;
}

lists * getConnectedComponents(graph *g) {
  if(g->directed) return NULL;
  
  int order[g->size];
  for(int i = 0; i < g->size; i++) {
    order[i] = i;
  }

  return computeComponents(g, order);
}

/**
 * It is used the Kosaraju algorithm to calculate the strongly connected components:
 *
 * 1. order will contain the vertices ordered by visit's end time
 * 2. calculate the transposed graph
 * 3. perform DFS on the transposed graph with the order calculated at point 1
 */
lists * getStronglyConnectedComponents(graph *g) {
  if(!g->directed) return NULL;

  graph *gTranspose = NULL;
  lists *stronglyConnectedComponents = NULL;
  visitInfo *visit = getDFSTOTForest(g, 0);;
  int order_endtime[g->size][2];
  int order[g->size];

  for(int i = 0; i < g->size; i++) {
    order_endtime[i][0] = i;
    order_endtime[i][1] = visit->endTime[i];
  }
  qsort(order_endtime, g->size, sizeof(order_endtime[0]), compareFunUtil1);
  for(int i = 0; i < g->size; i++) {
    order[i] = order_endtime[i][0];
  }

  gTranspose = getTransposedGraph(g);
  
  stronglyConnectedComponents = computeComponents(gTranspose, order);

  destroyVisit(visit);
  destroyGraph(gTranspose);
  return stronglyConnectedComponents;
}

graph * getTransposedGraph(graph *g) {
  graph *gTranspose = initializeGraph(true);
  edgenode *adj = NULL;
  addVertex(gTranspose, g->size);

  for(int i = 0; i < g->size; i++) {
    adj = g->adjList[i];
    while(adj != NULL) {
      addEdge(gTranspose, adj->edge, i);
      adj = adj->next;
    }
  }
  
  return gTranspose;
}

lists * computeComponents(graph *g, int *order) {
  visitInfo *visit = initializeVisit(g);
  int time = 0;

  lists *connectedComponents = initializeLists();
  listnode *component = NULL;
  listnode *node = NULL;
  bool visited[g->size];

  for(int i = 0; i < g->size; i++) {
    visited[i] = false;
  }

  for(int i = 0; i < g->size; i++) {
    if(visit->color[order[i]] == WHITE) {
      getDFSImpl(g, order[i], &time, visit);
      component = NULL;

      for(int v = 0; v < g->size; v++) {
        if(visit->color[v] == BLACK && visited[v] == false) {
          visited[v] = true;

          node = (listnode *) malloc(sizeof(listnode));
          node->value = v;
          node->next = component;
          component = node;
        }
      }

      if(component != NULL) {
        addList(connectedComponents, component);
      }
    }
  }
  
  destroyVisit(visit);
  return connectedComponents;
}

int * getTopologicalSort(graph *g) {
  if(!g->directed || isCyclic(g)) return NULL;

  visitInfo *visit = getDFSTOTForest(g, 0);
  int *order = (int *) malloc(sizeof(int) * g->size);
  int order_endtime[g->size][2];

  for(int i = 0; i < g->size; i++) {
    order_endtime[i][0] = i;
    order_endtime[i][1] = visit->endTime[i];
  }

  qsort(order_endtime, g->size, sizeof(order_endtime[0]), compareFunUtil1);
  
  for(int i = 0; i < g->size; i++) {
    order[i] = order_endtime[i][0]+1-1;
  }
  
  destroyVisit(visit);

  return order;
}

/* BASIC GRAPH OPERATIONS - END */


/* GRAPH TRAVERSAL - START */

visitInfo * getBFS(graph *g, int startingVertex) {
  if(!containsVertex(g, startingVertex)) return NULL;

  visitInfo *visit = initializeVisit(g);
  queue *q = initializeQueue();
  edgenode *adj = NULL;
  int vertex;
  int time = 0;

  visit->distance[startingVertex] = 0;
  visit->color[startingVertex] = GRAY;
  enqueue(q, startingVertex);
  while(q->size != 0) {
    vertex = dequeue(q);
    visit->startTime[vertex] = time++;
    adj = getAdjacent(g, vertex);
    while(adj != NULL) {
      if(visit->color[adj->edge] == WHITE) {
        visit->color[adj->edge] = GRAY;
        visit->parent[adj->edge] = vertex;
        enqueue(q, adj->edge);
      }
      adj = adj->next;
    }
    visit->color[vertex] = BLACK;
    visit->endTime[vertex] = time++;
  }
  destroyQueue(q);

  return visit;
}

visitInfo * getDFS(graph *g, int startingVertex) {
  if(!containsVertex(g, startingVertex)) return NULL;

  visitInfo *visit = initializeVisit(g);
  int time = 0;
  
  return getDFSImpl(g, startingVertex, &time, visit);
}

visitInfo * getDFSImpl(graph *g, int vertex, int *time, visitInfo *visit) {
  visit->color[vertex] = GRAY;
  visit->startTime[vertex] = (*time)++;
  
  edgenode *adj = getAdjacent(g, vertex);
  while(adj != NULL) {
    if(visit->color[adj->edge] == WHITE) {
      visit->parent[adj->edge] = vertex;
      getDFSImpl(g, adj->edge, time, visit);
    }
    adj = adj->next;
  }

  visit->color[vertex] = BLACK;
  visit->endTime[vertex] = (*time)++;
  return visit;
}

visitInfo * getDFSTOTForest(graph *g, int startingVertex) {
  if(!containsVertex(g, startingVertex)) return NULL;

  visitInfo *visit = initializeVisit(g);
  int time = 0;

  visit = getDFSImpl(g, startingVertex, &time, visit);
  for(int i = 0; i < g->size; i++) {
    if(visit->color[i] == WHITE)
      visit = getDFSImpl(g, i, &time, visit);
  }
  
  return visit;
}

/* GRAPH TRAVERSAL - END */


/* SHORTEST PATH - START */

visitInfo * getDijkstraShortestPaths(graph *g, int startingVertex) {
  if(!containsVertex(g, startingVertex)) return NULL;
  
  visitInfo *visit = initializeVisit(g);
  queue *q = initializeQueue();
  edgenode *adj = NULL;
  int vertex;

  visit->distance[startingVertex] = 0;
  for(int v = 0; v < g->size; v++) {
    enqueuePriority(q, v, visit->distance[v]);
  }

  while(q->size > 0) {
    vertex = dequeuePriority(q);
    adj = getAdjacent(g, vertex);
    while(adj != NULL) {
      if(adj->weight < 0) {
        return NULL;
      }

      if(visit->distance[adj->edge] > visit->distance[vertex] + adj->weight) {
        visit->parent[adj->edge] = vertex;
        visit->distance[adj->edge] = visit->distance[vertex] + adj->weight;
        editPriority(q, adj->edge, visit->distance[adj->edge]);
      }
      adj = adj->next;
    }
  }
  destroyQueue(q);

  return visit;
}

visitInfo * getBellmanFordShortestPaths(graph *g, int startingVertex) {
  if(!containsVertex(g, startingVertex)) return NULL;

  visitInfo *visit = initializeVisit(g);
  edgenode *adj = NULL;
  int nedges = getNumberOfEdges(g);
  int edges[nedges][3];
  int j = 0;

  for(int v = 0; v < g->size; v++) {
    adj = g->adjList[v];
    while(adj != NULL) {
      edges[j][0] = v;
      edges[j][1] = adj->edge;
      edges[j][2] = adj->weight;
      j++;
      adj = adj->next;
    }
  }

  visit->distance[startingVertex] = 0;

  for(int i = 0; i < g->size - 1; i++) {
    for(j = 0; j < nedges; j++) {
      if(visit->distance[edges[j][1]] > visit->distance[edges[j][0]] + edges[j][2]) {
        visit->parent[edges[j][1]] = edges[j][0];
        visit->distance[edges[j][1]] = visit->distance[edges[j][0]] + edges[j][2];
      }
    }
  }

  // Check negative cycle
  for(j = 0; j < nedges; j++) {
    if(visit->distance[edges[j][1]] > visit->distance[edges[j][0]] + edges[j][2]) {
      return NULL;
    }
  }

  return visit;
}

int ** getFloydWarshallShortestsPaths(graph *g) {
  int **distance = (int **) malloc(sizeof(int *) * g->size);
  for(int i = 0; i < g->size; i++) {
    distance[i] = (int *) malloc(sizeof(int) * g->size);
  }
  //int *parent[g->size] = (int **) malloc(sizeof(int *) * g->size);
  
  for(int i = 0; i < g->size; i++) {
    for(int j = 0; j < g->size; j++) {
      //parent[i][j] = -1;
      if(i == j) {
        distance[i][j] = 0;
      } else if(containsEdge(g, i, j)) {
        distance[i][j] = getEdgeWeight(g, i, j);
        //parent[i][j] = i;
      } else {
        distance[i][j] = MAX_DISTANCE;
      }
    }
  }

  for(int k = 0; k < g->size; k++) {
    for(int i = 0; i < g->size; i++) {
      for(int j = 0; j < g->size; j++) {
        if(distance[i][j] > distance[i][k] + distance[k][j]) {
          distance[i][j] = distance[i][k] + distance[k][j];
          //parent[i][j] = parent[k][j];
        }
      
        // Check negative cycle
        if(i == j && distance[i][j] < 0) {
          return NULL;
        }
      }
    }
  }

  return distance;
}

/* SHORTEST PATH - END */


/* MINUMUM SPANNING TREE - START */

graph * getPrimMST(graph *g) {
  if(!isConnected(g)) return NULL;

  graph *mst = initializeGraph(false);
  visitInfo *visit = initializeVisit(g);
  queue *q = initializeQueue();
  edgenode *adj = NULL;
  int vertex;

  addVertex(mst, g->size);

  visit->distance[0] = 0;
  for(int v = 0; v < g->size; v++) {
    enqueuePriority(q, v, visit->distance[v]);
  }

  while(q->size > 0) {
    vertex = dequeuePriority(q);
    if(visit->parent[vertex] != -1) {
      addEdgeWeight(mst, visit->parent[vertex], vertex, getEdgeWeight(g, visit->parent[vertex], vertex));
    }
    visit->color[vertex] = BLACK;
    adj = getAdjacent(g, vertex);
    while(adj != NULL) {
      if(visit->color[adj->edge] != BLACK && visit->distance[adj->edge] > adj->weight) {
        visit->parent[adj->edge] = vertex;
        visit->distance[adj->edge] = adj->weight;
        editPriority(q, adj->edge, visit->distance[adj->edge]);
      }
      adj = adj->next;
    }
  }
  destroyQueue(q);
  destroyVisit(visit);

  return mst;
}

graph * getKruskalMST(graph *g) {
  if(!isConnected(g)) return NULL;

  graph *mst = initializeGraph(false);
  k_union_find *kunion = initializeKUnionFind(g->size);
  edgenode *adj = NULL;
  int nedges = getNumberOfEdges(g);
  int edges[nedges][3];
  int count = 0;
  int j = 0;
  
  for(int v = 0; v < g->size; v++) {
    adj = g->adjList[v];
    while(adj != NULL) {
      if(adj->edge > v) {
        edges[j][0] = v;
        edges[j][1] = adj->edge;
        edges[j][2] = adj->weight;
        j++;
      }
      adj = adj->next;
    }
  }
  qsort(edges, nedges, sizeof(edges[0]), compareFunUtil2);

  addVertex(mst, g->size);

  for(int i = 0; i < nedges; i++) {
    if(count == g->size-1) {
      destroyKUnionFind(kunion);
      return mst;
    }
    
    if(k_find(kunion, edges[i][0]) != k_find(kunion, edges[i][1])) {
      k_union(kunion, edges[i][0], edges[i][1]);
      addEdgeWeight(mst, edges[i][0], edges[i][1], edges[i][2]);
      count += 1;
    }
  }

  destroyKUnionFind(kunion);
  return mst;
}

/* MINUMUM SPANNING TREE - END */


/* DEALLOCATING - START */

void destroyVisit(visitInfo *visit) {
  free(visit->startTime);
  free(visit->endTime);
  free(visit->parent);
  free(visit->distance);
  free(visit->color);
  free(visit);
}

void destroyAdjList(edgenode *list) {
  if(list == NULL) return;

  destroyAdjList(list->next);
  free(list);
}

void destroyGraph(graph *g) {
  for(int i = 0; i < g->size; i++) {
    destroyAdjList(g->adjList[i]);
  }
  free(g->adjList);
  free(g);
}

/* DEALLOCATING - END */