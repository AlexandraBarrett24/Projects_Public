/**
 * @file pathFinder.h
 */
#ifndef CSCI3081_PATHFINDER_H_
#define CSCI3081_PATHFINDER_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include "src/vector3.h"
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>

namespace csci3081 {

  /**
   * @brief A class implementing a path finding system
   *
   * This class uses Dijkstra's single source shortest path algorithm
   * The program is for adjacency matrix representation of the graph
   */
class PathFinder {
 public:
  /**
   * @brief PathFinder Constructor
   */

  /// Function to add the graph to Pathfinder.
  void SetGraph(const entity_project::IGraph *graph);

  /// Function that takes an entitiy's position and finds the closest graph node to it.
  std::string findClosestPoint(csci3081::Vector3 position);

  /// Function that takes the name of a node and searches an unordered_map for the node's index.
  int GetIndex(std::string name);

  /// A structure to represent a node in adjacency list
  struct AdjListNode {
    int prev = -6;
    int dest;
    float weight;
    struct AdjListNode* next;
  };


  /// A structure to represent an adjacency list
  struct AdjList {
    // Pointer to head node of list
    struct AdjListNode *head;
  };

  /// A structure to represent a graph (an array of adjacency lists).
  /// Size of array will be V (number of vertices in graph)
  struct Graph {
    int V;
    struct AdjList* array;
  };

  /// Gets the name of the node based on it's index.
  std::string GetNodeName(int index);

  /// A utility function to create a new adjacency list node
  struct AdjListNode* newAdjListNode(int dest, float weight);

  /// A utility function that creates a graph of V vertices
  struct Graph* createGraph(int V);

  /// Adds an edge to an undirected graph
  void addEdge(struct Graph* graph, int src, int dest, float weight);

  /// Structure to represent a min heap node
  struct MinHeapNode {
    int  v;  // maybe change to name
    int dist;
  };

  /// Structure to represent a min heap
  struct MinHeap {
    // Number of heap nodes present currently
    int size;
    // Capacity of min heap
    int capacity;
    // This is needed for decreaseKey()
    int *pos;
    struct MinHeapNode **array;
  };

  /// A utility function to create a new Min Heap Node
  struct MinHeapNode* newMinHeapNode(int v, float dist);

  /// A utility function to create a Min Heap
  struct MinHeap* createMinHeap(int capacity);

  /// A utility function to swap two nodes of min heap. Needed for min heapify
  void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

  /// A standard function to heapify at given idx. This function also updates
  /// position of nodes when they are swapped. Position is needed for decreaseKey().
  void minHeapify(struct MinHeap* minHeap, int idx);

  /// A utility function to check if the given minHeap is empty or not.
  int isEmpty(struct MinHeap* minHeap);

  /// Standard function to extract minimum node from heap.
  struct MinHeapNode* extractMin(struct MinHeap* minHeap);

  /// Function to decrease dist value of a given vertex v. This function
  /// uses pos[] of min heap to get the current index of node in min heap.
  void decreaseKey(struct MinHeap* minHeap, int v, float dist);

  /// A utility function to check if a given vertex 'v' is in min heap or not.
  bool isInMinHeap(struct MinHeap *minHeap, int v);

  /// A utility function used to print the solution.
  void printArr(float dist[], int prev[], int n, struct Graph* graph);

  /// The main function that calulates distances of shortest paths from src to all
  /// vertices. It is a O(ELogV) function src is the index to the starting node
  std::vector<Vector3> dijkstra(struct Graph* graph, int src, int goal);

  /// Takes the last node and iterates through the node's previous noded to create a path.
  std::vector<Vector3> FindPath(int endNode, int startNode, int prev[]);

  /// Driver program to test above functions, called by scheduler.cc
  std::vector<Vector3> algorithm(std::string startNode, std::string endNode);

  /// Creates the unordered_map that will be used to find indexes based on a node's name.
  void CreateMap();
  /// returns the route in string form.
  std::vector<std::string> GetStrRoute() { return strRoute_;}



 private:
  /// Graph of all nodes.
  std::vector<entity_project::IGraphNode *> nodes_;
  std::vector<std::string> strRoute_;
  /// Map of node names and their indexes.
  std::unordered_map<std::string, int> keys_;
  /// graph to be used to find nodes by name.
  const entity_project::IGraph *graph_;
  /// Size of graph, used to create helper data structures of the correct size.
  int graphSize = 0;
};
}  // namespace csci3081

#endif  // CSCI3081_PATHFINDER_H
