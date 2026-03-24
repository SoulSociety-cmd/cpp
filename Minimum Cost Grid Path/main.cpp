#include <bits/stdc++.h>

using namespace std;

void snt(int hang,int cot,vector<vector<int>> &a,vector<vector<int>> &cost)
{
    for(int i=0;i<hang;i++)
        {
            for(int j=0;j<cot;j++)
                {
                    for(int k=1;k<=a[i][j];k++)
                        {
                            if(a[i][j] % k == 0) cost[i][j]++;
                        }
                }
        }
}

int main()
{
    int n,m;cin>>n>>m;
    vector<vector<int>> a(n,vector<int>(m));
     vector<vector<int>> cost(n,vector<int>(m));
     vector<vector<int>> dp(n,vector<int>(m));
    for(int i=0;i<n;i++)
        {
            for(int j=0;j<m;j++) cin >> a[i][j];
        }
    snt(n,m,a,cost);dp[0][0]=cost[0][0];
    for(int i=0;i<n;i++)
        {
            for(int j=0;j<m;j++)
                {
                    if(i==0 && j==0) continue;
                    if(i==0) dp[i][j] = dp[i][j-1] + cost[i][j];
                    else if(j==0) dp[i][j] = dp[i-1][j] + cost[i][j];
                    else dp[i][j] = min(dp[i][j] = dp[i-1][j] ,dp[i][j-1] ) + cost[i][j];
                }
        }
    cout<<dp[n-1][m-1];
}
