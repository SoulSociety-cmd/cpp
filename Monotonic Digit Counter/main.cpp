#include <bits/stdc++.h>

using namespace std;
int n,i,d=0,x;
bool check(int n){
    while (n>10){
    x=n%10;
    n=n/10;
    if (n > x) return false;
    }
    return true;
}
int main()
{
    cin >> n;
    int vector<int>a(n);
    for(i=0;i<n;i++){
     cin >> a[i];
    if (a[i]>10 && check(a[i])) d++;
    }
        cout << d;
}
