#include <bits/stdc++.h>

using namespace std;
class bank
{
private:
    string ma,ten;
    int sodu,cong,tru;
    static int dem;
public:
    bank()
    {
        dem++;
    }
    void nhap()
    {
        cin>>ma>>ten>>sodu;
    }
    int naptien()
    {
        cin>>cong;
        sodu+=cong;
        return sodu;
    }
    bool ruttien()
    {
        cin>>tru;
        if(tru>sodu) return false;
        sodu-=tru;
        return true;
    }
    void kt()
    {
        cout<<sodu;
    }
    string getma() const {return ma;};
    static int tong() {return dem;}
};
int bank::dem=0;                   //must
int main(){
    vector<bank>q;
    while(true)
    {
        cout<<"ma"<<" "<<"ten"<<" "<<"sodu"<<endl;
        q.push_back(bank());             //tạo ô trống vector
        q.back().nhap();                 //nhập dữ liệu vào ô đó
        cout<<"nhap 0 de dung:";
        int x;cin>>x;
        if(x==0) break;
    }
    cout<<"so tk: "<<bank::tong()<<endl;
    cout<<"tra cu ma: ";
    string tracuu;cin>>tracuu;
    cout<<endl;
    bool ans=false;
    for(auto &a:q)
    {
        if(a.getma()==tracuu)
        {
            ans=true;
            cout<<"nhan 1 de nap tien"<<endl<<"nhan 2 de rut tien"<<endl<<"nhan 3 de kt so du"<<endl;
    int t;cin >>t; cout<<endl;
    if(t==1){a.naptien();a.kt();}
     if(t==2){if(!a.ruttien()) cout<<"ko du";a.kt();}
         if(t==3){a.kt();}
        }
    }
    if(!ans) cout<<"no bank";
}

