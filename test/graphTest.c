/**
 * @file graphTest.c
 * @author DaveDeDave --> https://github.com/DaveDeDave
 * @date August 9, 2021
 */

#include "../include/graph.h"
#include <assert.h>

void directedGraphTest();
void undirectedGraphTest();

graph * prepareDirectedGraphTest();
graph * prepareUndirectedGraphTest();
graph * editDirectedGraphTest(graph *g);
graph * editUndirectedGraphTest(graph *g);
void BFSTest(graph *g);
void DFSTest(graph *g);
void DFSTotForestTest(graph * g);
void isCyclicTest(graph *g);
void connectedComponentsTest(graph *g);
void stronglyConnectedComponentsTest(graph *g);
void getTopologicalSortTest(graph *g);
void getDijkstraShortestPathsTest(graph *g);
void getBellmanFordShortestPathsTest(graph *g);
void getFloydWarshallShortestsPathsTest(graph *g);
void getPrimMSTTest(graph *g);
void getKruskalMSTTest(graph *g);

int main() {
  directedGraphTest();
  undirectedGraphTest();

  printf("All tests passed.\n");

  return 0;
}

void directedGraphTest() {
  graph *g = prepareDirectedGraphTest();
  g = editDirectedGraphTest(g);

  BFSTest(g);
  DFSTest(g);
  DFSTotForestTest(g);
  isCyclicTest(g);
  stronglyConnectedComponentsTest(g);
  getTopologicalSortTest(g);
  destroyGraph(g);
  g = prepareDirectedGraphTest();
  getDijkstraShortestPathsTest(g);
  getBellmanFordShortestPathsTest(g);
  getFloydWarshallShortestsPathsTest(g);

  destroyGraph(g);
}

void undirectedGraphTest() {
  graph *g = prepareUndirectedGraphTest();
  g = editUndirectedGraphTest(g);

  connectedComponentsTest(g);
  getPrimMSTTest(g);
  getKruskalMSTTest(g);

  destroyGraph(g);
}

graph * prepareDirectedGraphTest() {
  graph *g = initializeGraph(true);

  addVertex(g, 6);

  addEdge(g, 0, 1);
  setEdgeWeight(g, 0, 1, 3);
  addEdge(g, 0, 2);
  setEdgeWeight(g, 0, 2, 6);
  addEdge(g, 0, 4);
  addEdge(g, 1, 3);
  setEdgeWeight(g, 1, 3, 4);
  addEdge(g, 2, 4);
  setEdgeWeight(g, 2, 4, 1);
  addEdge(g, 2, 5);
  setEdgeWeight(g, 2, 5, 1);
  addEdge(g, 4, 2);
  setEdgeWeight(g, 4, 2, 3);
  addEdge(g, 4, 5);
  setEdgeWeight(g, 4, 5, 2);
  addEdge(g, 5, 3);
  setEdgeWeight(g, 5, 3, 3);

  assert(g->size == 6);
  assert(containsVertex(g, 3) == true);
  assert(containsVertex(g, 8) == false);
  assert(containsEdge(g, 0, 4) == true);
  assert(containsEdge(g, 4, 0) == false);
  assert(getEdgeWeight(g, 0, 4) == 1);
  
  return g;
}

graph * prepareUndirectedGraphTest() {
  graph *g = initializeGraph(false);

  addVertex(g, 6);

  addEdgeWeight(g, 0, 1, 1);
  addEdgeWeight(g, 0, 2, 9);
  addEdgeWeight(g, 0, 4, 4);
  addEdgeWeight(g, 0, 5, 1);
  addEdgeWeight(g, 1, 3, 5);
  addEdgeWeight(g, 1, 5, 1);
  addEdgeWeight(g, 2, 4, 3);
  addEdgeWeight(g, 3, 4, 1);
  addEdgeWeight(g, 3, 5, 2);

  assert(g->size == 6);
  assert(containsEdge(g, 2, 4) == true);
  assert(containsEdge(g, 4, 2) == true);
  assert(containsEdge(g, 2, 3) == false);
  assert(getEdgeWeight(g, 0, 2) == 9);
  assert(getEdgeWeight(g, 2, 0) == 9);

  return g;
}

graph * editDirectedGraphTest(graph *g) {
  removeEdge(g, 2, 4);
  removeEdge(g, 4, 2);
  assert(containsEdge(g, 2, 4) == false);

  removeVertex(g, 1);
  assert(g->size == 5);

  assert(getEdgeWeight(g, 0, 1) == 6);
  setEdgeWeight(g, 0, 1, 3);
  assert(getEdgeWeight(g, 0, 1) == 3);

  addVertex(g, 1);

  return g;
}

graph * editUndirectedGraphTest(graph *g) {
  addEdge(g, 2, 3);

  assert(containsEdge(g, 2, 3) == true);
  assert(containsEdge(g, 3, 2) == true);

  removeEdge(g, 3, 2);

  assert(containsEdge(g, 2, 3) == false);
  assert(containsEdge(g, 3, 2) == false);

  return g;
}

void BFSTest(graph *g) {
  int startTimeExpected[] = {0, 4, 8, 2, 6, -1};
  int endTimeExpected[] = {1, 5, 9, 3, 7, -1};
  int parentExpected[] = {-1, 0, 4, 0, 3, -1};
  visitInfo *visit = getBFS(g, 0);

  for(int i = 0; i < g->size; i++) {
    assert(visit->startTime[i] == startTimeExpected[i]);
    assert(visit->endTime[i] == endTimeExpected[i]);
    assert(visit->parent[i] == parentExpected[i]);
  }

  destroyVisit(visit);
}

void DFSTest(graph *g) {
  int startTimeExpected[] = {0, 7, 3, 1, 2, -1};
  int endTimeExpected[] = {9, 8, 4, 6, 5, -1};
  int parentExpected[] = {-1, 0, 4, 0, 3, -1};
  visitInfo *visit = getDFS(g, 0);

  for(int i = 0; i < g->size; i++) {
    assert(visit->startTime[i] == startTimeExpected[i]);
    assert(visit->endTime[i] == endTimeExpected[i]);
    assert(visit->parent[i] == parentExpected[i]);
  }
  assert(visit->color[5] == WHITE);

  destroyVisit(visit);
}

void DFSTotForestTest(graph * g) {
  addEdge(g, 4, 1);

  int startTimeExpected[] = {0, 3, 5, 1, 2, 10};
  int endTimeExpected[] = {9, 4, 6, 8, 7, 11};
  int parentExpected[] = {-1, 4, 4, 0, 3, -1};
  visitInfo *visit = getDFSTOTForest(g, 0);

  for(int i = 0; i < g->size; i++) {
    assert(visit->startTime[i] == startTimeExpected[i]);
    assert(visit->endTime[i] == endTimeExpected[i]);
    assert(visit->parent[i] == parentExpected[i]);
  }
  assert(visit->color[5] == BLACK);

  removeEdge(g, 4, 1);
  destroyVisit(visit);
}

void isCyclicTest(graph *g) {
  assert(isCyclic(g) == false);
  addEdge(g, 4, 0);
  assert(isCyclic(g) == true);
  removeEdge(g, 4, 0);
}

void connectedComponentsTest(graph *g) {
  int expectedCC_0[] = {0, 1, 2, 3, 4, 5};
  int expectedCC_1[] = {6, 7};
  lists *cc = NULL;

  assert(isConnected(g) == true);

  cc = getConnectedComponents(g);
  assert(cc->size == 1);
  for(int i = 0; i < 6; i++) {
    assert(containsNode(cc, expectedCC_0[i], 0) == true);
  }
  destroyLists(cc);

  addVertex(g, 2);
  addEdge(g, 6, 7);
  assert(isConnected(g) == false);
  cc = getConnectedComponents(g);
  assert(cc->size == 2);
  for(int i = 0; i < 6; i++) {
    assert(containsNode(cc, expectedCC_0[i], 0) == true);
  }
  for(int i = 0; i < 2; i++) {
    assert(containsNode(cc, expectedCC_1[i], 1) == true);
  }
  destroyLists(cc);

  removeVertex(g, 7);
  removeVertex(g, 6);
}

void stronglyConnectedComponentsTest(graph *g) {
  int expectedCC[][3] = {{5}, {0, 1, 3}, {2, 4}};
  lists *scc = NULL;

  addEdge(g, 1, 3);
  addEdge(g, 3, 0);
  addEdge(g, 2, 4);
  scc = getStronglyConnectedComponents(g);
  assert(scc->size == 3);
  assert(containsNode(scc, expectedCC[0][0], 0) == true);
  for(int i = 0; i < 3; i++) {
    assert(containsNode(scc, expectedCC[1][i], 1) == true);
  }
  for(int i = 0; i < 2; i++) {
    assert(containsNode(scc, expectedCC[2][i], 2) == true);
  }
  destroyLists(scc);


  assert(isStronglyConnected(g) == false);
  addEdge(g, 2, 1);
  addEdge(g, 0, 5);
  addEdge(g, 5, 0);
  assert(isStronglyConnected(g) == true);

  removeEdge(g, 1, 3);
  removeEdge(g, 3, 0);
  removeEdge(g, 2, 4);
  removeEdge(g, 2, 1);
  removeEdge(g, 0, 5);
  removeEdge(g, 5, 0);
}

void getTopologicalSortTest(graph *g) {
  int expectedOrder[] = {5, 0, 1, 3, 4, 2};
  int *actualOrder = getTopologicalSort(g);

  for(int i = 0; i < g->size; i++) {
    assert(actualOrder[i] == expectedOrder[i]);
  }
  
  free(actualOrder);
}

void getDijkstraShortestPathsTest(graph *g) {
  int expectedDistance[] = {0, 3, 4, 6, 1, 3};
  visitInfo *visit = getDijkstraShortestPaths(g, 0);

  for(int i = 0; i < g->size; i++) {
    assert(visit->distance[i] == expectedDistance[i]);
  }

  destroyVisit(visit);
}

void getBellmanFordShortestPathsTest(graph *g) {
  setEdgeWeight(g, 1, 3, -1);
  setEdgeWeight(g, 2, 5, -2);
  int expectedDistance[] = {0, 3, 4, 2, 1, 2};
  visitInfo *visit = getBellmanFordShortestPaths(g, 0);

  for(int i = 0; i < g->size; i++) {
    assert(visit->distance[i] == expectedDistance[i]);
  }

  destroyVisit(visit);
}

void getFloydWarshallShortestsPathsTest(graph *g) {
  setEdgeWeight(g, 1, 3, -1);
  setEdgeWeight(g, 2, 5, -2);
  int expectedDistance[] = {0, 3, 4, 2, 1, 2};
  int **actualDistance = getFloydWarshallShortestsPaths(g);

  for(int i = 0; i < g->size; i++) {
    assert(actualDistance[0][i] == expectedDistance[i]);
  }

  for(int i = 0; i < g->size; i++) {
    free(actualDistance[i]);
  }
  free(actualDistance);
}

void getPrimMSTTest(graph *g) {
  int expectedEdge[][3] = {{0, 5, 1}, {0, 1, 1}, {5, 3, 2}, {3, 4, 1}, {4, 2, 3}};
  graph *mst = getPrimMST(g);

  assert(getNumberOfEdges(mst) == 5);
  for(int i = 0; i < 5; i++) {
    assert(containsEdge(mst, expectedEdge[i][0], expectedEdge[i][1]) == true);
    assert(getEdgeWeight(mst, expectedEdge[i][0], expectedEdge[i][1]) == expectedEdge[i][2]);
  }

  destroyGraph(mst);
}

void getKruskalMSTTest(graph *g) {
  int expectedEdge[][3] = {{0, 5, 1}, {0, 1, 1}, {5, 3, 2}, {3, 4, 1}, {4, 2, 3}};
  graph *mst = getKruskalMST(g);

  assert(getNumberOfEdges(mst) == 5);
  for(int i = 0; i < 5; i++) {
    assert(containsEdge(mst, expectedEdge[i][0], expectedEdge[i][1]) == true);
    assert(getEdgeWeight(mst, expectedEdge[i][0], expectedEdge[i][1]) == expectedEdge[i][2]);
  }

  destroyGraph(mst);
}