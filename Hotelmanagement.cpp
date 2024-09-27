#include <iostream>
#include <string>
#include <map> //this is used to store username and password in the key value pairs
#include <regex> //validates username and password

#define MAX_ROOMS 100
#define ADMIN_PASSWORD "2580admin" // Changed to string
using namespace std;

// Class to handle user login details
class User
{
public:
    string username;
    string password;
    // Encapsulation: User class encapsulates the user information.
};

// Class Customer
class Customer
{
public: // Encapsulation: Customer class encapsulates customer-related information.
    string name;
    string address;
    string phone;
    string from_date;
    string to_date;
    float payment_advance;
    int booking_id;
    string feedback;
};

// Class Room
class Room
{
public:
    string type;
    string stype;
    string ac;
    int roomNumber;
    int rent;
    int status;

    Customer cust; // Abstraction: The methods defined here abstract the functionality of room management.
    Room addRoom(int);
    void searchRoom(int);
    void deleteRoom(int);
    void displayRoom(const Room &);
};

// Global Declarations
Room rooms[MAX_ROOMS];
int roomCount = 0;

// Function Prototypes for Login System
void createAccount(map<string, string> &userDatabase);
bool login(map<string, string> &userDatabase, string &loggedInUser);
bool validateUsername(const string &username);
bool validatePassword(const string &password);

// Function for adding room
Room Room::addRoom(int rno)
{
    Room room;
    room.roomNumber = rno;
    cout << "\nType AC/Non-AC (A/N): ";
    cin >> room.ac;
    cout << "\nType Comfort (N/L): ";
    cin >> room.type;
    cout << "\nType Size (B/S): ";
    cin >> room.stype;
    cout << "\nDaily Rent: ";
    cin >> room.rent;
    room.status = 0;

    cout << "\nRoom Added Successfully!";
    return room;
}

void Room::searchRoom(int rno)
{
    bool found = false;
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].roomNumber == rno)
        {
            found = true;
            if (rooms[i].status == 1)
            {
                cout << "\nSorry, the Room is Reserved";
            }
            else
            {
                cout << "\nRoom is available";
            }
            displayRoom(rooms[i]);
            break;
        }
    }
    if (!found)
    {
        cout << "\nSorry, Room not found";
    }
}

void Room::displayRoom(const Room &tempRoom)
{
    cout << "\nPlease fill out the details of room. ";
    cout << "\nRoom Number: " << tempRoom.roomNumber;
    cout << "\nType AC/Non-AC (A/N): " << tempRoom.ac;
    cout << "\nType Comfort (N/L): " << tempRoom.type;
    cout << "\nType Size (B/S): " << tempRoom.stype;
    cout << "\nRent: " << tempRoom.rent;
}

// Hotel Management Class Inheritance
// Inheritance: HotelMgnt class inherits from Room class, gaining access to its properties and methods.
class HotelMgnt : public Room
{
public:
    void checkIn();
    void getAvailRoom();
    void searchCustomer(const string &);
    void checkOut(int);
    void guestSummaryReport();
    void collectFeedback(int);
    void showCheckedInCustomers(); // New function for admin to see checked-in customers
};

void HotelMgnt::guestSummaryReport()
{
    if (roomCount == 0)
    {
        cout << "\nNo Guest in Hotel!";
        return;
    }
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].status == 1)
        {
            cout << "\nCustomer Name: " << rooms[i].cust.name;
            cout << "\nRoom Number: " << rooms[i].roomNumber;
            cout << "\nAddress: " << rooms[i].cust.address;
            cout << "\nPhone: " << rooms[i].cust.phone;
            cout << "\n---------------------------------------";
        }
    }
}

void HotelMgnt::checkIn()
{
    int rno;
    bool found = false;

    cout << "\nPlease fill out the details of room. ";

    cout << "\nEnter Room number: ";
    cin >> rno;

    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].roomNumber == rno)
        {
            found = true;
            if (rooms[i].status == 1)
            {
                cout << "\nRoom is already Booked";
                return;
            }

            cout << "\nEnter Booking ID: ";
            cin >> rooms[i].cust.booking_id;
            cout << "\nEnter Customer Name: ";
            cin.ignore();
            getline(cin, rooms[i].cust.name);
            cout << "\nEnter Address: ";
            getline(cin, rooms[i].cust.address);
            cout << "\nEnter Phone: ";
            getline(cin, rooms[i].cust.phone);
            cout << "\nEnter From Date: ";
            getline(cin, rooms[i].cust.from_date);
            cout << "\nEnter To Date: ";
            getline(cin, rooms[i].cust.to_date);
            cout << "\nEnter Advance Payment: ";
            cin >> rooms[i].cust.payment_advance;

            rooms[i].status = 1;

            cout << "\nCustomer Checked-in Successfully!";
            break;
        }
    }

    if (!found)
    {
        cout << "\nRoom not found!";
    }
}

void HotelMgnt::getAvailRoom()
{
    bool found = false;
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].status == 0)
        {
            displayRoom(rooms[i]);
            found = true;
        }
    }
    if (!found)
    {
        cout << "\nAll rooms are reserved";
    }
}

void HotelMgnt::searchCustomer(const string &pname)
{
    bool found = false;
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].status == 1 && rooms[i].cust.name == pname)
        {
            cout << "\nCustomer Name: " << rooms[i].cust.name;
            cout << "\nRoom Number: " << rooms[i].roomNumber;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "\nPerson not found.\n";
    }
}

void HotelMgnt::collectFeedback(int roomNum)
{
    bool found = false;
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].status == 0 && rooms[i].roomNumber == roomNum)
        {
            found = true;
            cout << "\nPlease provide your feedback: ";
            cin.ignore();
            getline(cin, rooms[i].cust.feedback);
            cout << "\nThank you for your feedback!";
            break;
        }
    }
    if (!found)
    {
        cout << "\nRoom not found or not checked out yet.";
    }
}

void HotelMgnt::checkOut(int roomNum)
{
    bool found = false;
    int days;
    float billAmount;
    char paymentOption;
    string cardNumber;
    int pin;

    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].status == 1 && rooms[i].roomNumber == roomNum)
        {
            found = true;
            cout << "\nEnter Number of Days: ";
            cin >> days;
            billAmount = days * rooms[i].rent;

            cout << "\nCustomer Name: " << rooms[i].cust.name;
            cout << "\nRoom Number: " << rooms[i].roomNumber;
            cout << "\nTotal Amount Due: " << billAmount;
            cout << "\nAdvance Paid: " << rooms[i].cust.payment_advance;
            cout << "\nTotal Payable: " << billAmount - rooms[i].cust.payment_advance;

            cout << "\nSelect Payment Option (C for Card, H for Cash): ";
            cin >> paymentOption;

            if (paymentOption == 'C' || paymentOption == 'c')
            {
                do
                {
                    cout << "\nEnter 16-digit Card Number: ";
                    cin >> cardNumber;
                    if (cardNumber.length() != 16)
                    {
                        cout << "\nInvalid card number!";
                    }
                } while (cardNumber.length() != 16);

                do
                {
                    cout << "\nEnter 4-digit PIN: ";
                    cin >> pin;
                    if (pin < 1000 || pin > 9999)
                    {
                        cout << "\nInvalid PIN!";
                    }
                } while (pin < 1000 || pin > 9999);

                cout << "\nPayment Successful!";
            }
            else
            {
                cout << "\nPayment Received in Cash!";
            }

            rooms[i].status = 0;
            cout << "\nCheckout Completed Successfully!";
            break;
        }
    }

    if (!found)
    {
        cout << "\nRoom not found!";
    }
}

// Admin function to show checked-in customer details
void HotelMgnt::showCheckedInCustomers()
{
    string password;
    cout << "\nEnter Admin Password to view details: ";
    cin >> password;

    if (password == ADMIN_PASSWORD) // Password verification
    {
        cout << "\nDisplaying Checked-in Customer Details:";
        bool found = false;
        for (int i = 0; i < roomCount; i++)
        {
            if (rooms[i].status == 1)
            {
                found = true;
                cout << "\nCustomer Name: " << rooms[i].cust.name;
                cout << "\nRoom Number: " << rooms[i].roomNumber;
                cout << "\nBooking ID: " << rooms[i].cust.booking_id;
                cout << "\nAddress: " << rooms[i].cust.address;
                cout << "\nPhone: " << rooms[i].cust.phone;
                cout << "\n---------------------------------------";
            }
        }
        if (!found)
        {
            cout << "\nNo checked-in customers found.";
        }
    }
    else
    {
        cout << "\nInvalid Password. Access Denied!";
    }
}

// Function to create a new user account
void createAccount(map<string, string> &userDatabase)
{
    string username, password;

    cout << "\nCreate a new account";
    cout << "\nEnter Username: ";
    cin >> username;

    while (!validateUsername(username) || userDatabase.find(username) != userDatabase.end())
    {
        if (!validateUsername(username))
        {
            cout << "\nUsername should be alphanumeric and at least 5 characters long.";
        }
        else
        {
            cout << "\nUsername already exists!";
        }
        cout << "\nEnter Username: ";
        cin >> username;
    }

    cout << "\nEnter Password: ";
    cin >> password;

    while (!validatePassword(password))
    {
        cout << "\nPassword must be at least 8 characters long, contain at least one uppercase letter, one lowercase letter, and one number.";
        cout << "\nEnter Password: ";
        cin >> password;
    }

    userDatabase[username] = password;
    cout << "\nAccount created successfully!";
}

// Function for login
bool login(map<string, string> &userDatabase, string &loggedInUser)
{
    string username, password;
    cout << "\nEnter Username: ";
    cin >> username;
    cout << "\nEnter Password: ";
    cin >> password;

    if (userDatabase.find(username) != userDatabase.end() && userDatabase[username] == password)
    {
        loggedInUser = username;
        return true;
    }
    return false;
}

// Username validation
bool validateUsername(const string &username)
{
    regex usernameRegex("^[a-zA-Z0-9]{5,}$");
    return regex_match(username, usernameRegex);
}

// Password validation
bool validatePassword(const string &password)
{
    regex passwordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$");
    return regex_match(password, passwordRegex);
}

int main()
{
    map<string, string> userDatabase;
    HotelMgnt hotel;
    int choice, rno;
    string pname;
    string loggedInUser;
    bool isLoggedIn = false;

    while (true)
    {
        cout << "\n------- Welcome to YatriNest Hotel Management System -------";
        cout << "\n Please Login or Create an Account if you are a new user.";
        cout << "\n1. Create Account";
        cout << "\n2. Login";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;


        

        switch (choice)
        {
        case 1:
            createAccount(userDatabase);
            break;
        case 2:
            if (login(userDatabase, loggedInUser))
            {
                isLoggedIn = true;
                cout << "\nWelcome, " << loggedInUser << "!";
                while (isLoggedIn)
                {
                    cout << "\n--------- Main Menu ---------";
                    cout << "\n1. Add Room";
                    cout << "\n2. Search Room";
                    cout << "\n3. Check-In";
                    cout << "\n4. Check-Out";
                    cout << "\n5. Show Available Rooms";
                    cout << "\n6. Search Customer";
                    cout << "\n7. Guest Summary Report";
                    cout << "\n8. Collect Feedback";
                    cout << "\n9. Show Checked-in Customers";
                    cout << "\n10. Logout";
                    cout << "\nEnter your choice: ";
                    cin >> choice;

                    switch (choice)
                    {
                    case 1:
                        
                        cout << "\nEnter Room Number: ";
                        cin >> rno;
                        rooms[roomCount] = rooms[roomCount].addRoom(rno);
                        roomCount++;
                        break;
                    case 2:
                        cout << "\nEnter Room Number: ";
                        cin >> rno;
                        hotel.searchRoom(rno);
                        break;
                    case 3:
                        hotel.checkIn();
                        break;
                    case 4:
                        cout << "\nEnter Room Number: ";
                        cin >> rno;
                        hotel.checkOut(rno);
                        break;
                    case 5:
                        hotel.getAvailRoom();
                        break;
                    case 6:
                        cout << "\nEnter Customer Name: ";
                        cin.ignore();
                        getline(cin, pname);
                        hotel.searchCustomer(pname);
                        break;
                    case 7:
                        hotel.guestSummaryReport();
                        break;
                    case 8:
                        cout << "\nEnter Room Number: ";
                        cin >> rno;
                        hotel.collectFeedback(rno);
                        break;
                    case 9:
                        hotel.showCheckedInCustomers();
                        break;
                    case 10:
                        isLoggedIn = false;
                        cout << "\nLogged out successfully!";
                        break;
                    default:
                        cout << choice;
                        cout << "\nInvalid option!";
                        
                    }
                }
            }
            else
            {
                cout << "\nInvalid Username or Password!";
            }
            break;
        case 3:
            cout << "\nExiting... Have a nice day!";
            exit(0);
        default:
            cout << "\nInvalid option!";
        }
    }

    return 0;
}
