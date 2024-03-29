#include <bits/stdc++.h>
using namespace std;

vector<long long> phi(int n) {
    vector<long long> phi(n + 1, 0);
    vector<bool> prime(n + 1, true);
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            primes.emplace_back(i);
            phi[i] = i - 1;
        }

        for (int j = 0; primes[j] <= n / i; j++) {
            int k = i * primes[j];
            prime[k] = false;
            phi[k] = phi[i] * phi[primes[j]];

            if (!(i % primes[j])) {
                phi[k] = phi[i] * primes[j];
                break;
            }
        }
    }

    return phi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto totient = phi(n);
    for (int i = 1; i <= n; i++) totient[i] += totient[i - 1];
    
    auto sum = 0LL;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        sum += totient[n / l] * (l + r) * (r - l + 1) / 2;
    }

    cout << sum;
}
