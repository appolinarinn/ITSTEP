#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

string getCurrentTime() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &ltm);

    return string(buffer);
}

int main() {
    string nickname;
    cout << "Enter your nickname: ";
    getline(cin, nickname);

    ofstream outfile("chat_log.txt", ios::app);

    cout << "Enter lines (type 'exit' to stop):" << endl;

    string input;
    while (true) {
        getline(cin, input);
        if (input == "exit") break;

        string timestamp = getCurrentTime();
        outfile << nickname << " | " << timestamp << " | " << input << endl;
    }

    outfile.close();

    cout << "\n📜 All messages in the table:\n" << endl;

    ifstream infile("chat_log.txt");
    string line;
    while (getline(infile, line)) {
        cout << line << endl;
    }

    infile.close();

    return 0;
}
