#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

const int INF = 10000000;
vector<vector<pair<int, int>>> graph;
vector<vector<pair<int, int>>> reverseGraph;
vector<pair<pair<int, int>, int>> streetsToAdd;

vector<int> dijkstra(vector<vector<pair<int, int>>>& G, int v) {
    int n = G.size() - 1;
    vector<int> distance(n + 1, INF);
    vector<int> pred(n + 1);
    priority_queue<pair<int, int>> q;
    vector<bool> processed(n + 1, false);

    distance[v] = 0;
    q.emplace(0, v);
    pred[v] = 0;

    while (!q.empty()) {
        int a = q.top().second;
        q.pop();
        if (processed[a]) continue;
        processed[a] = true;

        for (auto u : G[a]) {
            int b = u.first, w = u.second;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                q.emplace(-distance[b], b);
                pred[b] = a;
            }
        }
    }

    return distance;
}

int main() {
    double totalDuration = 0.0;
    int numCases;

    cin >> numCases;
    int total = numCases;

    while (numCases--) {
        int n, m, k, s, t;
        cin >> n >> m >> k >> s >> t;
        graph.clear();
        reverseGraph.clear();
        streetsToAdd.clear();
        graph.resize(n + 1, {});
        reverseGraph.resize(n + 1, {});

        while (m--) {
            int x, y, w;
            cin >> x >> y >> w;
            graph[x].emplace_back(y, w);
            reverseGraph[y].emplace_back(x, w);
        }

        while (k--) {
            int x, y, w;
            cin >> x >> y >> w;
            streetsToAdd.emplace_back(make_pair(x, y), w);
        }

        vector<int> sDistances = dijkstra(graph, s);
        vector<int> tDistances = dijkstra(reverseGraph, t);

        int res = INF;

        for (auto& street : streetsToAdd) {
            pair<int, int> edge = street.first;
            int weight = street.second;
            int path1 = sDistances[edge.first] + tDistances[edge.second];
            int path2 = sDistances[edge.second] + tDistances[edge.first];
            int temp = weight + min(path1, path2);
            res = min(res, min(temp, sDistances[t]));
        }

        if (res == INF) res = -1;
        cout << res << endl;
    }

    double average_duration = totalDuration / total;
    return 0;
}
