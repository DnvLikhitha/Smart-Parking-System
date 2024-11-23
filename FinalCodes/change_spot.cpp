#include<iostream>
#include <fstream>
using namespace std;
class Spot_change {
        
    public:
    Spot_change(){
            
    }
    void update_spot(){
        ofstream file("spots.txt",  ios::trunc);
        int id,type,locality;
            cout<<"Enter number of spots for 2-wheeler parking at mall: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "small" << " " << "Mall" << endl;
            cout<<"Enter number of spots for Car parking at mall: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "compact" << " " << "Mall" << endl;
            cout<<"Enter number of spots for Bus/Truck parking at mall: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "large" << " " << "Mall" << endl;
            cout<<"Enter number of spots for Electric parking at mall: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "electric" << " " << "Mall" << endl;
            cout<<"Enter number of spots for Disabled parking at mall: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "disabled" << " " << "Mall" << endl;




            cout<<"Enter number of spots for 2-wheeler parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "small" << " " << "Park" << endl;
            cout<<"Enter number of spots for Car parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "compact" << " " << "Park" << endl;
            cout<<"Enter number of spots for Bus/Truck parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "large" << " " << "Park" << endl;
            cout<<"Enter number of spots for Electric parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "electric" << " " << "Park" << endl;
            cout<<"Enter number of spots for Disabled parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "disabled" << " " << "Park" << endl;




            cout<<"Enter number of spots for 2-wheeler parking at Temple: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "small" << " " << "Temple" << endl;
            cout<<"Enter number of spots for Car parking at Park: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "compact" << " " << "Temple" << endl;
            cout<<"Enter number of spots for Bus/Truck parking at Temple: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "large" << " " << "Temple" << endl;
            cout<<"Enter number of spots for Electric parking at Temple: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "electric" << " " << "Temple" << endl;
            cout<<"Enter number of spots for Disabled parking at Temple: ";
            cin>>id;
            for(int i=1;i<=id;i++)
            file << i<< " " << "disabled" << " " << "Temple" << endl;
    }
};
