#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    long long c[3], m[3];
    for (int i = 0; i < 3; i++) cin >> c[i] >> m[i];
    for (int i = 0; i < 100; i++) {
        int from = i % 3;
        int to = (i + 1) % 3;
        long long pour = min(m[from], c[to] - m[to]);
        m[from] -= pour;
        m[to] += pour;
    }

    for (int i = 0; i < 3; i++) cout << m[i] << endl;
    return 0;

}
