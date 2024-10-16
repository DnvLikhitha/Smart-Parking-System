# Smart Parking System
_The Smart Parking System project focuses on creating a real-time solution to help operators easily find available parking spots and manage reservations across the city._

# Topics Covered
1) Classes & Objects
2) Inheritance
3) Polymorphism
4) Exception Handling
5) STL

# Description
Develop a real-time smart parking system that helps users find available parking spots and manage reservations for parking spaces in a city.

# Features
1) Parking Spot Search
2) Reservation Management
3) Vehicle Type Support


# Parking Spot Search
  _Base class_
  1) Vehicle(Note the vehicle number and vehicle type)
  2) Parking place(Which place and which lot is available)


  _Derived class_
  1) Parking lot number
  2) User(Name and basic detail of user)
  3) Reservation (Reservation time and check if the reservation time exceeds)
  
  
  _Attributes_
  1) Finding the avalable spot
  2) Check the vehicle type and if it can fit in that space
  3) Reserve spot


# Reservation Management
  _Base Class_
  1) ParkingSpot
  _Derived Class_
  1) CompactSpot
  2) LargeSpot
  3) ElectricSpot
  4) disabledSpot


  _Attributes_
  1) spotId
  2) isAvailable
  3) spotType
  4) isSuitableFor


# Entry Exit
  _Base class_
  1) Price chart
  2) Entry exit time
  _Derived class_
  1) Calculation for payment

  _Attributes_
  1) Is payment done(Yes no type)
  2) Costomer Id
  3) Print the recipt
  4) Move to new costomer
