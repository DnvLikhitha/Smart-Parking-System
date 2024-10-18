#include <iostream>
#include <stdexcept>
#include <map>  
using namespace std;

class Reserve_Time {
protected:
    struct fees {
        int hour;
        int day;
        int month;
        int fine;
    };
    
    int cost, h, d, ty;
    map<int, fees> fee_structure;  

public:
    Reserve_Time() {
        fee_structure[2] = {20, 450, 4500, 210};  // Two-wheeler
        fee_structure[4] = {30, 500, 5000, 210};  // Four-wheeler
        fee_structure[0] = {35, 700, 7000, 210};  // Bus
    }

    void get_reservation_details() {
        try {
            cout << "How many hours do you want to reserve: ";
            cin >> h;

            if (cin.fail()) {
                throw invalid_argument("Invalid input! Please enter a valid number for hours.");
            }

            cout << "How many days do you want to reserve: ";
            cin >> d;
            if (cin.fail()) {
                throw invalid_argument("Invalid input! Please enter a valid number for days.");
            }
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
            exit(1);
        }
    }

    int price(int type) {  // 2 -> Two-wheeler, 4 -> Four-wheeler, 0 -> Bus
        try {
            ty = type;  
            if (fee_structure.find(type) != fee_structure.end()) {
                fees selected_fees = fee_structure[type];
                cost = h * selected_fees.hour + d * selected_fees.day;
                return cost;
            } else {
                throw runtime_error("Unexpected error in price calculation: Invalid vehicle type.");
            }
        } catch (const runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
            exit(1);
        } catch (const exception& e) {
            cerr << "An unexpected error occurred: " << e.what() << endl;
            exit(1);
        }
    }

    int getHours() const {
        return h;
    }

    int getDays() const {
        return d;
    }

    int getType() const {
        return ty;
    }
};

class Recipt : public Reserve_Time {
public:
    Recipt(Reserve_Time &res) {
        
        h = res.getHours();
        d = res.getDays();
        ty = res.getType();
    }

    void r_print() {
        for (int i = 0; i < 25; i++)
            cout << "*";
        cout << "\n      Temp Name" << endl;
        cout << "      1234567890" << endl;
        cout << "       Rs. " << price(getType()) << endl;
        for (int i = 0; i < 25; i++)
            cout << "*";
        cout << endl;
    }
};

int main() {
    try {
        Reserve_Time C;

        C.get_reservation_details();

        cout << "Rs. " << C.price(2) << endl;  // 2 -> Two-wheeler

        Recipt R(C);
        R.r_print();
    } catch (const exception &e) {
        cerr << "An unexpected error occurred in main: " << e.what() << endl;
        return 1;
    }

    return 0;
}
