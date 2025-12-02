#pragma once

#include <nlohmann/json.hpp>
#include <mutex>
#include <string>

using json = nlohmann::json;
using namespace std;

class Database {
private:
    json data;
    mutex mtx;
    string filename;

    void save();

public:
    Database(const string& file);

    bool userExists(const string& user_id);
    string getUsername(const string& user_id);
    json getUserData(const string& user_id);
    void registerUser(const string& user_id, const string& username);
};