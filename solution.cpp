#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3005;

class MaxFlowSolver {
private:
    int n;
    vector<vector<int>> adj;
    vector<int> parent;

    int bfs(int source, int sink, vector<vector<int>>& capacity) {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = source;
        queue<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty()) {
            int u = q.front().first;
            int flow = q.front().second;
            q.pop();

            if (u == sink) return flow;

            for (int v : adj[u]) {
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    int min_cap = min(flow, capacity[u][v]);
                    q.push({v, min_cap});
                }
            }
        }
        return 0;
    }

public:
    MaxFlowSolver(int N) : n(N) {
        adj.resize(n);
        parent.resize(n);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<vector<int>> createCapacityMatrix() {
        return vector<vector<int>>(n, vector<int>(n, 0));
    }

    void addEdgeCapacity(vector<vector<int>>& capacity, int u, int v, int cap) {
        capacity[u][v] += cap;
        capacity[v][u] += cap;
    }

    int maxFlow(int source, int sink, vector<vector<int>>& capacity) {
        int flow = 0;
        int new_flow;

        // Since each node has degree at most 3, max flow between any
        // two nodes <= 3. Early termination optimization.
        while ((new_flow = bfs(source, sink, capacity)) > 0) {
            flow += new_flow;

            int cur = sink;
            while (cur != source) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }

            if (flow >= 3) break;
        }
        return flow;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    MaxFlowSolver solver(n);
    vector<vector<int>> original = solver.createCapacityMatrix();

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        edges.push_back({a, b});
        solver.addEdge(a, b);
        solver.addEdgeCapacity(original, a, b, 1);
    }

    long long total = 0;

    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            vector<vector<int>> capacity = original;
            int flow = solver.maxFlow(a, b, capacity);
            total += flow;
        }
    }

    cout << total << endl;

    return 0;
}