#include <bits/stdc++.h>

using namespace std;
int n,a;
int kq(const vector<int>& k,int n)
{
    int mins=INT_MAX;
    unordered_map<int,int>fre;
    for(auto& x:k)fre[k[i]]++;
    for(const auto& [so,solan]:fre)
    {
        if(solan ==1 && so < mins) mins=so;
    }
    if(mins == INT_MAX) return -1;
    else return mins;
}

int main()
{
    cin >> n;
    vector<int>k(n);
    for(int i=0;i<n;i++) cin >> k[i];
    a=kq(k,n);
    cout << a;

}
