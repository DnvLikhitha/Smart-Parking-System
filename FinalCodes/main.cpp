#include "merged.cpp"
#include "change_password.cpp"

void loadSpotsFromFile(vector<unique_ptr<parkingspot>>& spots) {
    try {
        spots.clear();// Delete previous value in spot
        ifstream inFile("spots.txt");// Open the stop file
        // If file dont exist
        if (!inFile) {
            cout << "No previous spot data found. Starting fresh.\n";
            Spot_change reg;
            reg.update_spot();// create file if didnt exist
            
            return;
        }
        int spotId;
        string type, locality;
        // load data from fole
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
        Reserve_Time parkingRates;// Reserve_Time object
        Note parkingNote;// Note object
        password change_pass;// password Object
        vector<unique_ptr<parkingspot>> spots; 
        int revenue = 0;// Keep revenue as 0
        avail AVAILABLE(spots);// avail Object
        bool value = false;// Check if value loaded

        try {
            ifstream spotFile("spots.txt");// Open file
            if (spotFile.good()) {
                loadSpotsFromFile(spots);// Load from file
                cout << "Loaded parking spots from file." << endl;
                value = true;// Value loaded
            }
        } catch (const exception& e) {
            cerr << "Error loading parking spots: " << e.what() << endl;
        }

        while (true) {// Forever loop
            int userType;
            // Input for login type
            cout << "Select login type:\n1. User Login\n2. Owner Login\nEnter choice: ";
            cin >> userType;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // User Login
            if (userType == 1) {
                if (value) {
                    try {
                        // Get User preferences
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
                            //Find the spot for vehicle
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
                        // If no spot is found
                        if (!spotFound) {
                            cout << "No suitable " << spotPreference << " spot available for your vehicle type." << endl;
                        } else {
                            srand(static_cast<unsigned int>(time(0)));
                            // Random referance number
                            int referenceNumber = rand() % 100000 + 1;
                            cout << "\nAfter parking is complete, press any key to exit.\n";
                            // Wait till user exit the parking
                            system("pause");

                            Time exit;
                            // Get exit time
                            exit.setCurrentTime();
                            // Calculate time parked
                            Calculate duration(parkingNote.get_entry_time(user.getVehicleNo()), exit);
                            // Calculate the cost
                            int cost = parkingRates.price(user.getVehicleType(), duration.getHourDiff(), duration.getDayDiff(), revenue);
                            revenue += cost;
                            // Print the recipt
                            Receipt receipt(user.getVehicleNo(), vehicleType, duration.getHourDiff(), duration.getDayDiff(), cost, assignedSpotID, referenceNumber, user.loc);
                            receipt.printReceipt();
                        }
                    } catch (const exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                } else {
                    // If there is no spot declared by Owner
                    cout << "Parking spot not open for booking!\n";
                }
            } else if (userType == 2) {
                // Owner login
                try {
                    O_login owner;
                    avail AVAILABLE(spots);
                    if (value) {
                        int t;
                        bool active = true;
                        while (active) {
                            // Option for Owner booking
                            cout << "\n1. Book\n2. Revenue\n3. Availability\n4. Change Password\n5. Change Spot details\n6. Display entries\n7.Go to menu\nWhat do you want to do: ";
                            cin >> t;

                            if (cin.fail()) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                                throw invalid_argument("Invalid input for menu option.");
                            }

                            switch (t) {
                                case 1: {
                                    // Owner booking
                                    O_book book(spots, AVAILABLE);  
                                    int cost = book.amt();
                                    // Add the revenue
                                    revenue += cost; 
                                    srand(static_cast<unsigned int>(time(0)));
                                    // Get random referance number
                                    int referenceNumber = rand() % 100000 + 1;
                                    // Print the recipt
                                    Receipt receipt(book.vehicle, book.selectedVehicleType, book.hour, 
                                                    book.day, cost, AVAILABLE.id(book.selectedVehicleType, book.selectedLocation), referenceNumber, book.selectedLocation);
                                    receipt.printReceipt();
                                    break;
                                }
                                case 2:
                                    // Get the revenue
                                    cout << "The total revenue is: " << owner.loadTotalRevenue() << "\n";
                                    cout << "Do you want to make revenue 0? (1-Yes / 0-No): ";
                                    //Make revenue as 0
                                    int temp;
                                    cin >> temp;
                                    if (temp == 1) {
                                        // Discard all value and keep 0
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
                                //  Check all available spot
                                    AVAILABLE.check();
                                    break;
                                case 4:
                                // Change the password
                                    change_pass.update();
                                    break;
                                case 5:{
                                    // Update the spot details
                                    Spot_change reg;
                                    reg.update_spot();
                                    loadSpotsFromFile(spots);
                                    avail AVAILABLE(spots);
                                    break;
                                    }
                                case 6:
                                    // Display all users who enterd and exited
                                    parkingNote.display_entries();
                                    break;
                                case 7:
                                    // Exit the owner login
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
            // Exit the whole code
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
