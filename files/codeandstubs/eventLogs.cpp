/* 
* eventLogs.cpp
* 
* 
* 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

void addEvent(string, int);
void showEvents();
void showError();


void addEvent(string s, int error) {

    time_t currentTime = time(0);
    char* localTime = ctime(&currentTime);

    ofstream file("eventLogs.txt"); 
    if (file.is_open()) {
        file << localTime << "\n" << s << "\n\n";
        file.close();
    }

    else cout << "Unable to retrieve eventLog file";
}

void showError() { //print only the errors
    //**STUB**
    //open the file, parse for errors, and print only the errors
}

void showEvents() { //print full list of events
    string line;
    ifstream file("eventLogs.txt");
    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << '\n';
        }
        file.close();
    }

    else cout << "Unable to retrieve eventLog file";
}
