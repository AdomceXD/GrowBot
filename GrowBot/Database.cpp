#include "Database.hpp"
#include <iostream>
#include <fstream>

void Database::save() {
    try {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            cout << "Database saved successfully." << endl;
        } else {
            cerr << "Error: Could not open file for writing!" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error saving database: " << e.what() << endl;
    }
}

Database::Database(const string& file) : filename(file) {
    try {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            inFile >> data;
            inFile.close();
            cout << "Database loaded successfully." << endl;
        } else {
            cout << "No existing database found, creating new one." << endl;
            data = json::object();
        }
    } catch (const exception& e) {
        cerr << "Error loading database: " << e.what() << endl;
        data = json::object();
    }
}

bool Database::userExists(const string& user_id) {
    lock_guard<mutex> lock(mtx);
    return data.contains(user_id);
}

string Database::getUsername(const string& user_id) {
    lock_guard<mutex> lock(mtx);
    if (data.contains(user_id) && data[user_id].contains("username")) {
        return data[user_id]["username"].get<string>();
    }
    return "";
}

json Database::getUserData(const string& user_id) {
    lock_guard<mutex> lock(mtx);
    if (data.contains(user_id)) {
        return data[user_id];
    }
    return json::object();
}

void Database::registerUser(const string& user_id, const string& username) {
    lock_guard<mutex> lock(mtx);
    data[user_id] = {
        {"username", username},
        {"id", user_id},
        {"level", 1},
        {"xp", 0},
        {"xpneeded", 50},
        {"wls", 0},
        {"growtokens", 0},
        {"gems", 0},
        {"farmable", "Grass"}
    };
    save();
}