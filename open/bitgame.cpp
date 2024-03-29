#include <bits/stdc++.h>
using namespace std;

bool dfs(string &paper) {
    if (paper.size() == 20) {
        bool num;
        cout << "? " << stoi(paper, nullptr, 2) << "\n" << flush;
        cin >> num;

        return num;
    }

    string bits{"01"};
    if (rand() & 1) swap(bits[0], bits[1]);

    for (char b : bits) {
        paper += b;
        bool win = !dfs(paper);
        paper.pop_back();

        if (win) return true;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    srand(time(nullptr));
    string s;
    cout << (dfs(s) ? "! alice" : "! bob");
}
