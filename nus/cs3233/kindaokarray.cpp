#include <bits/stdc++.h>
using namespace std;

vector<int> sais(vector<int> &str, int range) {
    int n = str.size();
    if (!n) return {};
    if (n == 1) return {0};
    if (n == 2) return str[0] < str[1] ? vector<int>{0, 1} : vector<int>{1, 0};

    vector<int> sa(n, 0), sum_s(range + 1, 0), sum_l(range + 1, 0);
    vector<bool> sl(n, false);
    for (int i = n - 2; ~i; i--) sl[i] = (str[i] == str[i + 1]) ? sl[i + 1] : (str[i] < str[i + 1]);
    for (int i = 0; i < n; i++) {
        if (!sl[i]) sum_s[str[i]]++;
        else sum_l[str[i] + 1]++;
    }

    for (int i = 0; i <= range; i++) {
        sum_s[i] += sum_l[i];
        if (i < range) sum_l[i + 1] += sum_s[i];
    }

    auto induced_sort = [&](vector<int> &lms) {
        fill(sa.begin(), sa.end(), -1);
        vector<int> b(range + 1, 0);
        copy(sum_s.begin(), sum_s.end(), b.begin());
        for (auto i : lms)
            if (i < n) sa[b[str[i]]++] = i;

        copy(sum_l.begin(), sum_l.end(), b.begin());
        sa[b[str[n - 1]]++] = n - 1;
        for (int j : sa)
            if (j > 0 && !sl[j - 1]) sa[b[str[j - 1]]++] = j - 1;

        copy(sum_l.begin(), sum_l.end(), b.begin());
        for (int i = n - 1, j = sa[i]; ~i; j = sa[--i])
            if (j > 0 && sl[j - 1]) sa[--b[str[j - 1] + 1]] = j - 1;
    };

    vector<int> lms_map(n + 1, -1), lms;
    int m = 0;
    for (int i = 1; i < n; i++)
        if (!sl[i - 1] && sl[i]) {
            lms_map[i] = m++;
            lms.emplace_back(i);
        }
    induced_sort(lms);

    if (m) {
        vector<int> lms_sorted, str2(m);
        for (int j : sa)
            if (lms_map[j] != -1) lms_sorted.emplace_back(j);

        int range2 = 0;
        str2[lms_map[lms_sorted[0]]] = 0;
        for (int i = 1; i < m; i++) {
            int l = lms_sorted[i - 1], r = lms_sorted[i], l_end = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n, r_end = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
            bool same = true;
            if (l_end - l != r_end - r) same = false;
            else {
                while (l < l_end && str[l] == str[r]) {
                    l++;
                    r++;
                }

                if (l == n || str[l] != str[r]) same = false;
            }

            if (!same) range2++;
            str2[lms_map[lms_sorted[i]]] = range2;
        }

        auto sa2 = sais(str2, range2);
        for (int i = 0; i < m; i++) lms_sorted[i] = lms[sa2[i]];
        induced_sort(lms_sorted);
    }

    return sa;
}

vector<int> suffix_array(vector<int> &str, int range = 128) {
    return sais(str, range);
}

vector<int> kasai(vector<int> s, vector<int> sa) {
    int n = s.size();
    vector<int> rank(n), lcp(n - 1, 0);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    for (int i = 0, k = 0; i < n; i++) {
        if (k) k--;
        if (!rank[i]) continue;

        int j = sa[rank[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i] - 1] = k;
    }

    return lcp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<bool> pref_xor(n + 1, false);
    vector<int> pref_sum(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref_xor[i + 1] = pref_xor[i] ^ (a[i] & 1);
        pref_sum[i + 1] = pref_sum[i] + pref_xor[i + 1];
    }

    vector<int> indices(a.size()), str(a.size());
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) {return a[i] < a[j];});
    int range = 0;
    for (int i = 0; i < a.size(); i++) {
        if (i && a[indices[i - 1]] != a[indices[i]]) range++;
        str[indices[i]] = range;
    }
    range++;
    auto sa = suffix_array(str, range), lcp = kasai(str, sa);

    auto even = 0LL;
    for (int i = 0; i < n; i++) {
        int j = i ? sa[i] + lcp[i - 1] : sa[i], count = pref_sum[n] - pref_sum[j];
        even += (pref_xor[sa[i]]) ? count : n - j - count;
    }

    cout << even;
}