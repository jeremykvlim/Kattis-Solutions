#include <bits/stdc++.h>
using namespace std;

void radix_sort(vector<long long> &s) {
    long long radix = 1 << 16, biggest = *max_element(s.begin(), s.end());
    auto msd = (long long) ceil((log(biggest)) / (log(radix)));

    vector<int> bucket(radix, 0), r(s.size());
    for (auto d = 0LL; d < msd; d++) {
        fill(bucket.begin(), bucket.end(), 0);
        for (auto i : s) bucket[(i >> (d * 16)) & (radix - 1)]++;
        for (int i = 1; i < radix; i++) bucket[i] += bucket[i - 1];
        for (int i = r.size() - 1; ~i; i--) r[--bucket[(s[i] >> (d * 16)) & (radix - 1)]] = s[i];
        copy(r.begin(), r.end(), s.begin());
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    while (tc--) {
        long long n, a, b, c, x, y;
        cin >> n >> a >> b >> c >> x >> y;

        vector<long long> s(n);
        s[0] = a;
        for (int i = 1; i < n; i++) s[i] = (s[i - 1] * b + a) % c;
        radix_sort(s);

        auto v = 0LL;
        for (auto r : s) v = (v * x + r) % y;
        cout << v << "\n";
    }
}
