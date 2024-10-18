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


using namespace std;
using namespace std::chrono;

class Time {
protected:
    int year, month, day, hour, minute, second;

public:
    // Set the time to current system time
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

        cout << "Current date: " << year << "-" << month << "-" << day << endl;
        cout << "Current time: " << hour << ":" << minute << ":" << second << endl;
    }

    int getHour() const { return hour; }
    int getDay() const { return day; }
    int getMonth() const { return month; }

    void print() const {
        cout << year << "-" << setw(2) << setfill('0') << month << "-"
             << setw(2) << setfill('0') << day << " " << setw(2) << setfill('0')
             << hour << ":" << setw(2) << setfill('0') << minute << ":"
             << setw(2) << setfill('0') << second << endl;
    }
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
    int getMonthDiff() const { return d_month; }

    void print() const {
        cout << "Hours: " << d_hour << endl;
        cout << "Days: " << d_day << endl;
        cout << "Months: " << d_month << endl;
    }
};

class Reserve_Time {
protected:
    vector<pair<string, vector<int>>> vehicle_rates;

public:
    Reserve_Time() {
        vehicle_rates.push_back(make_pair("Two-wheeler", vector<int>{20, 450, 4500, 210}));
        vehicle_rates.push_back(make_pair("Four-wheeler", vector<int>{30, 500, 5000, 210}));
        vehicle_rates.push_back(make_pair("Bus/Other", vector<int>{35, 700, 7000, 210}));
    }

    int price(int type, int h, int d) {
        vector<int> rates;
        switch (type) {
            case 2:
                rates = vehicle_rates[0].second; // Two-wheeler rates
                break;
            case 4:
                rates = vehicle_rates[1].second; // Four-wheeler rates
                break;
            case 0:
                rates = vehicle_rates[2].second; // Bus/Other rates
                break;
            default:
                throw invalid_argument("Invalid vehicle type entered.");
        }

        int cost = h * rates[0] + d * rates[1]; // Total cost based on hours and days
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

        cout << "Entry added for vehicle: " << vehicle_no << endl;
    }

    Time get_entry_time(const string& vehicle_no) const {
        for (const auto& entry : entries) {
            if (entry.v_no == vehicle_no) {
                return entry.entry_time;
            }
        }
        throw invalid_argument("Vehicle not found.");
    }

    void display_entries() const {
        if (entries.empty()) {
            cout << "No entries to display!" << endl;
            return;
        }

        for (const auto& entry : entries) {
            cout << "Vehicle No: " << entry.v_no << " | Entry time: ";
            entry.entry_time.print();
        }
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

int main() {
    try {
        Note parking_notes;
        Time entryTime, exitTime;

        string vehicleNo = "MH12AB1234";

        
        entryTime.setCurrentTime();
        parking_notes.add_entry(vehicleNo, entryTime);

        parking_notes.display_entries();

        
        Sleep(3700000);

       
        cout << "\nExit time:" << endl;
        exitTime.setCurrentTime();


        Time recorded_entry_time = parking_notes.get_entry_time(vehicleNo);
        Calculate diff(recorded_entry_time, exitTime);
        diff.print();


        Reserve_Time pay;
        int type;

        cout << "Enter reservation type (2 for two-wheeler, 4 for four-wheeler, 0 for others): ";
        cin >> type;

        if (cin.fail()) {  
            throw invalid_argument("Invalid input! Please enter a numeric value for vehicle type.");
        }

        int totalCost = pay.price(type, diff.getHourDiff(), diff.getDayDiff());
        cout << "Total cost: Rs. " << totalCost << endl;


        string vehicleType = pay.getVehicleType(type);
        Receipt receipt(vehicleNo, vehicleType, diff.getHourDiff(), diff.getDayDiff(), totalCost);
        receipt.printReceipt();

    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }

    return 0;
}