#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    srand(time(0)); // seed
    int ans = rand() % 100 + 1; // số bí mật
    int n = -1, dem = 0;

    cout << "So bi mat (test): " << ans << endl; // in ra để test

    while (n != ans) {
        cin >> n;
        dem += 1;
        if (n > ans) cout << "high\n";
        else if (n < ans) cout << "low\n";
        else cout << "good\n";
    }

    cout << "Ban doan dung sau " << dem << " lan.\n";
}
