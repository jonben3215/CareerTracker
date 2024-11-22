#ifndef JOBTRACKER_HPP
#define JOBTRACKER_HPP

#include <iostream>
#include "json.hpp" //For Loading from JSON
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
using json = nlohmann::json;


class JobTracker{

    private:
        unordered_map<string, vector<pair<string, string>>> jobTracker;
        int totalJob = 0;


    public:
        JobTracker();
        void addJob(const string & jobTitle, const string &company);
        void searchByCompany(const string &company);
        void updateJob(const string &jobTitle, const string &company, const string&stage);
        void removeJob(const string & jobTitle, const string &company);
        void jobReport();
        void loadFromJson(const string & filename);
        void saveToJson();
};
#endif