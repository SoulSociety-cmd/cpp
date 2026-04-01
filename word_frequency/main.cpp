#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string s;
    getline(cin,s);
    string tam;
    unordered_map<string,int>freq;
    for(int i=0;i<=s.size();i++)
    {
        if(s[i] != ' ' && i<s.size())
        {
            tam+=s[i];
        }
        else
        {
            if(tam != "") freq[tam]++;
            tam="";
        }
    }
    for(auto [chu,lan] : freq) cout<<chu<<"|"<<lan<<'\n';
}
