#include "bits/stdc++.h"
using namespace std;

long long swap(long long curr, int pos, int remaining, bool repeat, vector<int> &digits, vector<vector<unordered_set<long long>>> &visited) {
    if (pos < 0) {
        if (repeat || !(remaining & 1)) return curr;
        else return curr - digits[0] - 10 * digits[1] + 10 * digits[0] + digits[1];
    }

    if (!remaining) return curr;
    if (visited[pos][remaining - 1].count(curr)) return -1;

    visited[pos][remaining - 1].emplace(curr);
    int p = pos;
    vector<int> indices;
    for (int i = pos - 1; ~i; i--)
        if (digits[p] < digits[i]) {
            p = i;
            indices = {p};
        } else if (digits[p] == digits[i]) indices.emplace_back(i);

    auto largest = -1LL;
    if (p == pos) largest = swap(curr, pos - 1, remaining, repeat, digits, visited);
    for (int i : indices) {
        auto next = curr - llround(pow(10, pos)) * digits[pos] - llround(pow(10, i)) * digits[i] + llround(pow(10, pos)) * digits[i] + llround(pow(10, i)) * digits[pos];
        swap(digits[pos], digits[i]);

        largest = max(largest, swap(next, pos - 1, remaining - 1, repeat, digits, visited));
        swap(digits[pos], digits[i]);
    }

    return largest;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    int rank = (int) log10((double) n) + 1;
    vector<int> digits(rank);
    vector<vector<unordered_set<long long>>> visited(rank, vector<unordered_set<long long>>(k));

    vector<bool> seen(10, false);
    int pos = 0;
    bool repeat = false;
    for (auto num = n; num; num /= 10) {
        digits[pos++] = num % 10;
        if (seen[num % 10]) repeat = true;
        seen[num % 10] = true;
    }

    cout << swap(n, pos - 1, k, repeat, digits, visited);
}
