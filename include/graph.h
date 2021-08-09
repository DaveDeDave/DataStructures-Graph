/**
 * @file graph.h
 * @author DaveDeDave --> https://github.com/DaveDeDave
 * @date August 9, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/utility.h"

#define MAX_DISTANCE 99999
#define DEFAULT_EDGE_WEIGHT 1

typedef struct edgenode edgenode;

/**
 * @struct edgenode
 * @brief This structure represents a node in an adjacency list.
 * @see graph
 * @var edgenode::edge
 * The number of the adjacent vertex.
 * @var edgenode::weight
 * The weight of the edge.
 * @var edgenode::next
 * The next adjacent vertex.
 */
struct edgenode {
  int edge;
  int weight;
  struct edgenode *next;
};

typedef struct graph graph;

/**
 * @struct graph
 * @brief This structure represents a graph.
 * @var graph::adjList
 * The adjacency lists.
 * @var graph::size
 * The number of vertices in the graph.
 * @var graph::directed
 * Indicates if the graph is directed or not.
 */
struct graph {
  edgenode **adjList;
  int size;
  bool directed;
};

/**
 * @enum color
 * @brief The color that a vertex could have in a graph traversals.
 * @var color::WHITE
 * An undiscovered vertex has this color.
 * @var color::GRAY
 * A discovered vertex has this color if at least one of his adjacent vertex is white.
 * @var color::BLACK
 * A discovered vertex has this color if all of his adjacent vertex aren't white.
 */
enum color {WHITE, GRAY, BLACK};

typedef struct visitInfo visitInfo;

/**
 * @struct visitInfo
 * @brief This structure represents the informations obtained from a graph traversal.
 * @var visitInfo::startTime
 * The start discovering time of a vertex. In a DFS, the start time of a vertex corresponds to when it turns gray.
 * @var visitInfo::endTime
 * The endTime of each vertex. In a DFS, the end time of a vertex corresponds to when it turns black.
 * @var visitInfo::parent
 * The parent of each vertex.
 * @var visitInfo::distance
 * The distance of each vertex from the source.
 * @var visitInfo::color
 * The color of each vertex.
 */
struct visitInfo {
  int *startTime; /* it contains the start visit time for each vertex */
  int *endTime; /* it contains the end visit time for each vertex */
  int *parent; /* it contains the parent of each vertex (if the vertex has no parent it's set to -1) */
  int *distance; /* it contains the distance from the source for each vertex (if the vertex is unreachable it's set to -1) */
  enum color *color; /* it contains the color of each vertex - WHITE -> unvisited, GRAY -> visiting, BLACK -> visit finished */
};


/* INITIALIZATION - START */

/**
 * @brief Initializes the graph structure.
 *
 * @param directed determine if the graph is directed or no.
 * @return the graph pointer.
 */
graph * initializeGraph(bool directed);

/**
 * @brief Prints the graph.
 *
 * @param g the graph to be printed.
 */
void printGraph(graph *g);

/**
 * @brief Initializes the visit information for the graph traversal.
 *
 * @param g the graph.
 * @return the visit information initialized.
 */
visitInfo * initializeVisit(graph *g);

/* INITIALIZATION - END */


/* BASIC GRAPH OPERATIONS - START */

/**
 * @brief Adds vertices to the graph.
 *
 * @param g the graph.
 * @param n number of vertices to add.
 * @return true if the vertices have been added, false otherwise.
 */
bool addVertex(graph *g, int n);

/**
 * @brief Checks if the graph contains the input vertex.
 *
 * @param g the graph.
 * @param vertex the vertex to check.
 * @return true if the vertex is contained in the graph, false otherwise.
 */
bool containsVertex(graph *g, int vertex);

/**
 * @brief Removes a vertex from the graph.
 *
 * @param g the graph.
 * @param vertex the vertex to be removed.
 * @return true if the vertex has been removed, false otherwise.
 */
bool removeVertex(graph *g, int vertex);

/**
 * @brief Adds an edge to the graph.
 *
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @return true if the edge has been added, false otherwise.
 */
bool addEdge(graph *g, int source, int destination);

/**
 * @brief Adds an edge to the graph specifying the weight.
 * 
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @param weight the weight of the edge.
 * @return true if the edge has been added, false otherwise.
 */
bool addEdgeWeight(graph *g, int source, int destination, int weight);

/**
 * @brief Sets the weight of an edge of the graph.
 * 
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @param weight the weight of the edge.
 * @return true if the edge has been added, false otherwise.
 */
bool setEdgeWeight(graph *g, int source, int destination, int weight);

/**
 * @brief Gets the weight of an edge of the graph.
 *
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @return the edge weight (returns 0 if the edge does not exists).
 */
int getEdgeWeight(graph *g, int source, int destination);

/**
 * @brief Checks if the graph contains the input edge.
 *
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @return true if the edge is contained in the graph, false otherwise.
 */
bool containsEdge(graph *g, int source, int destination);

/**
 * @brief Removes an edge from the graph.
 *
 * @param g the graph.
 * @param source the source vertex of the edge.
 * @param destination the destination vertex of the edge.
 * @return true if the edge has been removed, false otherwise.
 */
bool removeEdge(graph *g, int source, int destination);

/**
 * @brief Gets the number of edges in the graph.
 *
 * @param g the graph.
 * @return the number of edges in the graph.
 */
int getNumberOfEdges(graph *g);

/**
 * @brief Gets adjacent vertices of the input vertex.
 *
 * @param g the graph.
 * @param vertex vertex from which we get the adjacent vertices.
 * @return the adjacency list.
 */
edgenode * getAdjacent(graph *g, int vertex);

/** 
 * @brief Checks if the graph is cyclic.
 *
 * @param g the graph.
 * @return true if the graph is cyclic, false otherwise.
 */
bool isCyclic(graph *g);

/** 
 * @brief Recursive implementation of isCyclic.
 *
 * @param g the graph.
 * @param vertex the vertex from which to continue the visit.
 * @param visit the visit information.
 * @return true if the graph is cyclic, false otherwise.
 */
bool isCyclicImpl(graph *g, int vertex, visitInfo *visit);

/** 
 * @brief Checks if the graph is connected (only for undirected graph).
 *
 * @param g the graph.
 * @return true if the graph is connected, false otherwise.
 */
bool isConnected(graph *g);

/** 
 * @brief Checks if the graph is strongly connected (only for directed graph).
 *
 * @param g the graph.
 * @return true if the graph is connected, false otherwise.
 */
bool isStronglyConnected(graph *g);

/**
 * @brief Gets the connected components of an undirected graph.
 *
 * @param g the graph.
 * @return the connected components.
 */
lists * getConnectedComponents(graph *g);

/**
 * @brief Gets the strongly connected components of a directed graph.
 *
 * @param g the graph.
 * @return the strongly connected components.
 */
lists * getStronglyConnectedComponents(graph *g);

/**
 * @brief Gets the transposed graph of the input graph.
 *
 * @param g the graph.
 * @return the transposed graph.
 */
graph * getTransposedGraph(graph *g);

/**
 * @brief Utility function for getConnectedComponents and getStronglyConnectedComponents. Computes the components set.
 *
 * @param g the graph.
 * @param order the order of which to perform dfs of the vertices.
 * @return the components set.
 */
lists * computeComponents(graph *g, int *order);

/**
 * @brief Gets the topological sort of the graph.
 *
 * @param g the graph.
 * @return the topological sort.
 */
int * getTopologicalSort(graph *g);

/* BASIC GRAPH OPERATIONS - END */


/* GRAPH TRAVERSAL - START */

/**
 * @brief Performs a breadth-first-search of the graph.
 *
 * @param g the graph.
 * @param startingVertex the vertex where to start the visit.
 * @return the visit information.
 */
visitInfo * getBFS(graph *g, int startingVertex);

/**
 * @brief Performs a depth-first-search of the graph.
 *
 * @param g the graph.
 * @param startingVertex the vertex where to start the visit.
 * @return the visit information.
 */
visitInfo * getDFS(graph *g, int startingVertex);

/**
 * @brief Recursive implementation of getDFS.
 *
 * @param g the graph.
 * @param vertex the vertex from which to continue the visit.
 * @param time the current visit time.
 * @param visit the visit information.
 * @return the visit information.
 */
visitInfo * getDFSImpl(graph *g, int vertex, int *time, visitInfo *visit);

/**
 * @brief Performs a depth-first-search of the entire graph.
 *
 * @param g the graph.
 * @param startingVertex the vertex where to start the visit.
 * @return the visit information.
 */
visitInfo * getDFSTOTForest(graph *g, int startingVertex);

/* GRAPH TRAVERSAL - END */


/* SHORTEST PATH - START */

/**
 * @brief Performs the Dijkstra shortest path algorithm.
 *
 * @param g the graph.
 * @param startingVertex the vertex where to start the visit.
 * @return the visit information.
 */
visitInfo * getDijkstraShortestPaths(graph *g, int startingVertex);

/**
 * @brief Performs the Bellman-Ford shortest path algorithm.
 *
 * @param g the graph.
 * @param startingVertex the vertex where to start the visit.
 * @return the visit information.
 */
visitInfo * getBellmanFordShortestPaths(graph *g, int startingVertex);

/**
 * @brief Performs the Floyd-Warshall shortest path algorithm.
 *
 * @param g the graph.
 * @return the distance matrix.
 */
int ** getFloydWarshallShortestsPaths(graph *g);

/* SHORTEST PATH - END */


/* MINUMUM SPANNING TREE - START */

/**
 * @brief Performs the Prim algorithm to get the minumum spanning tree of the graph.
 é
 * @param g the graph.
 * @return the minumum spanning tree.
 */
graph * getPrimMST(graph *g);

/**
 * @brief Performs the Kruskal algorithm to get the minumum spanning tree of the graph.
 *
 * @param g the graph.
 * @return the minimum spanning tree.
 */
graph * getKruskalMST(graph *g);

/* MINUMUM SPANNING TREE - END */


/* DEALLOCATING - START */

/**
 * @brief Destroys the visit information structure.
 *
 * @param visit the visit information.
 */
void destroyVisit(visitInfo *visit);

/**
 * @brief Destroys the elements of the adjacency list.
 *
 * @param list the adjacency list to be destroyed.
 */
void destroyAdjList(edgenode *list);

/**
 * @brief Destroys the graph structure.
 *
 * @param g the graph to be destroyed.
 */
void destroyGraph(graph *g);

/* DEALLOCATING - END */