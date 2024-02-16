#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 10567201;

long long pow(long long base, long long exponent, long long mod = 1) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

pair<long long, long long> bezout(int a, int b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, v;
    while (cin >> n >> m >> v && n && m) {
        auto p2 = 1LL;
        while (n--) p2 = (p2 << 1) % MODULO;

        vector<long long> c(m + 1, 1), ways(m + 1, 1);
        for (int i = 1; i <= m; i++)
            c[i] = ((c[i - 1] * (p2 - i + 1)) % MODULO * pow(i, MODULO - 2, MODULO)) % MODULO;

        if (v) ways[0] = 0;
        for (int i = 2; i <= m; i++) {
            auto temp = ((__int128) ways[i - 2] * (p2 - i + 2 + MODULO)) % MODULO;
            temp = (c[i - 1] - temp + MODULO) % MODULO;

            auto [x, y] = bezout(i, MODULO);
            if (x < 0) x += MODULO;
            ways[i] = (temp * x) % MODULO;
        }

        cout << ways[m] << "\n";
    }
}
