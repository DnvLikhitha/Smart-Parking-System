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
class avail;
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
        if (!check_availability()) {
            throw runtime_error("Spot not available.");
        }
        if (!isSuitable(vtype, disabledAnswer)) {
            throw runtime_error("Spot not suitable for the vehicle type.");
        }
        cout << "Booking spot for " << id << ": ";
        
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
    electric(int spotid, string place) : parkingspot(spotid, "electric", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer) override { return disabledAnswer; }
};

class disabled : public parkingspot {
public:
    disabled(int spotid, string place) : parkingspot(spotid, "Disabled", place) {}
    bool isSuitable(string Vtype, bool disabledAnswer) override { return disabledAnswer; }
};

class avail {
            
public:
    
            int M_Compact, M_Four, M_Large, M_electric, M_Disabled;
            int T_Compact, T_Four, T_Large, T_electric, T_Disabled;
            int P_Compact, P_Four, P_Large, P_electric, P_Disabled;
    avail(){}
    avail(vector<unique_ptr<parkingspot>>& spots) {
        try {
            
            
            ifstream inFile("spots.txt");
            if (!inFile) {
                throw runtime_error("Unable to open spots configuration file.");
            }

            int spotId;
            string type, locality;

            M_Compact = M_Four = M_Large = M_electric = M_Disabled = 0;
            T_Compact = T_Four = T_Large = T_electric = T_Disabled = 0;
            P_Compact = P_Four = P_Large = P_electric = P_Disabled = 0;

            while (inFile >> spotId >> type >> locality) {
                try {
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
                        if (locality == "Mall") M_electric++;
                        if (locality == "Temple") T_electric++;
                        if (locality == "Park") P_electric++;
                        spots.push_back(make_unique<electric>(spotId, locality));
                    } else if (type == "disabled") {
                        if (locality == "Mall") M_Disabled++;
                        if (locality == "Temple") T_Disabled++;
                        if (locality == "Park") P_Disabled++;
                        spots.push_back(make_unique<disabled>(spotId, locality));
                    } else {
                        throw invalid_argument("Invalid spot type in configuration file.");
                    }
                } catch (const invalid_argument& e) {
                    cerr << "Error parsing spot configuration: " << e.what() << endl;
                }
            }
            inFile.close();
        } catch (const runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void decrementAvailableCount(const string& spotType, const string& locality) {
    try {
        if (locality == "Mall") {
            if (spotType == "Compact" && M_Compact > 0) M_Compact--;
            else if (spotType == "Four" && M_Four > 0) M_Four--;
            else if (spotType == "Large" && M_Large > 0) M_Large--;
            else if (spotType == "electric" && M_electric > 0) M_electric--;
            else if (spotType == "Disabled" && M_Disabled > 0) M_Disabled--;
            else throw runtime_error("Invalid decrement request for Mall.");
        } else if (locality == "Temple") {
            if (spotType == "Compact" && T_Compact > 0) T_Compact--;
            else if (spotType == "Four" && T_Four > 0) T_Four--;
            else if (spotType == "Large" && T_Large > 0) T_Large--;
            else if (spotType == "electric" && T_electric > 0) T_electric--;
            else if (spotType == "Disabled" && T_Disabled > 0) T_Disabled--;
            else throw runtime_error("Invalid decrement request for Temple.");
        } else if (locality == "Park") {
            if (spotType == "Compact" && P_Compact > 0) P_Compact--;
            else if (spotType == "Four" && P_Four > 0) P_Four--;
            else if (spotType == "Large" && P_Large > 0) P_Large--;
            else if (spotType == "electric" && P_electric > 0) P_electric--;
            else if (spotType == "Disabled" && P_Disabled > 0) P_Disabled--;
            else throw runtime_error("Invalid decrement request for Park.");
        } else {
            throw invalid_argument("Invalid locality.");
        }
    } catch (const exception& e) {
        cerr << "Error in decrementAvailableCount: " << e.what() << endl;
    }
}

void check() const {
    try {
        cout << "\nParking Details: " << endl;
        cout << "\n-------------------------";
        cout << "\nMall Parking details: " << endl;
        cout << "2 Wheelers: " << M_Compact << "\n4 Wheelers: " << M_Four
             << "\nLarge: " << M_Large << "\nelectric: " << M_electric
             << "\nDisabled: " << M_Disabled;
        cout << "\n-------------------------";
        cout << "\nPark Parking details: " << endl;
        cout << "2 Wheelers: " << P_Compact << "\n4 Wheelers: " << P_Four
             << "\nLarge: " << P_Large << "\nelectric: " << P_electric
             << "\nDisabled: " << P_Disabled;
        cout << "\n-------------------------";
        cout << "\nTemple Parking details: " << endl;
        cout << "2 Wheelers: " << T_Compact << "\n4 Wheelers: " << T_Four
             << "\nLarge: " << T_Large << "\nelectric: " << T_electric
             << "\nDisabled: " << T_Disabled;
        cout << "\n-------------------------\n\n\n";
    } catch (const exception& e) {
        cerr << "Error in check: " << e.what() << endl;
    }
}

int id(const string& spotType, const string& locality) {
    try {
        srand(static_cast<unsigned int>(time(0)));

        if (locality == "Mall") {
            if (spotType == "Compact" && M_Compact > 0) return 1 + (rand() % M_Compact);
            else if (spotType == "Four-wheeler" && M_Four > 0) return 1 + (rand() % M_Four);
            else if (spotType == "Large" && M_Large > 0) return 1 + (rand() % M_Large);
            else if (spotType == "electric" && M_electric > 0) return 1 + (rand() % M_electric);
            else if (spotType == "Disabled" && M_Disabled > 0) return 1 + (rand() % M_Disabled);
        } else if (locality == "Temple") {
            if (spotType == "Compact" && T_Compact > 0) return 1 + (rand() % T_Compact);
            else if (spotType == "Four-wheeler" && T_Four > 0) return 1 + (rand() % T_Four);
            else if (spotType == "Large" && T_Large > 0) return 1 + (rand() % T_Large);
            else if (spotType == "electric" && T_electric > 0) return 1 + (rand() % T_electric);
            else if (spotType == "Disabled" && T_Disabled > 0) return 1 + (rand() % T_Disabled);
        } else if (locality == "Park") {
            if (spotType == "Compact" && P_Compact > 0) return 1 + (rand() % P_Compact);
            else if (spotType == "Four-wheeler" && P_Four > 0) return 1 + (rand() % P_Four);
            else if (spotType == "Large" && P_Large > 0) return 1 + (rand() % P_Large);
            else if (spotType == "electric" && P_electric > 0) return 1 + (rand() % P_electric);
            else if (spotType == "Disabled" && P_Disabled > 0) return 1 + (rand() % P_Disabled);
        } else {
            throw invalid_argument("Invalid locality provided.");
        }

        throw runtime_error("No available spots for the given type and locality.");
    } catch (const exception& e) {
        cerr << "Error in id: " << e.what() << endl;
        return -1; 
    }
}

};

class Time {
protected:
    int year, month, day, hour, minute, second;

public:
    void setCurrentTime() {
        try {
            auto now = chrono::system_clock::now();
            time_t currentTime = chrono::system_clock::to_time_t(now);
            tm *localTime = localtime(&currentTime);

            if (!localTime) {
                throw runtime_error("Failed to retrieve local time.");
            }

            year = 1900 + localTime->tm_year;
            month = 1 + localTime->tm_mon;
            day = localTime->tm_mday;
            hour = localTime->tm_hour;
            minute = localTime->tm_min;
            second = localTime->tm_sec;

            cout << "Current date: " << year << "-" << month << "-" << day << endl;
            cout << "Current time: " << hour << ":" << minute << ":" << second << endl;
        } catch (const exception &e) {
            cerr << "Error in setCurrentTime: " << e.what() << endl;
        }
    }

    int getHour() const { return hour; }
    int getDay() const { return day; }
    int getMonth() const { return month; }

    void print() const {
        try {
            cout << year << "-" << setw(2) << setfill('0') << month << "-"
                 << setw(2) << setfill('0') << day << " " << setw(2) << setfill('0')
                 << hour << ":" << setw(2) << setfill('0') << minute << ":"
                 << setw(2) << setfill('0') << second << endl;
        } catch (const exception &e) {
            cerr << "Error in print: " << e.what() << endl;
        }
    }
};

class Calculate {
protected:
    int d_hour, d_day, d_month;

public:
    Calculate(const Time &entry, const Time &exit) {
        try {
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

            if (d_month < 0) {
                throw runtime_error("Exit time cannot be earlier than entry time.");
            }
        } catch (const exception &e) {
            cerr << "Error in Calculate constructor: " << e.what() << endl;
        }
    }

    int getHourDiff() const { return d_hour; }
    int getDayDiff() const { return d_day; }
    int getMonthDiff() const { return d_month; }

    void print() const {
        try {
            cout << "Hours: " << d_hour << endl;
            cout << "Days: " << d_day << endl;
            cout << "Months: " << d_month << endl;
        } catch (const exception &e) {
            cerr << "Error in print: " << e.what() << endl;
        }
    }
};

class Reserve_Time {
protected:
    vector<pair<string, vector<int>>> vehicle_rates;

public:
    Reserve_Time() {
        try {
            vehicle_rates.push_back(make_pair("Two-wheeler", vector<int>{20, 450, 4500, 210}));
            vehicle_rates.push_back(make_pair("Four-wheeler", vector<int>{30, 500, 5000, 210}));
            vehicle_rates.push_back(make_pair("Bus/Other", vector<int>{35, 700, 7000, 210}));
        } catch (const exception &e) {
            cerr << "Error in Reserve_Time constructor: " << e.what() << endl;
        }
    }

    int price(int type, int h, int d, int &revenue) {
        try {
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
            } else if (!rev.eof()) {
                throw runtime_error("Failed to read revenue file.");
            }
            rev.close();

            int cost = 20 + h * rates[0] + d * rates[1];
            revenue += cost;

            ofstream revenueFile("revenue.txt", ios::trunc);
            if (!revenueFile) {
                throw runtime_error("Failed to write to revenue file.");
            }
            revenueFile << cost + t_price << endl;
            revenueFile.close();

            return cost;
        } catch (const exception &e) {
            cerr << "Error in price: " << e.what() << endl;
            return -1; 
        }
    }

    string getVehicleType(int type) const {
        try {
            switch (type) {
                cout<<"\n1\t"<<type<<endl;
            case 2:
                return "Two-wheeler";
            case 4:
                return "Four-wheeler";
            case 0:
                return "Bus/Other";
            default:
                throw invalid_argument("Invalid vehicle type.");
            }
        } catch (const exception &e) {
            cerr << "Error in getVehicleType: " << e.what() << endl;
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
    void add_entry(const string &vehicle_no, const Time &time) {
        try {
            entry_note new_entry;
            new_entry.v_no = vehicle_no;
            new_entry.entry_time = time;

            entries.push_back(new_entry);

            cout << "Entry added for vehicle: " << vehicle_no << endl;
        } catch (const exception &e) {
            cerr << "Error in add_entry: " << e.what() << endl;
        }
    }

    Time get_entry_time(const string &vehicle_no) const {
        try {
            for (const auto &entry : entries) {
                if (entry.v_no == vehicle_no) {
                    return entry.entry_time;
                }
            }
            throw invalid_argument("Vehicle not found.");
        } catch (const exception &e) {
            cerr << "Error in get_entry_time: " << e.what() << endl;
            throw; // Re-throwing the exception for the caller to handle
        }
    }

    void display_entries() const {
        try {
            if (entries.empty()) {
                cout << "No entries to display!" << endl;
                return;
            }

            for (const auto &entry : entries) {
                cout << "Vehicle No: " << entry.v_no << " | Entry time: ";
                entry.entry_time.print();
            }
        } catch (const exception &e) {
            cerr << "Error in display_entries: " << e.what() << endl;
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
    Receipt(const string &vNo, const string &vType, int h, int d, int cost, int sID, int rNo, const string &loc)
        : vehicleNo(vNo), vehicleType(vType), hours(h), days(d), totalCost(cost),
          spotID(sID), referenceNo(rNo), location(loc) {}

    void printReceipt() const {
        try {
            cout << "\n********** RECEIPT **********\n";
            cout << "Vehicle No: " << vehicleNo << endl;
            cout << "Vehicle Type: " << vehicleType << endl;
            cout << "Location: " << location << endl;
            cout << "Parking Spot ID: " << spotID << endl;
            cout << "Reference Number: " << referenceNo << endl;
            cout << "Total Time: " << days << " days and " << hours << " hours\n";
            cout << "Total Cost: Rs. " << totalCost << endl;
            cout << "****************************\n";
        } catch (const exception &e) {
            cerr << "Error in printReceipt: " << e.what() << endl;
        }
    }
};

class U_login : public avail {
    string name;
    string vno;  // Vehicle number
    int v_type, pc;  // Vehicle type and parking choice
    long long mobile;

public:
    string loc;

    U_login(vector<unique_ptr<parkingspot>>& spots) : avail(spots) {
        try {
            cout << "Enter your name: ";
            getline(cin, name);

            cout << "Enter your mobile number: ";
            cin >> mobile;
            if (cin.fail() || mobile < 1000000000 || mobile > 9999999999) {
                throw invalid_argument("Invalid mobile number entered.");
            }
            cin.ignore();

            cout << "Enter your vehicle number: ";
            getline(cin, vno);

            cout << "1. Two-wheeler\n2. Four-wheeler\n3. Bus/Other\nEnter your vehicle type: ";
            cin >> v_type;
            if (v_type < 1 || v_type > 3) {
                throw invalid_argument("Invalid vehicle type entered.");
            }
            cin.ignore();

            cout << "1. Temple\n2. Mall\n3. Park\nEnter your parking location choice: ";
            cin >> pc;
            if (pc < 1 || pc > 3) {
                throw invalid_argument("Invalid parking location choice entered.");
            }
            cin.ignore();

            // Determine location
            switch (pc) {
                case 1: loc = "Temple"; break;
                case 2: loc = "Mall"; break;
                case 3: loc = "Park"; break;
            }

            // Determine vehicle type string
            string vehicleType;
            switch (v_type) {
                case 1: vehicleType = "Compact"; break;
                case 2: vehicleType = "Four"; break;
                case 3: vehicleType = "Large"; break;
                default:
                    throw invalid_argument("Invalid vehicle type.");
            }

            // Decrement the count for the selected spot type and locality
            decrementAvailableCount(vehicleType, loc);

            cout << "Booking successful! Spot type: " << vehicleType << ", Location: " << loc << endl;
        } catch (const exception& e) {
            cerr << "Error in U_login constructor: " << e.what() << endl;
        }
    }

        int getVehicleType() const {
        try {
            switch (v_type) {
                case 1: return 2;  // Two-wheeler
                case 2: return 4;  // Four-wheeler
                case 4: return 0;  // Bus/Other
                default:
                    throw invalid_argument("Invalid vehicle type.");
            }
        } catch (const exception& e) {
            cerr << "Error in getVehicleType: " << e.what() << endl;
            return -1; 
        }
    }


    string getLocation() const {
        try {
            switch (pc) {
                case 1: return "Temple";
                case 2: return "Mall";
                case 3: return "Park";
                default:
                    throw invalid_argument("Invalid parking location.");
            }
        } catch (const exception& e) {
            cerr << "Error in getLocation: " << e.what() << endl;
            return "Unknown";
        }
    }

    string getVehicleNo() const {
        try {
            return vno;
        } catch (const exception& e) {
            cerr << "Error in getVehicleNo: " << e.what() << endl;
            return "";
        }
    }

    void printDetails() const {
        try {
            cout << "\nUser: " << name << " | Mobile: " << mobile << " | Vehicle No: " << vno << "\n";
        } catch (const exception& e) {
            cerr << "Error in printDetails: " << e.what() << endl;
        }
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
        try {
            ifstream passwordFile("password.txt");
            if (!passwordFile) {
                throw runtime_error("Password file could not be opened.");
            }
            getline(passwordFile, stored);
            passwordFile.close();

            bool a = true;
            while (a) {
                cout << "Enter the password: ";
                int i;
                for (i = 0; i < 9; i++) {
                    c = getch();
                    if (c == 13) {  // Enter key
                        break;
                    } else if (c == 8) {  // Backspace key
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
                    a = false;
                } else {
                    cout << "\nIncorrect password. Try again.\n";
                }
            }
        } catch (const exception& e) {
            cerr << "Error in O_login: " << e.what() << endl;
        }
    }

    int getVehicleType() const {
        try {
            switch (v_type) {
                case 1: return 2;  // Two-wheeler
                case 2: return 4;  // Four-wheeler
                case 4: return 0;  // Others
                default: throw invalid_argument("Invalid vehicle type selected.");
            }
        } catch (const exception& e) {
            cerr << "Error in getVehicleType: " << e.what() << endl;
            return -1;  
        }
    }

    int loadTotalRevenue() {
        try {
            ifstream revenueFile("revenue.txt");
            if (!revenueFile) {
                throw runtime_error("Revenue file could not be opened.");
            }
            int revenue = 0;
            revenueFile >> revenue;
            revenueFile.close();
            return revenue;
        } catch (const exception& e) {
            cerr << "Error in loadTotalRevenue: " << e.what() << endl;
            return 0;  
        }
    }

    string getVehicleNo() const {
        try {
            return vno;
        } catch (const exception& e) {
            cerr << "Error in getVehicleNo: " << e.what() << endl;
            return "";
        }
    }

    void displayAvailableSpots(const vector<unique_ptr<parkingspot>>& spots) {
        try {
            cout << "\nAvailable Parking Spots:\n";
            for (const auto& spot : spots) {
                if (spot->check_availability()) {
                    spot->printdetails();
                    cout << "-------------------------\n";
                }
            }
        } catch (const exception& e) {
            cerr << "Error in displayAvailableSpots: " << e.what() << endl;
        }
    }
};

class O_book : public O_login {
private:
    string getLocation(int choice) {
        try {
            switch (choice) {
                case 1: return "Temple";
                case 2: return "Mall";
                case 3: return "Park";
                default: throw invalid_argument("Invalid parking location choice.");
            }
        } catch (const exception& e) {
            cerr << "Error in getLocation: " << e.what() << endl;
            return "Unknown";
        }
    }

    string getVehicleTypeString(int choice) {
        try {
            switch (choice) {
                cout<<"\n2\t"<<choice<<endl;
                case 1: return "Compact";       // Two-wheeler
                case 2: return "Four";          // Four-wheeler
                case 3: return "electric";      // electric vehicle
                case 4: return "Large";         // Bus/Other
                default: throw invalid_argument("Invalid vehicle type choice.");
            }
        } catch (const exception& e) {
            cerr << "Error in getVehicleTypeString: " << e.what() << endl;
            return "Unknown";
        }
    }

public:
    int day, hour;
    string vehicle;
    string selectedLocation;
    string selectedVehicleType;

    O_book(vector<unique_ptr<parkingspot>>& spots, avail& AVAILABLE) {
        try {
            int locationChoice;

            cout << "1. Temple\n2. Mall\n3. Park\nEnter parking location choice: ";
            cin >> locationChoice;
            selectedLocation = getLocation(locationChoice);

            int vehicleTypeChoice;
            cout << "1. Two-wheeler\n2. Four-wheeler\n3. electric\n4. Bus/Other\nEnter vehicle type: ";
            cin >> vehicleTypeChoice;
            selectedVehicleType = getVehicleTypeString(vehicleTypeChoice);

            cout << "Enter number of days: ";
            cin >> day;
            if (day < 0) {
                throw invalid_argument("Number of days cannot be negative.");
            }

            cout << "Enter number of hours: ";
            cin >> hour;
            if (hour < 0) {
                throw invalid_argument("Number of hours cannot be negative.");
            }

            cout << "Enter vehicle number: ";
            cin >> vehicle;

            cout << "\nProcessing your booking for " << selectedVehicleType
                 << " at " << selectedLocation << "...\n";

            // Decrement the available parking spots for the selected type and location
            AVAILABLE.decrementAvailableCount(selectedVehicleType, selectedLocation);

            cout << "Booking successful! Spot reserved.\n";
            cout << "\nUpdated Parking Spot Availability:\n";
            AVAILABLE.check();
        } catch (const exception& e) {
            cerr << "Error in O_book constructor: " << e.what() << endl;
        }
    }

    int amt() {
        try {
            int price = hour * 40 + day * 1000;
            int t_price = loadTotalRevenue();

            ofstream revenueFile("revenue.txt", ios::trunc);
            if (!revenueFile) {
                throw runtime_error("Revenue file could not be opened.");
            }
            revenueFile << price + t_price << endl;
            revenueFile.close();

            return price;
        } catch (const exception& e) {
            cerr << "Error in amt: " << e.what() << endl;
            return 0;  // Return 0 in case of errors
        }
    }
};
