#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

enum State {
    UNVISITED = 0,
    VISITED = 1,
    ONPATH = 2,
    LOOP = 3
};

void jump(int v, int D, vector<vector<tuple<int, int, int, int, bool>>> &adj_list) {
    int w = v;
    vector<int> states;
    while (w) {
        states.emplace_back(w % 6);
        w /= 6;
    }
    while (states.size() <= D) states.emplace_back(0);

    vector<int> path, unvisited;
    gp_hash_table<int, pair<int, int>> indices;
    for (int i = 0; i < states.size(); i++)
        if (states[i] == UNVISITED) unvisited.emplace_back(i);
        else if (states[i] == ONPATH) path.emplace_back(i);
        else if (states[i] >= LOOP) {
            if (indices.find(states[i]) != indices.end()) indices[states[i]].second = i;
            else indices[states[i]] = {i, -1};
        }

    auto hash = [](vector<int> states) {
        gp_hash_table<int, int> indices;
        vector<pair<int, int>> matches;
        for (int i = 0; i < states.size(); i++)
            if (states[i] >= LOOP) {
                if (indices.find(states[i]) != indices.end()) matches.emplace_back(indices[states[i]], i);
                else indices[states[i]] = i;
            }

        if (!matches.empty()) {
            sort(matches.begin(), matches.end());
            int s = matches.size() + 2;
            for (auto [i, j] : matches) states[i] = states[j] = s--;
        }

        int v = 0;
        for (int i = states.size() - 1; ~i; i--) v = v * 6 + states[i];

        return v;
    };

    vector<int> temp;
    for (int i : path)
        for (int j : unvisited) {
            temp = states;
            temp[i] = VISITED;
            temp[j] = ONPATH;
            adj_list[v].emplace_back(hash(temp), 1, min(i, j), max(i, j), false);
        }

    if (indices.size() < 2)
        for (int k = 0; k < unvisited.size(); k++) {
            int i = unvisited[k];
            for (int l = k + 1; l < unvisited.size(); l++) {
                int j = unvisited[l];
                for (int s = LOOP; s < 6; ++s)
                    if (indices.find(s) == indices.end()) {
                        temp = states;
                        temp[i] = temp[j] = s;
                        adj_list[v].emplace_back(hash(temp), 2, i, j, false);
                        break;
                    }
            }
        }

    for (auto [s1, p1] : indices)
        for (auto [s2, p2] : indices) {
            if (s1 == s2) continue;
            for (int k = 0; k < 2; k++)
                for (int l = 0; l < 2; l++) {
                    auto [i1, j1] = !k ? p1 : make_pair(p1.second, p1.first);
                    auto [i2, j2] = !l ? p2 : make_pair(p2.second, p2.first);

                    temp = states;
                    temp[i1] = temp[i2] = VISITED;
                    temp[j1] = temp[j2] = s1;
                    adj_list[v].emplace_back(hash(temp), 0, min(i1, i2), max(i1, i2), false);
                }
        }

    for (int i : path)
        for (auto [s, p] : indices)
            for (int l = 0; l < 2; l++) {
                auto [j, k] = !l ? p : make_pair(p.second, p.first);

                temp = states;
                temp[i] = temp[j] = VISITED;
                temp[k] = ONPATH;
                adj_list[v].emplace_back(hash(temp), 0, min(i, j), max(i, j), false);
            }

    if (path.size() < 2)
        for (int i : unvisited) {
            temp = states;
            temp[i] = ONPATH;
            adj_list[v].emplace_back(hash(temp), 1, -1, -1, false);
        }

    if (path.size() == 2) {
        temp = states;
        temp[path[0]] = temp[path[1]] = VISITED;

        bool end = true;
        int h = hash(temp);
        while (h) {
            if (h % 6 > 1) {
                end = false;
                break;
            }

            h /= 6;
        }

        if (end) adj_list[v].emplace_back(h, 0, path[0], path[1], true);
    }

    for (int i : unvisited)
        for (auto [s, p] : indices)
            for (int k = 0; k < 2; k++) {
                int j = !k ? p.first : p.second;

                temp = states;
                temp[i] = s;
                temp[j] = VISITED;
                adj_list[v].emplace_back(hash(temp), 1, min(i, j), max(i, j), false);
            }

    sort(adj_list[v].begin(), adj_list[v].end());
    adj_list[v].erase(unique(adj_list[v].begin(), adj_list[v].end()), adj_list[v].end());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, D, M;
    cin >> n >> D >> M;

    vector<int> arr(n);
    for (int &e : arr) cin >> e;

    D = min(D, n - 1);
    vector<vector<tuple<int, int, int, int, bool>>> adj_list(2e6);
    vector<bool> visited(2e6, false);
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        jump(v, D, adj_list);
        for (auto [u, jumps, src, dest, end] : adj_list[v])
            if (!visited[u]) {
                visited[u] = true;
                q.emplace(u);
            }
    }

    vector<vector<int>> dp(2, vector<int>(2e6, 0));
    vector<int> curr(1e5), prev(1e5);
    fill(visited.begin(), visited.end(), false);
    int len = INT_MIN;
    for (int i = 0, c = 0, p = 0; i + D < n; i++) {
        for (int j = 0; j < c; j++) dp[i & 1][curr[j]] = 0;
        for (int j = 0; j < p; j++) visited[prev[j]] = false;

        q.emplace(0);
        if (i)
            for (int j = 0; j < p; j++) {
                int v = prev[j];
                if (v % 6 > 1) continue;

                int u = v / 6;
                dp[i & 1][u] = max(dp[i & 1][u], dp[(i + 1) & 1][v]);

                if (!visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
            }

        c = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            curr[c++] = v;
            for (auto [u, jumps, src, dest, end] : adj_list[v])
                if (abs(arr[i + src] - arr[i + dest]) <= M) {
                    if (end) {
                        len = max(len, dp[i & 1][v]);
                        continue;
                    }

                    dp[i & 1][u] = max(dp[i & 1][u], dp[i & 1][v] + jumps);
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
                }
        }

        swap(curr, prev);
        swap(c, p);
    }

    cout << len;
}
