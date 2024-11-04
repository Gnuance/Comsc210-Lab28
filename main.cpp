/*
    Lab 24: Goat 3K from Lab 23 implemented as a set
    OBJECTIVES
    Task One: implement Goat 3K as a set instead of a list
*/

#include <iostream>
#include <fstream>
#include <iomanip>
// #include <list>
#include <set>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25, MAX_AGE = 20;

int select_goat(set<Goat> trip);
void delete_goat(set<Goat> &trip);
void add_goat(set<Goat> &trip, string[], string[]);
void display_trip(set<Goat> trip);
int main_menu();                      // outputs prompt and collects user selection
bool isValidOption(string, int, int); // helper function to validate user input

int main()
{
    set<Goat> trip = {};
    srand(static_cast<unsigned int>(time(nullptr)));
    bool again;
    int userSelectedOption = 0; // represents user option chosen from menu

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++])
        ;
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++])
        ;
    fin1.close();

    // prompt user for selection and input
    // user selection 4 is the program exit code
    do
    {
        userSelectedOption = main_menu();
        switch (userSelectedOption)
        {
        case 1:
            add_goat(trip, names, colors);
            break;
        case 2:
            delete_goat(trip);
            break;
        case 3:
            display_trip(trip);
            break;
        default:
            break;
        }
    } while (userSelectedOption != 4);

    return 0;
}

// main menu fuction for Task 2
int main_menu()
{
    string userInput = "";

    do
    {
        // output prompt
        cout << "*** GOAT MANAGER 3001 ***" << "\n"
             << "[1] Add a goat" << "\n"
             << "[2] Delete a goat" << "\n"
             << "[3] List goats" << "\n"
             << "[4] Quit" << "\n"
             << "Choice --> ";
        getline(cin, userInput); // get user input as string and test
        cout << "\n";
    } while (!isValidOption(userInput, 1, 4));

    // if isValidOption passed, stoi(userInput) has already been tested and is safe
    return stoi(userInput);
}

// return t/f if userInput is a valid int between min and max
// helper function for main_menu and delete_goat
// WARNING: stoi() will convert a double to an int or any string following an int.
// Ex: stoi("2.9") will return 2 and so will stoi("2tGznso"), etc.
bool isValidOption(string userInput, int minOption, int maxOption)
{
    int selectedOption = 0;
    try
    {
        selectedOption = stoi(userInput);
    }
    catch (const std::exception &e)
    {
        cout << "Invalid input: Please enter a valid integer." << "\n\n";
        return false;
    }

    // if userInput is an int but outside expected range
    if (selectedOption < minOption || selectedOption > maxOption)
    {
        cout << "Invalid input: Please enter an integer between " << minOption << " and " << maxOption << "." << "\n\n";
        return false;
    }

    return true;
}

// adds random goat to trip
void add_goat(set<Goat> &trip, string names[], string colors[])
{
    string name = names[rand() % SZ_NAMES];
    int age = rand() % MAX_AGE + 1;
    string color = colors[rand() % SZ_COLORS];

    trip.insert(Goat(name, age, color));

    cout << "Added to trip: " << name << " (" << age << ", " << color << ")" << "\n\n";
}

// directly outputs trip to console
void display_trip(set<Goat> trip)
{
    if (trip.size() < 1)
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }
    
    int count = 0;
    cout << "Trip:" << "\n";
    for (auto it = trip.begin(); it != trip.end(); it++)
    {
        cout << "\t[" << ++count << "] " << it->get_name() << " (" << it->get_age() << ", " << it->get_color() << ")" << "\n";
    }
    cout << "\n";
}

// lets user select a Goat to delete
void delete_goat(set<Goat> &trip)
{
    // guard statement in case of empty list
    if (trip.size() < 1)
    {
        cout << "Trip is empty, no goats to delete." << "\n\n";
        return;
    }

    string userInput = "";
    auto it = trip.begin(); // iterator to first element

    // display Goats in current trip available for selection
    // prompt user for index to delete
    // WARNING: index DISPLAYED TO USER begins at 1
    do
    {
        display_trip(trip);
        cout << "Please enter index to delete --> ";
        getline(cin, userInput); // get user input as string
        // make sure user value is valid and delete
        if (isValidOption(userInput, 1, trip.size()))
        {
            advance(it, stoi(userInput) - 1); // -1 because index displayed to users starts at 1
            cout << "\n" << "Updated Trip After Removing: " << it->get_name() << " (" << it->get_age() << ", " << it->get_color() << ")\n";
            trip.erase(it);            
            display_trip(trip);
            break;
        }
    } while (true);
}
