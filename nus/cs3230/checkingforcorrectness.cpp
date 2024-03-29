#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e4;

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    char op;
    while (cin >> a >> op >> b) {
        a %= MODULO;

        if (op == '^') cout << pow(a, b, MODULO) << "\n"; 
        else {
            b %= MODULO;
            cout << (op == '+' ? (a + b) % MODULO : mul(a, b, MODULO) << "\n";
        }
    }
}
