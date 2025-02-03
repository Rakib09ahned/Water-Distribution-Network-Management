#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
    int from, to, capacity, flow;
};

class MaxFlow {
private:
    int numNodes;
    vector<vector<int>> adjMatrix;
    vector<Edge> edges;

    bool bfs(vector<int>& parent, int source, int sink) {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;

        queue<pair<int, int>> q;
        q.push({source, numeric_limits<int>::max()});

        while (!q.empty()) {
            int currentNode = q.front().first;
            int currentFlow = q.front().second;
            q.pop();

            for (int nextNode : adjMatrix[currentNode]) {
                Edge& edge = edges[nextNode];
                if (parent[edge.to] == -1 && edge.capacity > edge.flow) {
                    parent[edge.to] = nextNode;
                    int newFlow = min(currentFlow, edge.capacity - edge.flow);
                    if (edge.to == sink) {
                        return true;
                    }
                    q.push({edge.to, newFlow});
                }
            }
        }

        return false;
    }

public:
    MaxFlow(int n) : numNodes(n) {
        adjMatrix.resize(numNodes);
    }

    void addEdge(int from, int to, int capacity) {
        adjMatrix[from].push_back(edges.size());
        edges.push_back({from, to, capacity, 0});
        adjMatrix[to].push_back(edges.size());
        edges.push_back({to, from, 0, 0});
    }

    int findMaxFlow(int source, int sink) {
        vector<int> parent(edges.size());
        int maxFlow = 0;

        while (bfs(parent, source, sink)) {
            int pathFlow = numeric_limits<int>::max();
            for (int node = sink; node != source; node = edges[parent[node]].from) {
                int edgeIdx = parent[node];
                pathFlow = min(pathFlow, edges[edgeIdx].capacity - edges[edgeIdx].flow);
            }

            for (int node = sink; node != source; node = edges[parent[node]].from) {
                int edgeIdx = parent[node];
                edges[edgeIdx].flow += pathFlow;
                edges[edgeIdx ^ 1].flow -= pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};

int main() {
    int numNodes, numPipes;
    cout << "Enter the number of nodes in the water distribution network: ";
    cin >> numNodes;
    cout << "Enter the number of pipes in the network: ";
    cin >> numPipes;

    MaxFlow network(numNodes);

    cout << "Enter the connections (from to capacity):\n";
    for (int i = 0; i < numPipes; ++i) {
        int from, to, capacity;
        cin >> from >> to >> capacity;
        network.addEdge(from, to, capacity);
    }

    int source, sink;
    cout << "Enter the source node: ";
    cin >> source;
    cout << "Enter the sink node: ";
    cin >> sink;

    int maxFlow = network.findMaxFlow(source, sink);
    cout << "Maximum flow in the water distribution network: " << maxFlow << endl;

    return 0;
}
