#include <bits/stdc++.h>
using namespace std;

long long dfs(vector<vector<int>> &adj_list, vector<unordered_set<int>> &magic, vector<int> &board, int root, int v) {
    bool inserted = magic[root].emplace(board[v]).second;

    long long wins = magic[root].size();
    for (int u : adj_list[v])
        wins += dfs(adj_list, magic, board, root, u);

    if (inserted) magic[root].erase(board[v]);
    return wins;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> board(n + 1), root(n + 1);
    vector<unordered_set<int>> magic(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> board[i];

        if (i + board[i] < 1 || i + board[i] > n) {
            root[i] = i;
            magic[i].emplace(board[i]);
        }
    }

    vector<bool> visited(n + 1);
    for (int i = 1; i <= n; i++) {
        if (visited[i]) continue;

        int v = i;
        vector<int> hops;
        while (1 <= v && v <= n && !visited[v]) {
            visited[v] = true;
            hops.emplace_back(v);
            v += board[v];
        }

        if (1 <= v && v <= n) {
            reverse(hops.begin(), hops.end());
            int len = find(hops.begin(), hops.end(), v) - hops.begin();
            if (len < hops.size())
                for (int j = 0; j <= len; j++) {
                    root[hops[j]] = hops.front();
                    magic[hops.front()].emplace(board[hops[j]]);
                }
        }
    }

    vector<vector<int>> adj_list(n + 1);
    for (int v = 1; v <= n; v++)
        if (!root[v]) adj_list[v + board[v]].emplace_back(v);

    auto wins = 0LL;
    for (int v = 1; v <= n; v++)
        if (root[v]) wins += dfs(adj_list, magic, board, root[v], v);

    cout << wins;
}
