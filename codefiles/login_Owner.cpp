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
#include <conio.h>


using namespace std;
using namespace std::chrono;



class O_login {
    string stored, entered;
    string vno;  // Vehicle number
    int v_type, pc;  // Vehicle type and parking choice
    char temp[10];
    char c;
    int totalLots, smallLots, mediumLots, largeLots;  // Number of lots
    bool isSetupComplete;  // Flag to track if setup is complete

public:
    O_login() {
        ifstream passwordFile("password.txt");
        getline(passwordFile, stored);
        passwordFile.close();

        cout << "Enter the password: ";
        int i;
        for (i = 0; i < 9; i++) {
            c = getch();
            if (c == 13) {
                break;
            } else if (c == 8) {
                if (i > 0) {
                    --i;
                    cout << "\b \b";
                    --i; 
                } else {
                    --i;
                }
            } else {
                cout << "*";
                temp[i] = c;
            }
        }
        temp[i] = '\0';

        if (stored == temp) {
            // Check if parking lot setup is complete
            ifstream setupFile("setup_status.txt");
            if (setupFile.good()) {
                setupFile >> isSetupComplete;
                setupFile.close();
            } else {
                isSetupComplete = false;
            }

            // First login setup
            if (!isSetupComplete) {
                // Input parking lot information
                cout << "\nEnter total number of parking lots: ";
                cin >> totalLots;
                cout << "Enter number of small parking lots: ";
                cin >> smallLots;
                cout << "Enter number of medium parking lots: ";
                cin >> mediumLots;
                cout << "Enter number of large parking lots: ";
                cin >> largeLots;

                // Validate lots
                if (smallLots + mediumLots + largeLots != totalLots) {
                    cout << "Error: Sum of small, medium, and large lots must equal total lots.\n";
                } else {
                    cout << "Parking lots setup complete!\n";

                    // Mark setup as complete
                    ofstream setupFile("setup_status.txt");
                    setupFile << true;
                    setupFile.close();
                }
            } else {
                cout << "\nSetup is already complete. Proceeding with login...\n";
            }

            // Enter vehicle details
            cout << "\nEnter your vehicle number: ";
            cin >> vno;
            cin.ignore();
            cout << "1. Two-wheeler\n2. Four-wheeler\n3. Electric\n4. Others\nEnter your vehicle type: ";
            cin >> v_type;
            cin.ignore();
            cout << "1. Temple\n2. Mall\n3. Park\nEnter your parking location choice: ";
            cin >> pc;
            cin.ignore();
        } else {
            cout << "Incorrect password. Try again.\n";
        }
    }

    int getVehicleType() const {
        switch (v_type) {
            case 1: return 2;  // Two-wheeler
            case 2: return 4;  // Four-wheeler
            case 3: return 0;  // Electric/Others
            case 4: return 0;  // Others
            default: return -1;  // Invalid type
        }
    }

    string getVehicleNo() const { return vno; }

    void displayLotInfo() const {
        cout << "Total Lots: " << totalLots << "\nSmall Lots: " << smallLots
             << "\nMedium Lots: " << mediumLots << "\nLarge Lots: " << largeLots << endl;
    }
};

class O_book{
    int day,hour;
    public:
        O_book(){
            cout<<"Enter number of days: ";
            cin>>day;
            cout<<"Enter number of hours: ";
            cin>>hour;
        }

        int amt(){
            int price=hour*40+day*1000;
            return price;
        }
};


int main() {
    O_login ownerLogin;
    ownerLogin.displayLotInfo();
    return 0;
}