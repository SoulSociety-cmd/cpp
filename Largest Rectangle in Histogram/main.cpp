#include <bits/stdc++.h>

using namespace std;
int n,i,j,r,l,s;

int main()
{
    cin >> n;
    vector<int>a(n);
    for (i=0;i<n;i++) cin >> a[i];
    for (i=0;i<n;i++)
    {
        for (j=i-1;j>=0;j--)
        {
            if (a[j] < a[i])
            {
                l=j;
                break;
            }
        }
        for (j=i+1;j<=n+1;j++)
        {
            if (a[j] < a[i])
            {
                r=j;
                break;
            }
        }
        s=max(s,a[i]*(r-l-1));
    }
    cout << s;
}
