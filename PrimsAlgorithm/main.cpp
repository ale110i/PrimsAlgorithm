//
//  main.cpp
//  PrimsAlgorithm
//
//  Created by Alexey Ischenko on 11/12/2020.
//  Copyright © 2019 Enthropy. All rights reserved.
//

#include <iostream>
#include <vector>


class PriorityQueue {
private:
    std::vector<std::pair<int, int>> queue;
    std::vector<int> indexOfEl;
    std::vector<int> isVertexInQueue;
    int curSize;
    
public:
    PriorityQueue (const int& amountOfElements);
    //~PriorityQueue();
    void SiftDown (int i);
    void SiftUp (int i);
    int GetSize ();
    void Push (const int& vertex, const int& weight);
    void ChangePriority (const int& vertex, const int& weight);
    bool IsVertexInPriorQueue (const int& vertex);
    std::pair<int, int> ExtractMin();
};

PriorityQueue::PriorityQueue(const int& amountOfElements) {
    queue.reserve(amountOfElements);
    indexOfEl.reserve(amountOfElements);
    isVertexInQueue.reserve(amountOfElements);
    curSize = 0;
}

bool PriorityQueue::IsVertexInPriorQueue(const int &vertex) {
    return (isVertexInQueue[vertex] == 1);
}

void PriorityQueue::Push(const int& vertex, const int& weight) {
    queue[curSize] = std::make_pair(vertex, weight);
    indexOfEl[vertex] = curSize;
    isVertexInQueue[vertex] = 1;
    SiftUp(curSize++);
}

int PriorityQueue::GetSize() {
    return curSize;
}

std::pair<int, int> PriorityQueue::ExtractMin() {
    isVertexInQueue[queue[0].first] = 0;
    std::swap(queue[0], queue[--curSize]);
    std::swap(indexOfEl[queue[0].first], indexOfEl[queue[curSize].first]);
    SiftUp(0);
    return (queue[curSize]);
}

void PriorityQueue::SiftDown(int i) {
    int least = i;
    int leftSon = 2 * i + 1;
    int rightSon = 2 * i + 2;
    while ((2 * least + 1 < curSize && queue[least].second > queue[2 * least + 1].second) ||
           (2 * least + 2 < curSize && queue[least].second > queue[2 * least + 2].second)) {
        if (leftSon < curSize && queue[least].second > queue[leftSon].second) {
            least = leftSon;
        }
        if (rightSon < curSize && queue[least].second > queue[rightSon].second) {
            least = rightSon;
        }
        if (least != i) {
            std::swap(queue[i], queue[least]);
            std::swap(indexOfEl[queue[i].first], queue[least].first);
        }
    }
}

void PriorityQueue::SiftUp(int i) {
    while (i > 0 && queue[i].second < queue[(i - 1) / 2].second) {
        std::swap(queue[i], queue[(i - 1) / 2]);
        std::swap(indexOfEl[queue[i].first], indexOfEl[queue[(i - 1) / 2].first]);
        i = (i - 1) / 2;
    }
}

void PriorityQueue::ChangePriority(const int& vertex, const int& weight) {
    if (queue[indexOfEl[vertex]].second > weight) {
        queue[indexOfEl[vertex]].second = weight;
        if (queue[indexOfEl[vertex]].second < queue[(indexOfEl[vertex] - 1) / 2].second) {
            SiftUp(indexOfEl[vertex]);
        } else {
            SiftDown(indexOfEl[vertex]);
        }
    }
}

class ListGraph {
private:
    std::vector<std::vector<std::pair<int, int>>> graph;
    
public:
    ListGraph (int amountOfVertices);
    int VerticesCount();
    void AddEdge (int from, int to, int weight);
    void GetNextVertices (int vertex, std::vector<std::pair<int, int>>& nextVertices);
    int MinOstWeight(int startVertex, int amountOfVertices);
};

ListGraph::ListGraph(int amountOfVertices) {
    graph.resize(amountOfVertices);
}

int ListGraph::VerticesCount() {
    return (int)graph.size();
}

void ListGraph::AddEdge(int from, int to, int weight) {
    graph[from].push_back({to, weight});
    graph[to].push_back({from, weight});
}

void ListGraph::GetNextVertices(int vertex, std::vector<std::pair<int, int>> &nextVertices) {
    nextVertices = graph[vertex];
}

int ListGraph::MinOstWeight(int startVertex, int amountOfVertices) {
    PriorityQueue queue(amountOfVertices);
    std::vector<int> visitedVertices(1, startVertex);
    std::vector<int> isVisited(amountOfVertices, 0);
    isVisited[startVertex] = 1;
    std::vector<std::pair<int, int>> nextVertices;
    int minweight = 0;
    int vertex = startVertex;
    std::pair<int, int> minEdge = {startVertex, 0};
    while (visitedVertices.size() != graph.size()) {
        GetNextVertices(vertex, nextVertices);
        for (auto i : nextVertices) {
            if (isVisited[i.first] == 0) {
                if (queue.IsVertexInPriorQueue(i.first)) {
                    queue.ChangePriority(i.first, i.second);
                } else {
                    queue.Push(i.first, i.second);
                }
            }
        }
        minEdge = queue.ExtractMin();
        minweight += minEdge.second;
        visitedVertices.push_back(minEdge.first);
        isVisited[minEdge.first] = 1;
        vertex = minEdge.first;
    }
    return minweight;
}


int main() {
    int amountOfVertices, amountOfEdges;
    std::cin >> amountOfVertices >> amountOfEdges;
    ListGraph graph(amountOfVertices);
    for (int i = 0; i < amountOfEdges; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }
    std::cout << graph.MinOstWeight(0, amountOfVertices);
    return 0;
}
