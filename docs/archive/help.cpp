#include <iostream>
using namespace std;

int main() {
    for(int i=0;i<23;i++){
            cout<<"{";
        for(int j=0;j<23;j++){
            if(i==0 || i==22 || j==0 || j==22){
                cout << "1,";
            }else if(i==11 && j==11){
                cout << "3,";
            }else{
                cout << "0,";
            }
        }
            cout<<"}";
        cout << endl;
    }
    cin.get();
    return 0;
}