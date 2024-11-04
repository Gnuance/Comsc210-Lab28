/*
    Lab 28: Goat 3K from Lab 24
        1. Expand the main menu of this program by adding 8 more options that feature different STL algorithms.

        Design decisions:
            Planning to add:
            1. find()
            2. clear()
            3. accumulate() to get average age of goats
            4. shuffle()
            5. reverse()
            6. any_of()
            7. transform()
            8. for_each()
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm> // for additional container functions
#include <numeric>
#include <set>
#include <random> // required for random engine
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25, MAX_AGE = 20;

void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string[], string[]);
void display_trip(list<Goat> trip);
int main_menu();                      // outputs prompt and collects user selection
bool isValidOption(string, int, int); // helper function to validate user input
void FindGoat(const list<Goat> &trip);
void ClearGoats(list<Goat> &trip);
void AverageAge(const list<Goat> &trip);
void ShuffleGoats(list<Goat> &trip);
void ReverseGoatOrder(list<Goat> &trip);
void AnyGoatsOver10(list<Goat> &trip);
void TransformAgeMinusOne(list<Goat> &trip);
void AddOneYearForEachGoat(list<Goat> &trip);

int main()
{
    list<Goat> trip = {};
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
        case 4:
            FindGoat(trip);
            break;
        case 5:
            ClearGoats(trip);
            break;
        case 6:
            AverageAge(trip);
            break;
        case 7:
            ShuffleGoats(trip);
            break;
        case 8:
            ReverseGoatOrder(trip);
            break;
        case 9:
            AnyGoatsOver10(trip);
            break;
        case 10:
            TransformAgeMinusOne(trip);
            break;
        case 11:
            AddOneYearForEachGoat(trip);
            break;
        default:
            break;
        }
    } while (userSelectedOption != 12);

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
             << "[4] Find goat" << "\n"
             << "[5] Clear goats" << "\n"
             << "[6] Get average age" << "\n"
             << "[7] Shuffle list" << "\n"
             << "[8] Reverse lsit" << "\n"
             << "[9] Any goats over 10" << "\n"
             << "[10] Minus 1 to goat ages" << "\n"
             << "[11] Plus 1 to goat ages" << "\n"
             << "[12] Quit" << "\n"
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
void add_goat(list<Goat> &trip, string names[], string colors[])
{
    string name = names[rand() % SZ_NAMES];
    int age = rand() % MAX_AGE + 1;
    string color = colors[rand() % SZ_COLORS];

    trip.push_back(Goat(name, age, color));

    cout << "Added to trip: " << name << " (" << age << ", " << color << ")" << "\n\n";
}

// directly outputs trip to console
void display_trip(list<Goat> trip)
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
void delete_goat(list<Goat> &trip)
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
            cout << "\n"
                 << "Updated Trip After Removing: " << it->get_name() << " (" << it->get_age() << ", " << it->get_color() << ")\n";
            trip.erase(it);
            display_trip(trip);
            break;
        }
    } while (true);
}

// FUNCTIONS ADDED FOR LAB 28
// finds a goat based on name
// couldn't figure out how to search for a Goat object with find()
void FindGoat(const list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip to find." << "\n\n";
        return;
    }

    string name = "";

    cout << "Please enter goat name to find: ";
    getline(cin, name);

    // auto it = find(trip.begin(), trip.end(), )

    for (auto it = trip.begin(); it != trip.end(); it++)
    {
        if (it->get_name() == name)
        {
            cout << "Goat: " << it->get_name() << " FOUND!\n";
            break;
        }
    }
    cout << "\n";
}

// clears entire list of goats
void ClearGoats(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip to find." << "\n\n";
        return;
    }

    trip.clear();

    cout << "Goat trip cleared." << "\n\n";
}

// outputs average age of goat herd after using accumulate
void AverageAge(const list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    // compiler didn't recognize accumulate
    // accumulate(trip.begin(), trip.end(), [](int &sum, const Goat &goat)
    //            { sum += goat.get_age(); });

    int sum = 0;
    int count = 0;
    for (auto it = trip.begin(); it != trip.end(); it++)
    {
        sum += it->get_age();
        count++;
    }

    cout << "Average goat age: " << sum / count << "\n\n";
}

// shuffles list of goats in random order
void ShuffleGoats(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    shuffle(trip.begin(), trip.end(), default_random_engine());

    cout << "Trip after shuffle:\n";
    display_trip(trip);
}

// reversed order of goats in list
void ReverseGoatOrder(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    reverse(trip.begin(), trip.end());

    cout << "Trip after reverse:\n";
    display_trip(trip);
}

// sorts goats by name
void AnyGoatsOver10(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    bool goatOver10 = any_of(trip.begin(), trip.end(), [](Goat &goat)
                             { return goat.get_age() > 10; });
    cout << "Any goats over 10: " << (goatOver10 ? "Yes" : "No") << endl
         << endl;
}

// uses transform to deduct 1 from age
void TransformAgeMinusOne(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    transform(trip.begin(), trip.end(), trip.begin(), [](Goat &goat)
              { return goat.set_age(goat.get_age() - 1); });
    cout << "Goat age deducted by 1" << endl
         << endl;
    display_trip(trip);
}

// adds a year to each goat using for_each()
void AddOneYearForEachGoat(list<Goat> &trip)
{
    if (trip.empty())
    {
        cout << "No goats in current trip." << "\n\n";
        return;
    }

    for_each(trip.begin(), trip.end(), [](Goat &goat)
             { return goat.set_age(goat.get_age() + 1); });
    cout << "Goat age + 1 for each." << endl
         << endl;
    display_trip(trip);
}