#include <bits/stdc++.h>

using namespace std;


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    vector <int> menhgia = {500 , 200 , 100 , 50 , 20 , 10 , 5 , 2 , 1};
    int n;cin>> n;
    int dem=0;
    for(int i=0;i<9;i++)
        {
            dem=n/menhgia[i];
            if(dem > 0)
            {
                cout<<menhgia[i]<<"|"<<dem<<'\n';
                n=n%menhgia[i];
            }
        }
}
