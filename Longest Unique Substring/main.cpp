#include <iostream>

using namespace std;

int main()
{
    string s;
    cin>>s;
    int l=0;
    int ans=0;
    for(int r=0;r<s.size();r++)
    {
        for(int i=l;i<r;i++)
        {
            if(s[i] == s[r])
            {
                l=i+1;
                break;
            }
        }
        ans=max(ans,r-l+1);
    }
    cout<<ans;
}
