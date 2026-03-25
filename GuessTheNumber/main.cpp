#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    srand(time(0)); 
    int ans = rand() % 100 + 1; 
    int n = -1, dem = 0;
    while (n != ans) {
        cin >> n;
        dem += 1;
        if (n > ans) cout << "high\n";
        else if (n < ans) cout << "low\n";
        else cout << "good\n";
    }
    cout << "Ban doan dung sau " << dem << " lan.\n";
}
