#include <bits/stdc++.h>

using namespace std;
bool check(int a, int dem)
{
    int x=a;
    int ans=0,tam;
    while(a > 0)
    {
        tam=a%10;
        int sum=1;
        for(int i=0;i<dem;i++)
        {
            sum=sum*tam;
        }
        ans+=sum;
        a/=10;
    }
    return ans == x;
}

int main()
{
    int n;cin>>n;
    int x=10;
    int dem=1;
    for(int i=1;i<=n;i++)
    {
        if(i == x)
        {
            dem++;
            x=x*10;
        }
        if(check(i,dem)) cout<<i<<"|";
    }
}
