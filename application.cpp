#include "application.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip> /*setprecision*/
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class prioritize {
   public:
    bool operator()(const pair<long long, double>& p1,
                    const pair<long long, double>& p2) const {
        return p1.second > p2.second;
    }
};

double INF = numeric_limits<double>::max();

graph<long long, double> buildGraph(
    const map<long long, Coordinates>& Nodes,
    const vector<FootwayInfo>& Footways,
    const vector<BuildingInfo>& Buildings) {
    graph<long long, double> G;

    // Add nodes to the graph
    for (const auto& node : Nodes) {
        G.addVertex(node.first);
    }

    // Iterate over each footway
    for (const auto& footway : Footways) {
        const vector<long long>& nodes = footway.Nodes;
        
        // Iterate over each node in the footway
        for (size_t i = 0; i < nodes.size() - 1; ++i) {
            long long node1 = nodes[i];
            long long node2 = nodes[i + 1];
            
            // Calculate the distance between the two nodes
            double distance = distBetween2Points(
                Nodes.at(node1).Lat, Nodes.at(node1).Lon,
                Nodes.at(node2).Lat, Nodes.at(node2).Lon);
            
            // Add an edge between the two nodes in the graph
            G.addEdge(node1, node2, distance);
            G.addEdge(node2, node1, distance); // Assuming undirected graph
        }
    }

    // Add buildings as vertices and create edges between buildings and nearby footway nodes
    for (const auto& building : Buildings) {
        long long buildingNodeID = building.Coords.ID;
        G.addVertex(buildingNodeID); // Add building as a vertex

        // Iterate over each node in the footway
        for (const auto& node : Nodes) {
            if(!node.second.OnFootway) continue;
            
            // Calculate the distance between the building and the footway node
            double distance = distBetween2Points(
                building.Coords.Lat, building.Coords.Lon,
                node.second.Lat, node.second.Lon);
            
            // If the distance is within the threshold, add an edge between the building and the footway node
            if (distance < 0.041) {
                G.addEdge(buildingNodeID, node.second.ID, distance);
                G.addEdge(node.second.ID, buildingNodeID, distance); // Assuming undirected graph
            }
        }
    }

    return G;
}

vector<long long> dijkstra(
    const graph<long long, double>& G,
    long long start,
    long long target,
    const set<long long>& ignoreNodes) {

    vector<long long> path;
    unordered_map<long long, long long> previous;
    unordered_map<long long, double> dist;

    priority_queue<pair<long long, double>,
                   vector<pair<long long, double>>,
                   prioritize>
        worklist;

    // Initialize distances to infinity
    for (const auto& vertex : G.getVertices()) {
        dist[vertex] = INF;
    }

    dist[start] = 0.0;
    worklist.push({start, 0.0});

    while (!worklist.empty()) {
        auto [u, d] = worklist.top();
        worklist.pop();

        if (d > dist[u] || (ignoreNodes.count(u) && u != start && u != target)) continue;

        for (auto v : G.neighbors(u)) {
            double w;
            if (G.getWeight(u, v, w) && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                previous[v] = u; // Track the previous node
                worklist.push({v, dist[v]});
            }
        }
    }

    if (dist[target] < INF) {
        // Reconstruct the path from the 'previous' map
        long long curr = target;
        while (curr != start) {
            path.push_back(curr);
            curr = previous[curr];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }

    return path;
}

double pathLength(const graph<long long, double>& G, const vector<long long>& path) {
    double length = 0.0;
    double weight;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
        assert(res);
        length += weight;
    }
    return length;
}

void outputPath(const vector<long long>& path) {
    for (size_t i = 0; i < path.size(); i++) {
        cout << path.at(i);
        if (i != path.size() - 1) {
            cout << "->";
        }
    }
    cout << endl;
}

void application(
    const vector<BuildingInfo>& Buildings,
    const graph<long long, double>& G) {
    string person1Building, person2Building;

    set<long long> buildingNodes;
    for (const auto& building : Buildings) {
        buildingNodes.insert(building.Coords.ID);
    }

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    while (person1Building != "#") {
        cout << "Enter person 2's building (partial name or abbreviation)> ";
        getline(cin, person2Building);

        //
        // find the building coordinates
        //
        bool foundP1 = false;
        bool foundP2 = false;
        Coordinates P1Coords, P2Coords;
        string P1Name, P2Name;

        for (const BuildingInfo& building : Buildings) {
            if (building.Abbrev == person1Building) {
                foundP1 = true;
                P1Name = building.Fullname;
                P1Coords = building.Coords;
            }
            if (building.Abbrev == person2Building) {
                foundP2 = true;
                P2Name = building.Fullname;
                P2Coords = building.Coords;
            }
        }

        for (const BuildingInfo& building : Buildings) {
            if (!foundP1 &&
                building.Fullname.find(person1Building) != string::npos) {
                foundP1 = true;
                P1Name = building.Fullname;
                P1Coords = building.Coords;
            }
            if (!foundP2 && building.Fullname.find(person2Building) != string::npos) {
                foundP2 = true;
                P2Name = building.Fullname;
                P2Coords = building.Coords;
            }
        }

        if (!foundP1) {
            cout << "Person 1's building not found" << endl;
        } else if (!foundP2) {
            cout << "Person 2's building not found" << endl;
        } else {
            cout << endl;
            cout << "Person 1's point:" << endl;
            cout << " " << P1Name << endl;
            cout << " (" << P1Coords.Lat << ", " << P1Coords.Lon << ")" << endl;
            cout << "Person 2's point:" << endl;
            cout << " " << P2Name << endl;
            cout << " (" << P2Coords.Lat << ", " << P2Coords.Lon << ")" << endl;

            string destName;
            Coordinates destCoords;

            Coordinates centerCoords = centerBetween2Points(
                P1Coords.Lat, P1Coords.Lon, P2Coords.Lat, P2Coords.Lon);

            double minDestDist = numeric_limits<double>::max();

            for (const BuildingInfo& building : Buildings) {
                double dist = distBetween2Points(
                    centerCoords.Lat, centerCoords.Lon,
                    building.Coords.Lat, building.Coords.Lon);
                if (dist < minDestDist) {
                    minDestDist = dist;
                    destCoords = building.Coords;
                    destName = building.Fullname;
                }
            }

            cout << "Destination Building:" << endl;
            cout << " " << destName << endl;
            cout << " (" << destCoords.Lat << ", " << destCoords.Lon << ")" << endl;

            vector<long long> P1Path = dijkstra(G, P1Coords.ID, destCoords.ID, buildingNodes);
            vector<long long> P2Path = dijkstra(G, P2Coords.ID, destCoords.ID, buildingNodes);

            // This should NEVER happen with how the graph is built
            if (P1Path.empty() || P2Path.empty()) {
                cout << endl;
                cout << "At least one person was unable to reach the destination building. Is an edge missing?" << endl;
                cout << endl;
            } else {
                cout << endl;
                cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
                cout << " miles" << endl;
                cout << "Path: ";
                outputPath(P1Path);
                cout << endl;
                cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
                cout << " miles" << endl;
                cout << "Path: ";
                outputPath(P2Path);
            }
        }

        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter person 1's building (partial name or abbreviation), or #> ";
        getline(cin, person1Building);
    }
}
