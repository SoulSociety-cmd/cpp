#include <bits/stdc++.h>

using namespace std;

void snt(int hang,int cot,int a[100][100])
{
    for(int i=0;i<hang;i++)
    for(int j=0;j<cot;j++)
    for(int k=2;k<=a[i][j];k++)
    if (a[i][j]%k==0) cost[i][j]+=1;
}

int main()
{
    vector<vector<int,int>> cost,dp;
    int n,m;cin>>m>>n;
    vector<vector<int,int>> a(n,vector<int>(m));
    for(int i=0;i<m;i++)
    for(int j=0;j<n;j++) cin >> a[i][j];
    snt(m,n,a);dp[0][0]=cost[0][0];
    for(int i=0;i<m;i++)
    for(int j=0;j<n;j++)
        dp[i][j]=min(dp[i][j-1]+cost[i][j],dp[i-1][j]+cost[i][j]);
    cout<<dp[m-1][n-1];
}
