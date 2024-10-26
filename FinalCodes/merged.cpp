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
#include <random>
#include <cstring>
#undef small
using namespace std;
using namespace std::chrono;

bool value = false;

class parkingspot {
protected:
    int id;
    bool isAvailable;

public:
    string spotType;
    string locality;
    parkingspot(int spotid, string type, string place) {
        id = spotid;
        spotType = type;
        isAvailable = true;
        locality = place;
    }
    void printdetails() {
        cout << "Spot ID: " << id << "\nAvailability: " << (isAvailable ? "Yes" : "No")
             << "\nSpot Type: " << spotType << "\nLocality: " << locality << endl;
    }
    virtual bool isSuitable(string Vtype, bool disabledAnswer = false) = 0;
    int getSpotid() { return id; }
    bool check_availability() { return isAvailable; }
    void reservation(string vtype, bool disabledAnswer = false) {
        string Ans;
        if (!check_availability()) {
            cout << "Spot not available" << endl;
            return;
        }
        if (!isSuitable(vtype, disabledAnswer)) {
            cout << "Spot not suitable" << endl;
            return;
        }
        cout << "Booking spot for "<< id << ": ";
        
    }
};

class small : public parkingspot {
public:
    small(int spotid, string place) : parkingspot(spotid, "Compact", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer = false) override { return Vtype == "Two-wheeler"; }
};

class compact : public parkingspot {
public:
    compact(int spotid, string place) : parkingspot(spotid, "Compact", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer = false) override { return Vtype == "Four-wheeler"; }
};

class large : public parkingspot {
public:
    large(int spotid, string place) : parkingspot(spotid, "Large", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer = false) override { return Vtype == "Bus/Other"; }
};

class electric : public parkingspot {
public:
    electric(int spotid, string place) : parkingspot(spotid, "Electric", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer = false) override { return Vtype == "Electric"; }
};

class disabled : public parkingspot {
public:
    disabled(int spotid, string place) : parkingspot(spotid, "Disabled", place) {}

    bool isSuitable(string Vtype, bool disabledAnswer) override {
        return disabledAnswer;
    }
};

class avail {
public:
    int M_Compact, M_Four, M_Large, M_Electric, M_Disabled;
    int T_Compact, T_Four, T_Large, T_Electric, T_Disabled;
    int P_Compact, P_Four, P_Large, P_Electric, P_Disabled;

    avail(vector<unique_ptr<parkingspot>>& spots) {
        ifstream inFile("spots.txt");
        int spotId;
        string type, locality;

        // Initialize counters to 0
        M_Compact = M_Four = M_Large = M_Electric = M_Disabled = 0;
        T_Compact = T_Four = T_Large = T_Electric = T_Disabled = 0;
        P_Compact = P_Four = P_Large = P_Electric = P_Disabled = 0;

        // Read configuration from file and set up parking spots
        while (inFile >> spotId >> type >> locality) {
            if (type == "small") {
                if (locality == "Mall") M_Compact++;
                if (locality == "Temple") T_Compact++;
                if (locality == "Park") P_Compact++;
                spots.push_back(make_unique<small>(spotId, locality));
            } else if (type == "compact") {
                if (locality == "Mall") M_Four++;
                if (locality == "Temple") T_Four++;
                if (locality == "Park") P_Four++;
                spots.push_back(make_unique<compact>(spotId, locality));
            } else if (type == "large") {
                if (locality == "Mall") M_Large++;
                if (locality == "Temple") T_Large++;
                if (locality == "Park") P_Large++;
                spots.push_back(make_unique<large>(spotId, locality));
            } else if (type == "electric") {
                if (locality == "Mall") M_Electric++;
                if (locality == "Temple") T_Electric++;
                if (locality == "Park") P_Electric++;
                spots.push_back(make_unique<electric>(spotId, locality));
            } else if (type == "disabled") {
                if (locality == "Mall") M_Disabled++;
                if (locality == "Temple") T_Disabled++;
                if (locality == "Park") P_Disabled++;
                spots.push_back(make_unique<disabled>(spotId, locality));
            }
        }
        inFile.close();
    }

    void decrementAvailableCount(const string& spotType, const string& locality) {
        if (locality == "Mall") {
            if (spotType == "Compact") M_Compact--;
            else if (spotType == "Four") M_Four--;
            else if (spotType == "Large") M_Large--;
            else if (spotType == "Electric") M_Electric--;
            else if (spotType == "Disabled") M_Disabled--;
        } else if (locality == "Temple") {
            if (spotType == "Compact") T_Compact--;
            else if (spotType == "Four") T_Four--;
            else if (spotType == "Large") T_Large--;
            else if (spotType == "Electric") T_Electric--;
            else if (spotType == "Disabled") T_Disabled--;
        } else if (locality == "Park") {
            if (spotType == "Compact") P_Compact--;
            else if (spotType == "Four") P_Four--;
            else if (spotType == "Large") P_Large--;
            else if (spotType == "Electric") P_Electric--;
            else if (spotType == "Disabled") P_Disabled--;
        }
    }

    void check() const {
        cout << "\nParking Details: " << endl;
        cout << "\n-------------------------";
        cout << "\nMall Parking details: " << endl;
        cout << "2 Wheelers: " << M_Compact << "\n4 Wheelers: " << M_Four
             << "\nLarge: " << M_Large << "\nElectric: " << M_Electric
             << "\nDisabled: " << M_Disabled;
        cout << "\n-------------------------";
        cout << "\nPark Parking details: " << endl;
        cout << "2 Wheelers: " << P_Compact << "\n4 Wheelers: " << P_Four
             << "\nLarge: " << P_Large << "\nElectric: " << P_Electric
             << "\nDisabled: " << P_Disabled;
        cout << "\n-------------------------";
        cout << "\nTemple Parking details: " << endl;
        cout << "2 Wheelers: " << T_Compact << "\n4 Wheelers: " << T_Four
             << "\nLarge: " << T_Large << "\nElectric: " << T_Electric
             << "\nDisabled: " << T_Disabled;
        cout << "\n-------------------------\n\n\n";
    }
    int id(const string& spotType, const string& locality) {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    if (locality == "Mall") {
        if (spotType == "Compact" && M_Compact > 0) return 1 + (rand() % M_Compact);
        else if (spotType == "Four-wheeler" && M_Four > 0) return 1 + (rand() % M_Four);
        else if (spotType == "Large" && M_Large > 0) return 1 + (rand() % M_Large);
        else if (spotType == "Electric" && M_Electric > 0) return 1 + (rand() % M_Electric);
        else if (spotType == "Disabled" && M_Disabled > 0) return 1 + (rand() % M_Disabled);
    } else if (locality == "Temple") {
        if (spotType == "Compact" && T_Compact > 0) return 1 + (rand() % T_Compact);
        else if (spotType == "Four-wheeler" && T_Four > 0) return 1 + (rand() % T_Four);
        else if (spotType == "Large" && T_Large > 0) return 1 + (rand() % T_Large);
        else if (spotType == "Electric" && T_Electric > 0) return 1 + (rand() % T_Electric);
        else if (spotType == "Disabled" && T_Disabled > 0) return 1 + (rand() % T_Disabled);
    } else if (locality == "Park") {
        if (spotType == "Compact" && P_Compact > 0) return 1 + (rand() % P_Compact);
        else if (spotType == "Four-wheeler" && P_Four > 0) return 1 + (rand() % P_Four);
        else if (spotType == "Large" && P_Large > 0) return 1 + (rand() % P_Large);
        else if (spotType == "Electric" && P_Electric > 0) return 1 + (rand() % P_Electric);
        else if (spotType == "Disabled" && P_Disabled > 0) return 1 + (rand() % P_Disabled);
    }
    
    // If no available spots are found, return -1 to indicate failure
    return -1;
}

};

class Time {
protected:
    int year, month, day, hour, minute, second;

public:
    void setCurrentTime() {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        tm *localTime = localtime(&currentTime);

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
    Calculate(const Time &entry, const Time &exit) {
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

    int price(int type, int h, int d, int &revenue) {
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

        ifstream rev("revenue.txt");
        int t_price = 0;
        if (rev) {
            rev >> t_price;
        }
        rev.close();

        int cost = 20+h * rates[0] + d * rates[1];
        revenue += cost;

        ofstream revenueFile("revenue.txt", ios::trunc);
        revenueFile << cost + t_price << endl;
        revenueFile.close();

        return cost;
    }

    string getVehicleType(int type) const {
        switch (type) {
        case 2:
            return "Two-wheeler";
        case 4:
            return "Four-wheeler";
        case 3:
            return "Electric Vehicle";
        case 0:
            return "Bus/Other";
        default:
            return "Unknown";
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
    int spotID;
    int referenceNo;
    string location;  // Added location field

public:
    Receipt(const string& vNo, const string& vType, int h, int d, int cost, int sID, int rNo, const string& loc)
        : vehicleNo(vNo), vehicleType(vType), hours(h), days(d), totalCost(cost), 
          spotID(sID), referenceNo(rNo), location(loc) {}

    void printReceipt() const {
        cout << "\n********** RECEIPT **********\n";
        cout << "Vehicle No: " << vehicleNo << endl;
        cout << "Vehicle Type: " << vehicleType << endl;
        cout << "Location: " << location << endl;  // Added location printing
        cout << "Parking Spot ID: " << spotID << endl;
        cout << "Reference Number: " << referenceNo << endl;
        cout << "Total Time: " << days << " days and " << hours << " hours\n";
        cout << "Total Cost: Rs. " << totalCost << endl;
        cout << "****************************\n";
    }
};

class U_login {
    string name;
    string vno;  // Vehicle number
    int v_type, pc;  // Vehicle type and parking choice
    long mobile;

public:
    string loc;
    U_login() {
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your mobile number: ";
        cin >> mobile;
        cin.ignore();
        cout << "Enter your vehicle number: ";
        getline(cin, vno);
        cout << "1. Two-wheeler\n2. Four-wheeler\n3. Bus/Other\nEnter your vehicle type: ";
        cin >> v_type;
        cin.ignore();
        cout << "1. Temple\n2. Mall\n3. Park\nEnter your parking location choice: ";
        cin >> pc;
        switch(pc){
            case 1:
                loc="Mall";
                break;
            case 2:
                loc="Temple";
                break;
            case 3:
                loc="Park";
                break;
        }
        cin.ignore();
    }

    int getVehicleType() const {
        switch (v_type) {
            case 1: return 2;  // Two-wheeler
            case 2: return 4;  // Four-wheeler
            case 3: return 0;  // Bus/Other
            default: return -1;  // Invalid type
        }
    }

    string getLocation() const {
        switch (pc) {
            case 1: return "Temple";
            case 2: return "Mall";
            case 3: return "Park";
            default: return "Unknown";
        }
    }

    string getVehicleNo() const { return vno; }

    void printDetails() const {
        cout << "\nUser: " << name << " | Mobile: " << mobile << " | Vehicle No: " << vno << "\n";
    }
};

class O_login {
    string stored, entered;
    string vno;  // Vehicle number
    int v_type, pc;  // Vehicle type and parking choice
    char temp[10];
    char c;
    
public:
    O_login() {
        bool a=true;
        ifstream passwordFile("password.txt");
        getline(passwordFile, stored);
        passwordFile.close();
        while(a){
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
            cout << "\nLogin Sucessful\n";
            a=false;
        } else {
            cout << "Incorrect password. Try again.\n";
        }
        }
    }

    int getVehicleType() const {
    switch (v_type) {
        case 1: return 2;  // Two-wheeler
        case 2: return 4;  // Four-wheeler
        case 3: return 3;  // Electric
        case 4: return 0;  // Others
        default: return -1;  // Invalid type
    }
}


    int loadTotalRevenue() {
        ifstream revenueFile("revenue.txt");
        int revenue = 0;
        if (revenueFile) {
            revenueFile >> revenue;
        }
        revenueFile.close();
        return revenue;
    }

    string getVehicleNo() const { return vno; }

    void displayAvailableSpots(const vector<unique_ptr<parkingspot>>& spots) {
    cout << "\nAvailable Parking Spots:\n";
    for (const auto& spot : spots) {
        if (spot->check_availability()) {
            spot->printdetails();
            cout << "-------------------------\n";
        }
    }
}
};

class O_book : public O_login {
private:
    string getLocation(int choice) {
        switch (choice) {
            case 1: return "Temple";
            case 2: return "Mall";
            case 3: return "Park";
            default: return "Unknown";
        }
    }

    string getVehicleTypeString(int choice) {
        switch (choice) {
            case 1: return "Two-wheeler";
            case 2: return "Four-wheeler";
            case 3: return "Electric";
            case 4: return "Bus/Other";
            default: return "Unknown";
        }
    }

public:
    int day, hour;
    string vehicle;
    string selectedLocation;
    string selectedVehicleType;

    O_book(vector<unique_ptr<parkingspot>>& spots, avail& AVAILABLE) {
        int locationChoice;
        cout << "1. Temple\n2. Mall\n3. Park\nEnter parking location choice: ";
        cin >> locationChoice;
        selectedLocation = getLocation(locationChoice);

        int vehicleTypeChoice;
        cout << "1. Two-wheeler\n2. Four-wheeler\n3. Electric\n4. Bus/Other\nEnter vehicle type: ";
        cin >> vehicleTypeChoice;
        selectedVehicleType = getVehicleTypeString(vehicleTypeChoice);

        cout << "Enter number of days: ";
        cin >> day;
        cout << "Enter number of hours: ";
        cin >> hour;
        cout << "Enter vehicle number: ";
        cin >> vehicle;

        cout << "\nAvailable Parking Spots at " << selectedLocation << " for " << selectedVehicleType << ":\n";
        bool spotsFound = false;
        
        // Display available spots suitable for the vehicle type
        
        
    }

    int amt() {
        int price = hour * 40 + day * 1000;
        int t_price = loadTotalRevenue();
        ofstream revenueFile("revenue.txt", ios::trunc);
        revenueFile << price + t_price << endl;
        revenueFile.close();
        return price;
    }
};