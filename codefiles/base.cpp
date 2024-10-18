#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

class Current_Time {
    int cost,h,d;
    string entryTime,exitTime;
public:

    void enter(){
        ofstream file("time.txt", ios::app);
        if (file.is_open()) {
            auto now = chrono::system_clock::now();
            time_t currentTime = chrono::system_clock::to_time_t(now);
            entryTime=currentTime;
            file << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S") << endl;
            file.close();
        } else {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    void calculateTimeDifference() {
        ifstream file("time.txt");
        string lastLoggedTime;
        if (!file.is_open()) {
            cerr << "Error opening the file." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            lastLoggedTime = line;
        }
        file.close();

        if (lastLoggedTime.empty()) {
            cerr << "File is empty or no time data found." << endl;
            return;
        }

        tm enteredTime = {};
        istringstream ss(lastLoggedTime);
        ss >> get_time(&enteredTime, "%Y-%m-%d %H:%M:%S");

        if (ss.fail()) {
            cerr << "Invalid format in file. Please use the format YYYY-MM-DD HH:MM:SS." << endl;
            return;
        }
        time_t enteredTimeT = mktime(&enteredTime);
        auto enteredTimePoint = chrono::system_clock::from_time_t(enteredTimeT);
        auto now = chrono::system_clock::now();
        auto duration = now - enteredTimePoint;
        auto hoursTotal = chrono::duration_cast<chrono::hours>(duration).count();
        auto days = hoursTotal / 24;
        auto hours = hoursTotal % 24; 
        auto minutes = chrono::duration_cast<chrono::minutes>(duration).count() % 60;
        auto seconds = chrono::duration_cast<chrono::seconds>(duration).count() % 60;

            h=hours;
            d=days;
    }

    int price(){
        cost= h*20+d*500;
        cout<<entryTime;
        return cost;
    }
};

int main() {
    Current_Time C;
    // C.enter();
    C.calculateTimeDifference();
    cout<<"Rs. "<<C.price();
    return 0;
}
