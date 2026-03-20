#include <bits/stdc++.h>

using namespace std;
int n,gia,a;
int kq(const int k[],int n)
{
    int mins=INT_MAX;
    unordered_map<int,int>fre;
    for(int i=0;i<n;i++)fre[k[i]]++;
    for(const auto& [so,solan]:fre)
    {
        if(solan ==1 && so < mins) mins=so;
    }
    return mins;
}


int main()
{
    cin >>n;
    int k[n];
    for(int i=0;i<n;i++) cin >> k[i];
    a=kq(k,n);
    cout <<a;

}
