#include <iostream>

using namespace std;
int len;
string s;

int dd(const string s,int l,int r)
{
    while (l>=0 && r<s.length() && s[r]==s[l])
    {
        l--;
        r++;
    }
    return r-l-1;
}
int maxdd(string s)
{
    for (int i=0;i<s.length();i++)
    {
        int len1=dd(s,i,i);
        int len2=dd(s,i,i+1);
        len=max(len,max(len1,len2));
    }
    return len;
}
int main()
{
    cin >> s;
    cout << maxdd(s);

}
