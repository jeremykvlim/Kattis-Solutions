#include <bits/stdc++.h>
using namespace std;

void update(int i, long long value, vector<long long> &fenwick) {
    for (; i < fenwick.size(); i += i & -i) fenwick[i] += value;
}

long long pref_sum(int i, vector<long long> &fenwick) {
    auto sum = 0LL;
    for (; i; i &= (i - 1)) sum += fenwick[i];
    return sum;
}

tuple<long long, long long, long long> operator +(tuple<long long, long long, long long> t1, tuple<long long, long long, long long> t2) {
    auto [i1, j1, k1] = t1;
    auto [i2, j2, k2] = t2;
    return {i1 + i2, j1 + j2, i1 * j2 + k1 + k2};
}

tuple<long long, long long, long long> operator *(tuple<long long, long long, long long> t, long long v) {
    auto [i, j, k] = t;
    return {i * v, j * v, i * j * (v * (v - 1) / 2) + k * v};
}

tuple<long long, long long, long long> calc(long long range, long long a, long long m, long long b, tuple<long long, long long, long long> t1, tuple<long long, long long, long long> t2) {
    if (!range) return {0, 0, 0};

    if (m >= b) return calc(range, a, m % b, b, t1, t2) + t2 * (m / b);
    if (a >= b) return calc(range, a % b, m, b, t1, t1 * (a / b) + t2);

    auto r = (a * range + m) / b;
    if (!r) return t2 * range;

    return t2 * ((b - m - 1) / a) + t1 + calc(r - 1, b, (b - m - 1) % a, a, t2, t1) + t2 * (range - (b * r - m - 1) / a);
}

long long sum(int l, int r, int a, int b) {
    int m = ((long long) a * ((l - 1) % b) + b) % b;
    auto [i, j, k] = calc(r - l + 1, a, m, b, {-b, 0, 0}, {a, 1, a});
    return m * j + k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<tuple<int, int, int, int>> queries(q);
    vector<int> boxes;
    for (auto &[l, r, a, b] : queries) {
        int c;
        cin >> c >> l >> r;

        if (c == 1) cin >> a >> b;
        else {
            boxes.emplace_back(l - 1);
            boxes.emplace_back(r);
        }
    }
    sort(boxes.begin(), boxes.end());
    boxes.erase(unique(boxes.begin(), boxes.end()), boxes.end());

    auto index = [&](int v) -> int {
        return lower_bound(boxes.begin(), boxes.end(), v) - boxes.begin() + 1;
    };

    vector<long long> fenwick(boxes.size() + 1, 0);
    auto cmp = [](auto t1, auto t2) {return get<1>(t1) < get<1>(t2);};
    set<tuple<int, int, int, int, int, int>, decltype(cmp)> s(cmp);
    s.emplace(1, n, 0, 1, 1, n);
    for (auto [l1, r1, a1, b1] : queries) {
        if (a1 && b1) {
            for (auto it = s.lower_bound({0, l1, 0, 0, 0, 0}); it != s.end(); it = s.lower_bound({0, l1, 0, 0, 0, 0})) {
                auto [l2, r2, a2, b2, l3, r3] = *it;
                if (l2 > r1) break;

                s.erase(it);
                update(index(r2), -sum(l3, r3, a2, b2), fenwick);
                if (l2 < l1) {
                    s.emplace(l2, l1 - 1, a2, b2, l3, l1 - l2 - 1 + l3);
                    update(index(l1 - 1), sum(l3, l1 - l2 - 1 + l3, a2, b2), fenwick);
                }
                if (r1 < r2) {
                    s.emplace(r1 + 1, r2, a2, b2, r1 - r2 + 1 + r3, r3);
                    update(index(r2), sum(r1 - r2 + 1 + r3, r3, a2, b2), fenwick);
                }
            }

            s.emplace(l1, r1, a1, b1, 1, r1 - l1 + 1);
            update(index(r1), sum(1, r1 - l1 + 1, a1, b1), fenwick);
        } else {
            auto [l2, r2, a2, b2, l3, r3] = *s.lower_bound({0, r1, 0, 0, 0, 0});

            if (l1 >= l2) cout << sum(l1 - l2 + l3, r1 - r2 + r3, a2, b2) << "\n";
            else {
                auto rq = pref_sum(index(r1), fenwick) - pref_sum(index(l1 - 1), fenwick);

                if (r1 != r2) rq += sum(l3, r1 - l2 + l3, a2, b2);
                auto [l2, r2, a2, b2, l3, r3] = *s.lower_bound({0, l1, 0, 0, 0, 0});
                if (l1 != l2) rq -= sum(l3, l1 - l2 - 1 + l3, a2, b2);

                cout << rq << "\n";
            }
        }
    }
}
