#include<iostream>
#include <fstream>
using namespace std;
class password {
        
    public:
    password(){
            
    }
    void update(){
        ofstream file("password.txt",  ios::trunc);
            int num;
            cout<<"Enter the password: ";
            cin>>num;
            file << to_string(num);
            file.close();
            cout<<"Password updated!";
    }
};