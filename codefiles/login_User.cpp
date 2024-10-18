#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <utility>
#include <windows.h>
#include <string>


using namespace std;
using namespace std::chrono;


class U_login{
    string name;
    string vno;// Vehicle number
    int v_type,pc;// Parking choice
    long mobile;
    public:
        U_login(){
            cout<<"Enter your name: ";
            cin>>name;
            cin.ignore();
            cout<<"Enter your mobile number: ";
            cin>>mobile;
            cin.ignore();
            cout<<"Enter your vehicle number: ";
            cin>>vno;
            cin.ignore();
            cout<<"1. 2-Wheeler\n2. 4-Wheeler\n3. Electric\n4. Others\nEnter your choice: ";
            cin>>v_type;
            cin.ignore();
            cout<<"1. Temple\n2. Mall\n3. Park\nEnter your choice: ";
            cin>>pc;
        }
        
};
