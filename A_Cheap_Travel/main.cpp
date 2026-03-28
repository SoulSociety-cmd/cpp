#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, m, a, b;
    if (!(cin >> n >> m >> a >> b)) return 0;
    long long cost1 = n * a;
    long long cost2 = (n / m) * b + (n % m) * a;
    long long cost3 = ((n + m - 1) / m) * b;
    cout << min(cost1, min(cost2, cost3)) << '\n';
    return 0;
}




