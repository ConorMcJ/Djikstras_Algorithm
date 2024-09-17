#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
   private:
    unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjacencyList;

   public:
    /// Default constructor
    graph() {}

    /// @brief Add the vertex `v` to the graph, must run in at most O(log |V|).
    /// @param v
    /// @return true if successfully added; false if it existed already
    bool addVertex(VertexT v) {
        // If the vertex doesn't exist, add it to the graph
        if (adjacencyList.find(v) == adjacencyList.end()) {
            adjacencyList.insert({v, unordered_map<VertexT, WeightT>()});
            return true;
        }
        return false;
    }

    /// @brief Add or overwrite directed edge in the graph, must run in at most O(log |V|).
    /// @param from starting vertex
    /// @param to ending vertex
    /// @param weight edge weight / label
    /// @return true if successfully added or overwritten;
    ///         false if either vertices isn't in graph
    bool addEdge(VertexT from, VertexT to, WeightT weight) {
        // Ensure both vertices exist before adding the edge
        if (adjacencyList.find(from) != adjacencyList.end() 
                && adjacencyList.find(to) != adjacencyList.end()) {
            // Add or update the edge from 'from' to 'to'
            adjacencyList.at(from)[to] = weight;
            return true;
        }
        return false;
    }

    /// @brief Maybe get the weight associated with a given edge, must run in at most O(log |V|).
    /// @param from starting vertex
    /// @param to ending vertex
    /// @param weight output parameter
    /// @return true if the edge exists, and `weight` is set;
    ///         false if the edge does not exist
    bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
        // Check if the edge exists and set the weight if found
        if (adjacencyList.find(from) != adjacencyList.end()
                && adjacencyList.at(from).find(to) != adjacencyList.at(from).end()) {
            weight = adjacencyList.at(from).at(to);
            return true;
        }
        return false;
    }

    /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
    /// @param v
    /// @return vertices that v has an edge to
    set<VertexT> neighbors(VertexT v) const {
        set<VertexT> S;
        // Retrieve the neighbors of vertex 'v' from the adjacency list
        if (adjacencyList.find(v) != adjacencyList.end()) {
            for (const auto& edge : adjacencyList.at(v)) {
                S.insert(edge.first);
            }
        }
        return S;
    }

    /// @brief Return a vector containing all vertices in the graph
    vector<VertexT> getVertices() const {
        vector<VertexT> vertices;
        // Extract all vertices from the adjacency list
        for (const auto& pair : adjacencyList) {
            vertices.push_back(pair.first);
        }
        return vertices;
    }

    /// @brief Get the number of vertices in the graph. Runs in O(1).
    size_t NumVertices() const {
        return adjacencyList.size();
    }

    /// @brief Get the number of directed edges in the graph.
    /// Runs in at most O(|V|) because it counts each edge once.
    size_t NumEdges() const {
        size_t count = 0;
        // Count the total number of edges by summing the size of each adjacency list
        for (const auto& pair : adjacencyList) {
            count += pair.second.size();
        }
        return count;
    }
};
