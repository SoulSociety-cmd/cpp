#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;cin>>n;
    vector<int>a(n);
    for(int *p=a.data();p<a.data()+n;p++)
    {
        cin>>*p;
    }
    for(int *p=a.data();p<a.data()+n;p++)
    {
        bool t=false;
        for(int *k=a.data();k<p;k++)
        {
            if(*k == *p)
            {
                t=true;
                break;
            }
        }
        if(t) continue;
        int countt = 0;
        for(int *q=a.data();q<a.data()+n;q++)
        {
            if(*q == *p) countt++;
        }
        cout<<*p<<"|"<<countt<<'\n';
    }



}
