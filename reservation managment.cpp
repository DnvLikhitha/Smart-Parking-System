#include<iostream>
#include<vector>
#include<memory>
#include<string>
using namespace std;

class parkingspot {
    protected:
    int id;
    bool isAvailable;
    string spotType;
    string locality;
    public:
    parkingspot(int spotid, string type,string place)//creating a parking spot of required type
    {
        id= spotid;
        spotType=type;
        isAvailable=true;
        locality=place;
    }
    void printdetails()
    {
        cout<<"Spot ID:"<<id<<"\n Availability: "<<isAvailable<<"\n Spot Type:"<<spotType<<"\nLocality: "<<locality<<endl;
    }
    virtual bool isSuitable (string Vtype)=0;
    int getSpotid()
    {
        return id;
    }
    bool check_availability()
    {
        return isAvailable;
    }
    void reservation (string vtype)
    {
        string Ans;
        bool k=check_availability();
        if(k==false)
        {
            cout<<"Spot not available"<<endl;
            return;
        }
        k=isSuitable(vtype);
        if(k==false)
        {
            cout<<"Spot not suitable"<<endl;
            return;
        }
        cout<<"Would you like to book spot number "<<id<<"?"<<endl;
        cin>>Ans;
        if(Ans.compare("Yes")==0)
        {
            cout<<"Reservation complete"<<endl;
            isAvailable=false;

        }

        
    }

    

};
class small: public parkingspot{
    public:
    small(int spotid,string place):parkingspot(spotid,"Compact",place){}
    bool isSuitable (string Vtype) override
    {
        if(Vtype=="Two-wheeler")
        return true;
        else
        return false;
    }
};
class compact: public parkingspot{
    public:
    compact(int spotid,string place):parkingspot(spotid,"Compact",place){}
    bool isSuitable (string Vtype) override
    {
        if(Vtype=="four-wheeler")
        return true;
        else
        return false;
    }


};
class large: public parkingspot{
    public:
    large(int spotid,string place):parkingspot(spotid,"Large",place){}
    bool isSuitable (string Vtype) override
    {
        if(Vtype=="others")
        return true;
        else
        return false;
    }
};
class electric: public parkingspot{
    public:
    electric(int spotid,string place):parkingspot(spotid,"Eletric",place){}
    bool isSuitable (string Vtype) override
    {
        if(Vtype=="eletric")
        return true;
        else
        return false;
    }

};
class disabled:public parkingspot{
    public:
    disabled(int spotid,string place):parkingspot(spotid,"Disabled",place){}
    bool isSuitable (string Vtype) override
    {
        string ans;
        cout<<"Are you carrying a disabled person?\n Yes or No"<<endl;
        cin>>ans;
        if(ans=="Yes")
        return true;
        else
        return false;
    }
    
};
int main()
{
vector<unique_ptr<parkingspot>> p;
int sspots,cspots,lspots,espots,dspots,spotid=0;
string place;
int ntemp,nmall,npark;
int k=1;
while(k<4)
{
    if(k==1)
    {
        cout<<"Enter the plot details near the Temple"<<endl;
        place="Temple";
    }
    else if(k==2)
    {
        cout<<"Enter the plot details near the Mall"<<endl;
        ntemp=spotid-1;
        place="Mall";
    }
    else if(k==3)
    {
        cout<<"Enter the plot details near the Park"<<endl;
        nmall=spotid-1;
        place="Park";
    }
    cout<<"Enter the number of small spots"<<endl;
cin>>sspots;
for(int i=0;i<sspots;i++)
{
    spotid++;
    p.push_back(make_unique<small>(spotid,place));
}
cout<<"Enter the number of compact spots"<<endl;
cin>>cspots;
for(int i=0;i<cspots;i++)
{
    spotid++;
    p.push_back(make_unique<compact>(spotid,place));
}
cout<<"Enter the number of large spots"<<endl;
cin>>lspots;
for(int i=0;i<lspots;i++)
{
    spotid++;
    p.push_back(make_unique<large>(spotid,place));
}
cout<<"Enter the number of eletric spots"<<endl;
cin>>espots;
for(int i=0;i<espots;i++)
{
spotid++;
p.push_back(make_unique<electric>(spotid,place));
}
cout<<"Enter the number of disabled spots"<<endl;
cin>>dspots;
for(int i=0;i<dspots;i++)
{
    spotid++;
    p.push_back(make_unique<disabled>(spotid,place));
}
k++;
npark=spotid-1;
}
for(int i=0;i<p.size();++i)
{
    p[i]->printdetails();
}
cout<<p[0]->isSuitable("Two-wheeler");
int pAvailable=spotid;//keeping up with number of vacant spots
while(pAvailable>= 0.1*spotid)
{
    string vtype;//their vechicle type
    //searching 
    vtype="Two-wheeler";
    p[0]->reservation(vtype);
    pAvailable--;
}
return 0;
}
