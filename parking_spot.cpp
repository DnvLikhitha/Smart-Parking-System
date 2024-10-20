#include <iostream>
#include <vector>
#include <string>
using namespace std;

//vehicle details
class Vehicle {
protected:
    string vehicleNo;     // Vehicle number
    string vehicleType;   // Vehicle type
public:
    Vehicle(const string& no, const string& type){
        vehicleNo=no;
        vehicleType=type;
    }

    string getVehicleNo() const { 
        return vehicleNo; 
    }
    string getVehicleType() const { 
        return vehicleType; 
    }
    
    void printDetails() const {
        cout << "Vehicle Number: " << vehicleNo << endl;
        cout << "Vehicle Type: " << vehicleType << endl;
    }
};

//user details
class User {
protected:
    string userName;      // Name of the user
    string mobileNumber;  // Mobile number of the user
    Vehicle vehicle;      
public:
    User(const string& name, const string& mobile, const Vehicle& v){
        userName=name;
        mobileNumber=mobile;
        vehicle=v;
    }
    
    string getUserName() const { 
        return userName; 
    }
    string getMobileNumber() const { 
        return mobileNumber; 
    }
    Vehicle getVehicle() const { 
        return vehicle; 
    }
    
    void printDetails() const {
        cout << "User Name: " << userName << endl;
        cout << "Mobile Number: " << mobileNumber << endl;
        vehicle.printDetails();
    }
};

// Class representing a specific parking location
class ParkingPlace {
protected:
    string placeName;
    int availableLots;
public:
    ParkingPlace(const string& place, int lots){
       placeName=place;
       availableLots=lots;
    }
    
    bool isLotAvailable() const { 
        return availableLots > 0; 
    }
    
    void reserveLot() {
        if (availableLots > 0) {
            availableLots--;
            cout << "Parking spot reserved at " << placeName << ". Remaining lots: " << availableLots << endl;
        } else {
            cout << "No available parking spots at " << placeName << endl;
        }
    }
    
    string getPlaceName() const { 
        return placeName; 
    }
};
