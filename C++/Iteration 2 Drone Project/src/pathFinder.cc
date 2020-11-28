/**
 * @file pathFinder.cc
 */

#include "src/pathFinder.h"

namespace csci3081 {
  void PathFinder::SetGraph(const entity_project::IGraph *graph) {
    graph_ = graph;
    nodes_ = graph->GetNodes();
    graphSize = nodes_.size();
    CreateMap();
  }

  std::string PathFinder::findClosestPoint(csci3081::Vector3 position) {
    // std::cout << "find closest point" << std::endl;
    std::string closest = nodes_[0]->GetName();
    csci3081::Vector3 pos1 = Vector3(nodes_[0]->GetPosition());
    float cDistance = position.DistanceBetween(pos1);
    int size = nodes_.size();
    for (int i =  1; i < size; i++) {
      csci3081::Vector3 pos = Vector3(nodes_[i]->GetPosition());
      float newDist = position.DistanceBetween(pos);
      if (newDist < cDistance) {
        cDistance = newDist;
        closest = nodes_[i]->GetName();
      }
    }
    return closest;
  }

  int PathFinder::GetIndex(std::string name) {
    // std::cout << "GetIndex" << std::endl;
    std::unordered_map<std::string, int>::const_iterator got = keys_.find(name);
    if (got == keys_.end()) {
      return -1;
    } else {
      return got->second;
    }
  }

  std::string PathFinder::GetNodeName(int index) {
    // std::cout << "GetNodeName" << std::endl;
    return nodes_[index]->GetName();
  }


  struct PathFinder::AdjListNode* PathFinder::newAdjListNode(int dest, float weight) {
    // std::cout << "AdjListNode" << std::endl;
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
  }

  struct PathFinder::Graph* PathFinder::createGraph(int V) {
    // std::cout << "createGraph" << std::endl;
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    // Create an array of adjacency lists.
    // Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    // Initialize each adjacency list
    // as empty by making head as NULL
    for (int i = 0; i < V; ++i) {
      graph->array[i].head = NULL;
    }
    return graph;
  }

  void PathFinder::addEdge(struct Graph* graph, int src, int dest, float weight) {
    // std::cout << "add edge" << std::endl;
    // Add an edge from src to dest. A new node is added to the adjacency
    // list of src.  The node is added at the beginning
    struct PathFinder::AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
  }


  struct PathFinder::MinHeapNode* PathFinder::newMinHeapNode(int v, float dist) {
    // std::cout << "PathFinder" << std::endl;
    struct PathFinder::MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct
      MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
  }

  struct PathFinder::MinHeap* PathFinder::createMinHeap(int capacity) {
    // std::cout << "createMinHeap" << std::endl;
    struct PathFinder::MinHeap* minHeap =
    reinterpret_cast<struct PathFinder::MinHeap*>(malloc(sizeof(struct MinHeap)));
    minHeap->pos = reinterpret_cast<int *>(malloc(capacity * sizeof(int)));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity *
      sizeof(struct PathFinder::MinHeapNode*));
    return minHeap;
  }

  void PathFinder::swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    // std::cout << "swapMinHeapNode" << std::endl;
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
  }

  void PathFinder::minHeapify(struct MinHeap* minHeap, int idx) {
    // std::cout << "minHeapify" << std::endl;
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
    if (smallest != idx) {
      // The nodes to be swapped in min heap
      MinHeapNode *smallestNode = minHeap->array[smallest];
      MinHeapNode *idxNode = minHeap->array[idx];
      // Swap positions
      minHeap->pos[smallestNode->v] = idx;
      minHeap->pos[idxNode->v] = smallest;
      // Swap nodes
      swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
      minHeapify(minHeap, smallest);
    }
  }

  int PathFinder::isEmpty(struct PathFinder::MinHeap* minHeap) {
    // std::cout << "check is empty" << std::endl;
    return minHeap->size == 0;
  }

  struct PathFinder::MinHeapNode* PathFinder::extractMin(struct PathFinder::MinHeap* minHeap) {
    // std::cout << "extractmin" << std::endl;
    if (isEmpty(minHeap))
      return NULL;

    // Store the root node
    struct PathFinder::MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
  }

  void PathFinder::decreaseKey(struct PathFinder::MinHeap* minHeap, int v, float dist) {
    // std::cout << "decreaseKey" << std::endl;
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified. This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
      // Swap this node with its parent
      minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
      minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
      swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
      // move to parent index
      i = (i - 1) / 2;
    }
  }

  bool PathFinder::isInMinHeap(struct PathFinder::MinHeap *minHeap, int v) {
    // std::cout << "isInMinHeap" << std::endl;
    if (minHeap->pos[v] < minHeap->size)
      return true;
    return false;
  }

  void PathFinder::printArr(float dist[], int prev[], int n, struct Graph* graph) {
    // std::cout << "printArr" << std::endl;
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i) {
      if (prev[i] <= 20 && prev[i] > 0) {
        std::cout << GetNodeName(i) << ", " << dist[i] << ", " << prev[i] << std::endl;
      }
    }
  }

  std::vector<Vector3> PathFinder::dijkstra(struct Graph* graph, int src, int goal) {
    // std::cout << "dijkstra" << std::endl;
    // Get the number of vertices in graph
    int V = graph->V;
    // dist values used to pick minimum weight edge in cut
    float dist[V];
    int prev[V];
    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v) {
      dist[v] = 1000000;
      prev[v] = -2;
      minHeap->array[v] = newMinHeapNode(v, dist[v]);
      minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    // minHeap->array[src] = newMinHeapNode(src, dist[src]);
    // minHeap->pos[src] = src;
    dist[src] = 0;
    prev[src] = src;
    // std::cout << src << " " << dist[src] << std::endl;
    decreaseKey(minHeap, src, dist[src]);
    // minHeapify(minHeap, src);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In this loop, min heap contains all nodes whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
      // Extract the vertex with minimum distance value
      struct MinHeapNode* minHeapNode = extractMin(minHeap);

      // Store the extracted vertex number
      int u = minHeapNode->v;


      // Traverse through all adjacent vertices of u (extracted vertex) and update their distance
      struct AdjListNode* pCrawl = graph->array[u].head;
      // std::cout << "extracted vertex number: " << u << "neighbor: " << pCrawl->dest
      //           << " the dist is: " << dist[u] << std::endl;
      while (pCrawl != NULL) {
        int v = pCrawl->dest;


        // If shortest distance to v is not finalized yet, and distance to v
        // through u is less than its previously calculated distance
        // std::cout << isInMinHeap(minHeap, v) << std::endl;
        // std::cout << ((pCrawl->weight + dist[u]) < dist[v]) << std::endl;
        if (isInMinHeap(minHeap, v) && dist[u] != 1000000 &&
            (pCrawl->weight + dist[u]) < dist[v]) {
            // std::cout << "yes" << std::endl;
          dist[v] = dist[u] + pCrawl->weight;
          // std::cout << "setting " << v << "'s prev to: " << u;
          prev[v] = u;
          // update distance value in min heap also
          decreaseKey(minHeap, v, dist[v]);
        }
        pCrawl = pCrawl->next;
      }
      // if (u == goal) {
      //   break;
      // }
    }

    // print the calculated shortest distances printArr(dist, prev, V, graph);
    return FindPath(goal, src, prev);
  }

  std::vector<Vector3> PathFinder::FindPath(int endNode, int startNode, int prev[]) {
    // std::cout << "FindPath" << std::endl;
    int iter = endNode;
    std::vector<Vector3> route_;
    strRoute_.clear();
    while (iter != startNode) {
      // std::cout << "looping " << iter << std::endl;
      route_.push_back(Vector3(nodes_[iter]->GetPosition()));
      strRoute_.push_back(nodes_[iter]->GetName());
      iter = prev[iter];
    }
    route_.push_back(Vector3(nodes_[iter]->GetPosition()));
    strRoute_.push_back(nodes_[iter]->GetName());
    return route_;
  }

  // Driver program to test above functions
  std::vector<Vector3> PathFinder::algorithm(std::string startNode, std::string endNode) {
    // std::cout << "algorithm" << std::endl;
    // create the graph of nodes
    int V = nodes_.size();
    struct Graph* graph = createGraph(V);
    for (int i = 0; i < V; i++) {
      entity_project::IGraphNode * node = nodes_[i];
      std::vector<entity_project::IGraphNode *> neighbors = node->GetNeighbors();
      for (int n = 0; n < neighbors.size(); n++) {
        float weight =
        Vector3(node->GetPosition()).DistanceBetween(Vector3(neighbors[n]->GetPosition()));
        addEdge(graph, GetIndex(node->GetName()), GetIndex(neighbors[n]->GetName()), weight);
      }
    }
    // the second argument will be the index of the starting node
    int startIndex = GetIndex(startNode);
    int endIndex = GetIndex(endNode);
    return dijkstra(graph, endIndex, startIndex);
  }

  void PathFinder::CreateMap() {
    // std::cout << "CreateMap" << std::endl;
    for (int i = 0; i < nodes_.size(); i++) {
      keys_[nodes_[i]->GetName()] = i;
    }
  }

}  // namespace csci3081
