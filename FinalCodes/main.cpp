#include "merged.cpp"
#include "change_password.cpp"
#include "change_spot.cpp"

void loadSpotsFromFile(vector<unique_ptr<parkingspot>>& spots) {
    try {
        spots.clear();
        ifstream inFile("spots.txt");
        if (!inFile) {
            cerr << "No previous spot data found. Starting fresh.\n";
            Spot_change reg;
            reg.update_spot();
            return;
        }
        int spotId;
        string type, locality;

        while (inFile >> spotId >> type >> locality) {
            if (type == "small") {
                spots.push_back(make_unique<small>(spotId, locality));
            } else if (type == "compact") {
                spots.push_back(make_unique<compact>(spotId, locality));
            } else if (type == "large") {
                spots.push_back(make_unique<large>(spotId, locality));
            } else if (type == "electric") {
                spots.push_back(make_unique<electric>(spotId, locality));
            } else if (type == "disabled") {
                spots.push_back(make_unique<disabled>(spotId, locality));
            }
        }
        cout<<"\nFetch data from file sucessfully"<<endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    try {
        Reserve_Time parkingRates;
        Note parkingNote;
        password change_pass;
        vector<unique_ptr<parkingspot>> spots; 
        int revenue = 0;
        avail AVAILABLE(spots);
        bool value = false;

        try {
            ifstream spotFile("spots.txt");
            if (spotFile.good()) {
                loadSpotsFromFile(spots);
                cout << "Loaded parking spots from file." << endl;
                value = true;
            }
        } catch (const exception& e) {
            cerr << "Error loading parking spots: " << e.what() << endl;
        }

        while (true) {
            int userType;
            cout << "Select login type:\n1. User Login\n2. Owner Login\nEnter choice: ";
            cin >> userType;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (userType == 1) {
                if (value) {
                    try {
                        int spotChoice;
                        cout << "Select the type of spot you need:\n";
                        cout << "1) Regular\n2) Electric\n3) Disabled\nEnter choice: ";
                        cin >> spotChoice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                        if (spotChoice < 1 || spotChoice > 3) {
                            throw invalid_argument("Invalid spot choice.");
                        }

                        U_login user(spots);
                        Time entry;
                        entry.setCurrentTime();
                        parkingNote.add_entry(user.getVehicleNo(), entry);

                        string spotPreference;
                        switch(spotChoice) {
                            case 1:
                                spotPreference = "Regular";
                                break;
                            case 2:
                                spotPreference = "electric";
                                break;
                            case 3:
                                spotPreference = "Disabled";
                                break;
                        }

                        string vehicleType = parkingRates.getVehicleType(user.getVehicleType());
                        cout << "Attempting to reserve a " << spotPreference << " spot for vehicle type: " << vehicleType << endl;

                        bool spotFound = false;
                        int assignedSpotID = -1;

                        for (auto& spot : spots) {
                            if (spot->check_availability()) {
                                if (spotPreference == "electric" && spot->spotType == "electric") {
                                    spot->reservation(vehicleType,true);
                                    assignedSpotID = spot->getSpotid();
                                    spotFound = true;
                                    break;
                                } else if (spotPreference == "Disabled" && spot->spotType == "Disabled") {
                                    spot->reservation(vehicleType, true);
                                    assignedSpotID = spot->getSpotid();
                                    spotFound = true;
                                    break;
                                } else if (spotPreference == "Regular" && spot->isSuitable(vehicleType) && 
                                        spot->spotType != "electric" && spot->spotType != "Disabled") {
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

                            Receipt receipt(user.getVehicleNo(), vehicleType, duration.getHourDiff(), duration.getDayDiff(), cost, assignedSpotID, referenceNumber, user.loc);
                            receipt.printReceipt();
                        }
                    } catch (const exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                } else {
                    cout << "Parking spot not open for booking!\n";
                }
            } else if (userType == 2) {
                try {
                    O_login owner;
                    avail AVAILABLE(spots);
                    if (value) {
                        int t;
                        bool active = true;
                        while (active) {
                            cout << "\n1. Book\n2. Revenue\n3. Availability\n4. Change Password\nChange Spot details\n6. EXIT\nWhat do you want to do: ";
                            cin >> t;

                            if (cin.fail()) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                                throw invalid_argument("Invalid input for menu option.");
                            }

                            switch (t) {
                                case 1: {
                                    O_book book(spots, AVAILABLE);  
                                    int cost = book.amt();
                                    revenue += cost; 
                                    srand(static_cast<unsigned int>(time(0)));
                                    int referenceNumber = rand() % 100000 + 1;
                                    Receipt receipt(book.vehicle, book.selectedVehicleType, book.hour, 
                                                    book.day, cost, AVAILABLE.id(book.selectedVehicleType, book.selectedLocation), referenceNumber, book.selectedLocation);
                                    receipt.printReceipt();
                                    break;
                                }
                                case 2:
                                    cout << "The total revenue is: " << owner.loadTotalRevenue() << "\n";
                                    cout << "Do you want to make revenue 0? (1-Yes / 0-No): ";
                                    int temp;
                                    cin >> temp;
                                    if (temp == 1) {
                                        ofstream revi("revenue.txt", ios::out);
                                        if (revi.is_open()) {
                                            revi << "0";
                                            revi.close();
                                            revenue = 0;
                                            cout << "Revenue set to 0.\n";
                                        } else {
                                            cout << "Error opening revenue file.\n";
                                        }
                                    }
                                    break;
                                case 3:
                                    AVAILABLE.check();
                                    break;
                                case 4:
                                    change_pass.update();
                                    break;
                                case 5:{
                                    Spot_change reg;
                                    reg.update_spot();
                                    loadSpotsFromFile(spots);
                                    avail AVAILABLE(spots);
                                    break;
                                    }
                                case 6:
                                    active = false;
                                    break;
                                default:
                                    cout << "Invalid option.\n";
                            }
                        }
                    }
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
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
    } catch (const exception& e) {
        cerr << "Critical error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
