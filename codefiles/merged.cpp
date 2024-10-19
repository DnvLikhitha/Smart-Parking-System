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
#include <conio.h>
#include <string>
#include <memory>

using namespace std;
using namespace std::chrono;

class Time {
protected:
    int year, month, day, hour, minute, second;

public:
    void setCurrentTime() {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        tm* localTime = localtime(&currentTime);

        year = 1900 + localTime->tm_year;
        month = 1 + localTime->tm_mon;
        day = localTime->tm_mday;
        hour = localTime->tm_hour;
        minute = localTime->tm_min;
        second = localTime->tm_sec;
    }

    void print() const {
        cout << year << "-" << setw(2) << setfill('0') << month << "-";
        cout << setw(2) << setfill('0') << day << " " << setw(2) << setfill('0');
        cout << hour << ":" << setw(2) << setfill('0') << minute << ":" << setw(2) << setfill('0') << second << endl;
    }

    int getHour() const { return hour; }
    int getDay() const { return day; }
    int getMonth() const { return month; }
};

class Calculate {
protected:
    int d_hour, d_day, d_month;

public:
    Calculate(const Time& entry, const Time& exit) {
        d_hour = exit.getHour() - entry.getHour();
        d_day = exit.getDay() - entry.getDay();
        d_month = exit.getMonth() - entry.getMonth();

        if (d_hour < 0) {
            d_hour += 24;
            d_day--;
        }
        if (d_day < 0) {
            d_day += 30;
            d_month--;
        }
    }

    int getHourDiff() const { return d_hour; }
    int getDayDiff() const { return d_day; }
};

class Reserve_Time {
protected:
    vector<pair<string, vector<int>>> vehicle_rates;

public:
    Reserve_Time() {
        vehicle_rates.push_back(make_pair("Two-wheeler", vector<int>{20, 450}));
        vehicle_rates.push_back(make_pair("Four-wheeler", vector<int>{30, 500}));
        vehicle_rates.push_back(make_pair("Bus/Other", vector<int>{35, 700}));
    }

    int price(int type, int h, int d) { 
        vector<int> rates;
        switch (type) {
            case 2:
                rates = vehicle_rates[0].second; 
                break;
            case 4:
                rates = vehicle_rates[1].second;  
                break;
            case 0:
                rates = vehicle_rates[2].second;  
                break;
            default:
                throw invalid_argument("Invalid vehicle type entered.");
        }

        int cost = h * rates[0] + d * rates[1];  
        return cost;
    }

    string getVehicleType(int type) const {
        switch (type) {
            case 2: return "Two-wheeler";
            case 4: return "Four-wheeler";
            case 0: return "Bus/Other";
            default: return "Unknown";
        }
    }
};

class Note {
    struct entry_note {
        string v_no;
        Time entry_time;
    };

    vector<entry_note> entries;

public:
    void add_entry(const string& vehicle_no, const Time& time) {
        entry_note new_entry;
        new_entry.v_no = vehicle_no;
        new_entry.entry_time = time;
        entries.push_back(new_entry);
    }

    Time get_entry_time(const string& vehicle_no) const {
        for (const auto& entry : entries) {
            if (entry.v_no == vehicle_no) {
                return entry.entry_time;
            }
        }
        throw invalid_argument("Vehicle not found.");
    }
};

class Receipt {
    string vehicleNo;
    string vehicleType;
    int hours;
    int days;
    int totalCost;

public:
    Receipt(const string& vNo, const string& vType, int h, int d, int cost)
        : vehicleNo(vNo), vehicleType(vType), hours(h), days(d), totalCost(cost) {}

    void printReceipt() const {
        cout << "\n********** RECEIPT **********\n";
        cout << "Vehicle No: " << vehicleNo << endl;
        cout << "Vehicle Type: " << vehicleType << endl;
        cout << "Total Time: " << days << " days and " << hours << " hours\n";
        cout << "Total Cost: Rs. " << totalCost << endl;
        cout << "****************************\n";
    }
};

class U_login {
    string name;
    string vno;  
    int v_type;  
    long mobile;

public:
    U_login() {
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your mobile number: ";
        cin >> mobile;
        cin.ignore();
        cout << "Enter your vehicle number: ";
        getline(cin, vno);
        cout << "1. Two-wheeler\n2. Four-wheeler\n3. Others\nEnter your vehicle type: ";
        cin >> v_type;
        cin.ignore();
    }

    int getVehicleType() const {
        switch (v_type) {
            case 1: return 2;  
            case 2: return 4;  
            case 3: return 0;  
            default: return -1;  
        }
    }

    string getVehicleNo() const { return vno; }
};

class O_login {
    string stored, entered;
    char temp[10];
    char c;

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
            cout << "\nLogin Successful\n";
        } else {
            cout << "Incorrect password. Try again.\n";
        }
    }
};

class O_book{
public:
    int day, hour;
    O_book() {
        cout << "Enter number of days: ";
        cin >> day;
        cout << "Enter number of hours: ";
        cin >> hour;
    }

    int amt() {
        return hour * 40 + day * 1000;
    }
};

int main() {
    Reserve_Time parkingRates;
    Note parkingNote;
    int revenue = 0;

    while (true) {
        int userType;
        cout << "Select login type:\n1. User Login\n2. Owner Login\n3. Exit\nEnter choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            U_login user;
            Time entry;
            entry.setCurrentTime();
            parkingNote.add_entry(user.getVehicleNo(), entry);

            cout << "\nAfter parking is complete, press any key to exit.\n";
            getch();

            Time exit;
            exit.setCurrentTime();

            Calculate duration(parkingNote.get_entry_time(user.getVehicleNo()), exit);
            int vehicleType = user.getVehicleType();
            int cost = parkingRates.price(vehicleType, duration.getHourDiff(), duration.getDayDiff());
            revenue += cost;

            Receipt receipt(user.getVehicleNo(), parkingRates.getVehicleType(vehicleType),duration.getHourDiff(), duration.getDayDiff(), cost);
            receipt.printReceipt();
        } 
        else if (userType == 2) {
            int t=0;
            O_login owner;
            while(t==0){
            int choice;
            cout << "\n1. Book\n2. Revenue\n3. Exit\nWhat do you want to do: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    O_book book;
                    int cost = book.amt();
                    revenue += cost;
                    string v_no;
                    cout<<"Enter the vehicle number: ";
                    cin>>v_no;
                    Receipt receipt(v_no, "Offline", book.hour, book.day, cost);
                    receipt.printReceipt();
                    break;
                }
                case 2: {
                    cout << "The total revenue is: Rs. " << revenue << "\n";
                    break;
                }
                case 3:{
                    t=1;
                    break;
                }
                default:
                    cout << "Invalid option.\n";
            }
            }
        } else if(userType == 3){
            return 0;
        } else {
            cout << "Invalid login type entered.\n";
        }

        int exit_choice;
        cout << "\nDo you want to exit? 1. Yes 2. No\n";
        cin >> exit_choice;
        if (exit_choice == 1) {
            break;
        }
    }
}
