#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3005;
const int MAXM = 4505;

struct Edge {
    int to, cap, rev;
};

class MaxFlow {
private:
    vector<Edge> graph[MAXN];
    int level[MAXN];
    int iter[MAXN];

    void bfs(int s) {
        memset(level, -1, sizeof(level));
        queue<int> que;
        level[s] = 0;
        que.push(s);

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            for (size_t i = 0; i < graph[v].size(); i++) {
                Edge &e = graph[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;

        for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    void init(int n) {
        for (int i = 0; i < n; i++) {
            graph[i].clear();
        }
    }

    void addEdge(int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
        graph[from].push_back({to, 0, (int)graph[to].size()});
        graph[to].push_back({from, cap, (int)graph[from].size() - 1});
    }

    int maxFlow(int s, int t) {
        const int INF = 1e9;
        int flow = 0;

        while (true) {
            bfs(s);
            if (level[t] < 0) break;

            memset(iter, 0, sizeof(iter));
            int f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
        return flow;
    }

    void reset() {
        for (int i = 0; i < MAXN; i++) {
            for (size_t j = 0; j < graph[i].size(); j++) {
                if (j % 2 == 0) {
                    graph[i][j].cap = 1;
                } else {
                    graph[i][j].cap = 0;
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    MaxFlow mf;
    mf.init(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        edges.push_back({a, b});
        mf.addEdge(a, b, 1);
    }

    long long total = 0;

    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            mf.reset();
            int flow = mf.maxFlow(a, b);
            total += flow;
        }
    }

    cout << total << endl;

    return 0;
}