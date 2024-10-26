#include "merged.cpp"
#include "change_password.cpp"
void saveSpotsToFile(const vector<unique_ptr<parkingspot>>& spots) {
    ofstream outFile("spots.txt");
    for (const auto& spot : spots) {
        outFile << spot->getSpotid() << " " << spot->spotType << " " << spot->locality << endl;
    }
    outFile.close();
}

// Function to load parking spots from a file
void loadSpotsFromFile(vector<unique_ptr<parkingspot>>& spots) {
    ifstream inFile("spots.txt");
    int spotId;
    string type, locality;

    while (inFile >> spotId >> type >> locality) {
        if (type == "Compact") {
            spots.push_back(make_unique<small>(spotId, locality));
        } else if (type == "Four-wheeler") {
            spots.push_back(make_unique<compact>(spotId, locality));
        } else if (type == "Large") {
            spots.push_back(make_unique<large>(spotId, locality));
        } else if (type == "Electric") {
            spots.push_back(make_unique<electric>(spotId, locality));
        } else if (type == "Disabled") {
            spots.push_back(make_unique<disabled>(spotId, locality));
        }
    }
    inFile.close();
}
int main() {
    Reserve_Time parkingRates;
    Note parkingNote;
    password change_pass;
    vector<unique_ptr<parkingspot>> spots; 
    int revenue = 0;
    avail AVAILABLE(spots);
    ifstream spotFile("spots.txt");
    if (spotFile.good()) {
        loadSpotsFromFile(spots);

        cout << "Loaded parking spots from file." << endl;
        value=true;
    }

    while (true) {
        int userType;
        cout << "Select login type:\n1. User Login\n2. Owner Login\nEnter choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            if (value == true) {
                // Get spot preference before user details
                int spotChoice;
                cout << "Select the type of spot you need:\n";
                cout << "1) Regular\n2) Electric\n3) Disabled\nEnter choice: ";
                cin >> spotChoice;
                cin.ignore();

                U_login user;
                Time entry;
                entry.setCurrentTime();
                parkingNote.add_entry(user.getVehicleNo(), entry);

                string spotPreference;
                switch(spotChoice) {
                    case 1:
                        spotPreference = "Regular";
                        break;
                    case 2:
                        spotPreference = "Electric";
                        break;
                    case 3:
                        spotPreference = "Disabled";
                        break;
                    default:
                        spotPreference = "Regular";
                }

                string vehicleType = parkingRates.getVehicleType(user.getVehicleType());
                cout << "Attempting to reserve a " << spotPreference << " spot for vehicle type: " << vehicleType << endl;

                bool spotFound = false;
                int assignedSpotID = -1;

                for (auto& spot : spots) {
                    if (spot->check_availability()) {
                        if (spotPreference == "Electric" && spot->spotType == "Electric" && spot->isSuitable(vehicleType)) {
                            spot->reservation(vehicleType);
                            assignedSpotID = spot->getSpotid();
                            spotFound = true;
                            break;
                        }
                        else if (spotPreference == "Disabled" && spot->spotType == "Disabled") {
                            spot->reservation(vehicleType, true);
                            assignedSpotID = spot->getSpotid();
                            spotFound = true;
                            break;
                        }
                        else if (spotPreference == "Regular" && spot->isSuitable(vehicleType) && 
                                spot->spotType != "Electric" && spot->spotType != "Disabled") {
                            spot->reservation(vehicleType);
                            assignedSpotID = spot->getSpotid();
                            spotFound = true;
                            break;
                        }
                    }
                }

                if (!spotFound) {
                    cout << "No suitable " << spotPreference << " spot available for your vehicle type." << endl;
                } else {
                    srand(static_cast<unsigned int>(time(0)));
                    int referenceNumber = rand() % 100000 + 1;

                    cout << "\nAfter parking is complete, press any key to exit.\n";
                    system("pause");

                    Time exit;
                    exit.setCurrentTime();

                    Calculate duration(parkingNote.get_entry_time(user.getVehicleNo()), exit);
                    int cost = parkingRates.price(user.getVehicleType(), duration.getHourDiff(), duration.getDayDiff(), revenue);
                    revenue += cost;

                    Receipt receipt(user.getVehicleNo(), vehicleType, duration.getHourDiff(), duration.getDayDiff(), cost, assignedSpotID, referenceNumber,user.loc);
                    receipt.printReceipt();
                }
            } else {
                cout << "Parking spot not open for booking!\n";
            }
        }  else if (userType == 2) {
            O_login owner;
avail AVAILABLE(spots);

if (value == true) {
    int t;
    bool active = true;
    while (active) {
        cout << "\n1. Book\n2. Revenue\n3. Availability\n4. Change Password\n5. EXIT\nWhat do you want to do: ";
        cin >> t;
        switch (t) {
            case 1: {
                O_book book(spots, AVAILABLE);  // Pass BOTH spots and AVAILABLE
                
                // Generate booking cost and receipt
                int cost = book.amt();
                revenue += cost;  // Update revenue
                srand(static_cast<unsigned int>(time(0)));
                int referenceNumber = rand() % 100000 + 1;
                Receipt receipt(book.vehicle, book.selectedVehicleType, book.hour, 
                                book.day, cost,AVAILABLE.id(book.selectedVehicleType,book.selectedLocation) , referenceNumber, book.selectedLocation);
                receipt.printReceipt();
                break;
            }
            case 2:
                // Display total revenue
                cout << "The total revenue is: " << owner.loadTotalRevenue() << "\n";
                cout<<"Do you want to make revenue 0?(1-Yes/0-No)";
                break;
            case 3:
                // Show availability of parking spots
                AVAILABLE.check();
                break;
            case 4:
                change_pass.update();
                break;
            case 5:
                // Exit loop
                active = false;
                break;
            default:
                cout << "Invalid option.\n";
        }
    }
}
else {
                int temp;
                ofstream outFile("spots.txt");
                cout << "Enter number of bike parking spot of mall: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                    outFile << i<< " " << "small" << " " << "Mall" << endl;
                    spots.push_back(make_unique<small>(i, "Mall"));
                }
                cout << "Enter number of car parking spot of mall: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                    outFile << i<< " " << "compact" << " " << "Mall" << endl;
                    spots.push_back(make_unique<compact>(i, "Mall"));
                }
                cout << "Enter number of large parking spot of mall: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                    outFile << i<< " " << "large" << " " << "Mall" << endl;
                    spots.push_back(make_unique<large>(i, "Mall"));
                }
                cout << "Enter number of electric spot of mall: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                    outFile << i<< " " << "electric" << " " << "Mall" << endl;
                    spots.push_back(make_unique<electric>(i, "Mall"));
                    }
                cout << "Enter number of disabled parking spot of mall: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "disabled" << " " << "Mall" << endl;
                spots.push_back(make_unique<disabled>(i, "Mall"));
                }               
                cout << "Enter number of bike parking spot of Park: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "small" << " " << "Park" << endl;
                spots.push_back(make_unique<small>(i, "Park"));
                }
                cout << "Enter number of car parking spot of Park: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "compact" << " " << "Park" << endl;
                spots.push_back(make_unique<compact>(i, "Park"));
                }
                cout << "Enter number of large parking spot of Park: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "large" << " " << "Park" << endl;
                spots.push_back(make_unique<large>(i, "Park"));
                }
                cout << "Enter number of electric spot of Park: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "electric" << " " << "Park" << endl;
                spots.push_back(make_unique<electric>(i, "Park"));
                }
                cout << "Enter number of disabled parking spot of Park: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "disabled" << " " << "Park" << endl;
                spots.push_back(make_unique<disabled>(i, "Park"));
                }
                cout << "Enter number of bike parking spot of Temple: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "small" << " " << "Temple" << endl;
                spots.push_back(make_unique<small>(i, "Temple"));
                }
                cout << "Enter number of car parking spot of Temple: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "compact" << " " << "Temple" << endl;
                spots.push_back(make_unique<compact>(i, "Temple"));
                }
                cout << "Enter number of large parking spot of Temple: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "large" << " " << "Temple" << endl;
                spots.push_back(make_unique<large>(i, "Temple"));
                }
                cout << "Enter number of electric spot of Temple: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "electric" << " " << "Temple" << endl;
                spots.push_back(make_unique<electric>(i, "Temple"));
                }
                cout << "Enter number of disabled parking spot of Temple: ";
                cin >> temp;
                for(int i=1;i<=temp;i++){
                outFile << i<< " " << "disabled" << " " << "Temple" << endl;
                spots.push_back(make_unique<disabled>(i, "Temple"));
                }
                value = true;
                
            }
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