#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

enum NodeColor {
    WHITE,
    GRAY,
    BLACK
};

int n, m;
vector<vector<int>> graph;
vector<NodeColor> color;
vector<vector<int>> treeEdges;
vector<int> backEdgeWithHighEndpointIn;
vector<int> backEdgeWithLowEndpointIn;
vector<int> memo;
vector<bool> visited;
vector<long long> amountOfCoveredNodes;
long long bridgeComponentsCount;
long long waysToWin;

void dfs (int v, int p = -1) {
    color[v] = GRAY;
    for (auto u: graph[v]) {
        if (color[u] == WHITE) {
            treeEdges[v].push_back(u);
            dfs(u, v);
        } else if (u != p and color[u] == GRAY) {
            backEdgeWithHighEndpointIn[u]++;
            backEdgeWithLowEndpointIn[v]++;
        }
    }
    color[v] = BLACK;
}


int amountOfBackEdgesCovering (int v, int p = -1) {
    if (memo[v] != -1) return memo[v];
    int res = 0;
    res += backEdgeWithLowEndpointIn[v];
    res -= backEdgeWithHighEndpointIn[v];
    for (auto u: treeEdges[v]) {
        if (u != p) res += amountOfBackEdgesCovering(u, v);
    }

    memo[v] = res;
    return res;
}

void searchBridgeComponent (int v, int p = -1) {
    visited[v] = true;
    amountOfCoveredNodes[bridgeComponentsCount]++;
    for (auto u: graph[v]) {
        if (!visited[u] and amountOfBackEdgesCovering(u) > 0)
            searchBridgeComponent(u, v);
    }
}

void findBackEdges() {
    for (int i = 1; i <= n; i++) {
        if (color[i] == WHITE) {
            dfs(i);
        }
    }
}

void bridgeComponentsSize() {
    bridgeComponentsCount = 0;
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            bridgeComponentsCount++;
            searchBridgeComponent(i);
        }
    }

}

void findWaysToWin() {
    for (int i = 1; i <= bridgeComponentsCount; i++) {
        long long amount = amountOfCoveredNodes[i];
        if (amount > 0) {
            waysToWin += (long long)(amount * (amount-1)) / 2;
        }
    }
}


void getProbabilityToLoose () {
    findBackEdges();
    bridgeComponentsSize();
    findWaysToWin();

    double waysToPlay = (double)n * (double)(n - 1) / 2;
    double result = 1.0 - (double)(waysToWin) / waysToPlay;
    cout << fixed << setprecision(5) << result;
}

int main(){
    cin >> n >> m;
    graph.resize(n+1, {});
    color.resize(n+1, WHITE);
    treeEdges.resize(n+1, {});
    backEdgeWithLowEndpointIn.resize(n+1, 0);
    backEdgeWithHighEndpointIn.resize(n+1, 0);
    visited.resize(n+1, false);
    memo.resize(n+1, -1);
    amountOfCoveredNodes.resize(n+1, 0);
    waysToWin = 0;

    while (m--) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    getProbabilityToLoose();
}
