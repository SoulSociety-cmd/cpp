#include <iostream>

using namespace std;
int n,i,j,r,l,s;
int a[10000+2];

int main()
{
    cin >> n;
    for (i=1;i<=n;i++) cin >> a[i];
    for (i=1;i<=n;i++){
            for (j=i-1;j>=0;j--){
                if (a[j] < a[i]){
                    l=j;
                    break;
                }
            }
            for (j=i+1;j<=n+1;j++){
                if (a[j] < a[i]){
                    r=j;
                    break;
                }
            }
        s=max(s,a[i]*(r-l-1));
    }
    cout << s;
}
