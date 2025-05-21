/**
 * Author: Quang Nhat Ngo
 * Date: 2025-05-21
 * Source: https://judge.yosupo.jp/submission/130647
 * Description: Count the number of primes less than n.
 * Time: O(n^(2/3))
 */
#pragma once

long long prime_counting(long long n) {
    constexpr int p[10]{2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    for (int i = 0; i < 10; ++i) if (n < p[i]) 
        return i;
    const int v =  sqrtl(n);
    int s = (v + 1) / 2;
    vector<int> smalls(s), roughs(s);
    vector<long long> larges(s);
    for (int i = 0; i < s; ++i) 
        smalls[i] = i, roughs[i] = 2 * i + 1, larges[i] = (n / (2 * i + 1) - 1) / 2;
    vector<bool> skip(v + 1);
    const auto divide = [](ll n, ll d) -> int { return (double)n / d;};
    const auto half = [](int n) -> int { return (n - 1) >> 1;};
    int pc = 0;
    for (int p = 3; p <= v; p += 2) if (!skip[p]) {
        int q = p * p;
        if ((ll)q * q > n) break;
        skip[p] = true;
        for (int i = q; i <= v; i += 2 * p) skip[i] = true;
        int ns = 0;
        for (int k = 0; k < s; k++) {
            int i = roughs[k];
            if (skip[i]) continue;
            ll d = (ll)i * p;
            larges[ns] = larges[k] - (d <= v ? larges[smalls[d >> 1] - pc] : smalls[half(divide(n, d))]) + pc;
            roughs[ns++] = i;
        }
        s = ns;
        for (int i = half(v), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
            int c = smalls[j >> 1] - pc;
            for (int e = (j * p) >> 1; i >= e; i--) smalls[i] -= c;
        }
        pc++;
    }
    larges[0] += (ll)(s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int k = 1; k < s; k++) larges[0] -= larges[k];
    for (int l = 1; l < s; l++) {
        ll q = roughs[l], m = n / q;
        int e = smalls[half(m / q)] - pc;
        if (e < l + 1) break;
        ll t = 0;
        for (int k = l + 1; k <= e; k++)
            t += smalls[half(divide(m, roughs[k]))];
        larges[0] += t - (ll)(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}