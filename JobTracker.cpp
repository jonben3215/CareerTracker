#include "JobTracker.hpp"
#include <fstream>
#include <ctime>
#include <iostream>

// Use the JSON library correctly
#include "json.hpp"
using json = nlohmann::json;

JobTracker::JobTracker() {}

// Function to get the current date as a string
string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    return to_string(ltm->tm_mon + 1) + "/" +
           to_string(ltm->tm_mday) + "/" +
           to_string(1900 + ltm->tm_year);
}

bool isFileEmpty(std::ifstream &file)
{
    return file.peek() == std::ifstream::traits_type::eof();
}

string toLower(const string &company)
{
    string newString = company;
    transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
    return newString;
}

// Function to check if a file exists
bool fileExists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

void JobTracker::searchByCompany(const string &companyName)
{
    bool found = false;
    string normalizeString = toLower(companyName);

    for(const auto &entry: jobTracker)
    {
        size_t atPos = entry.first.find(" @ ");

        if(atPos != string::npos)
        {
            string company = entry.first.substr(atPos + 3);
            string companyData = toLower(company);

            if(companyData == normalizeString)
            {
                found  = true;
                cout << entry.first << ": ";

                for(size_t i = 0;  i < entry.second.size(); i++)
                {
                    cout << entry.second[i].first;
                    if (i < entry.second.size() - 1)
                        cout << " -> ";
                }
                cout << endl;
            } 
        }
    }
    if (!found)
    {
        cerr << "\033[31mNo jobs found for company: " << companyName << "\033[0m" << endl;
    }
}

// Function to add a job to the tracker
void JobTracker::addJob(const string &jobTitle, const string &Company)
{
    string key = jobTitle + " @ " + Company;

    if (jobTracker.find(key) != jobTracker.end())
    {
        const auto &history = jobTracker[key];

        if (!history.empty() && history[0].first == "applied")
        {
            cout << "\033[31mYou have already applied for this job on: " << history[0].second << "\033[0m" << endl;
            return;
        }
        else
        {
            cout << "\033[31mPreviously applied, but time is unknown\033[0m" << endl;
        }
        return;
    }

    string date = getCurrentDate();

    jobTracker[key] = {{"applied", date}};

    cout << "\033[32m" << jobTitle << " @ " << Company << " has been added successfully.\033[0m" << endl;
}

// Function to update a job's stage in the tracker
void JobTracker::updateJob(const string &jobTitle, const string &Company, const string &stage)
{
    string key = jobTitle + " @ " + Company;

    if (jobTracker.find(key) == jobTracker.end())
    {
        cout << "\033[31m" << jobTitle << " @ " << Company << " is not found!\033[0m" << endl;
        return;
    }

    string date = getCurrentDate();

    jobTracker[key].emplace_back(stage, date);

    cout << "\033[32mSuccessfully Updated " << jobTitle << " @ " << Company << " to stage: " << stage << " on " << date << ".\033[0m" << endl;
}

// Function to remove a job from the tracker
void JobTracker::removeJob(const string &jobTitle, const string &Company)
{
    string key = jobTitle + " @ " + Company;

    if (jobTracker.find(key) == jobTracker.end())
    {
        cout << "\033[31m" << jobTitle << " @ " << Company << " is not found!\033[0m" << endl;
        return;
    }

    jobTracker.erase(key);
    saveToJson();

    cout << "\033[32mSuccessfully Removed " << jobTitle << " @ " << Company << "\033[0m" << endl;
}

// Function to save the job tracker to a JSON file
void JobTracker::saveToJson()
{
    string jsonFile = "my_jobs.json";
    json jsonData;

    // Load existing data if the file exists and is not empty
    if (fileExists(jsonFile))
    {
        ifstream input(jsonFile);

        if (input.is_open())
        {
            if (input.peek() != ifstream::traits_type::eof())
            {
                try
                {
                    input >> jsonData;
                }
                catch (const exception &e)
                {
                    cerr << "Error reading JSON file: " << e.what() << endl;
                }
            }
            input.close();
        }
    }

    // Use a temporary vector to sort the jobs
    vector<pair<string, vector<pair<string, string>>>> sortedJobs(jobTracker.begin(), jobTracker.end());

    // Sort by the most recent job (reverse order of insertion in jobTracker)
    sort(sortedJobs.begin(), sortedJobs.end(), [](const auto &a, const auto &b) {
        return a.second.front().second > b.second.front().second; // Compare based on the most recent date
    });

    // Clear the JSON object before re-populating
    jsonData.clear();

    // Add sorted jobs to the JSON object
    for (const auto &entry : sortedJobs)
    {
        jsonData[entry.first] = json::array();
        for (const auto &status : entry.second)
        {
            jsonData[entry.first].push_back({status.first, status.second});
        }
    }

    // Write back to the file
    ofstream output(jsonFile);

    if (output.is_open())
    {
        output << jsonData.dump(4); // Pretty-print with 4-space indentation
        output.close();
        cout << "\033[32mData written to: " << jsonFile << "\033[0m"<< endl;
    }
    else
    {
        cerr << "\033[31mFailed to write to " << jsonFile << "!\033[0m" << endl;
    }
}


void JobTracker::loadFromJson(const string & filename)
{
    ifstream input(filename);
    json JsonData;

    if(isFileEmpty(input))
    {
        return;
    }

    if(!input.is_open())
    {
        cerr << "\033[31mError Opening JSON file: " << filename << "\033[0m" << endl;
        return;
    }
    try
    {
        input >> JsonData;
        
        for(auto it = JsonData.begin(); it != JsonData.end(); it++)
        {
            string key =it.key();
            vector<pair<string, string>> history; 

            for(auto & entry: it.value())
            {
                if (entry.is_array() && entry.size() == 2)
                {
                    string stage = entry[0].get<string>();
                    string date = entry[1].get<string>();
                    history.emplace_back(stage, date);
                }
            }
            jobTracker[key] = history;
        }
        cout << "\033[32mData loaded successfully from: " << filename << "\033[0m" << endl;
    }
    catch(const std::exception& e)
    {
         cerr << "\033[31mError reading or parsing JSON file: " << e.what() << "\033[0m" << endl;
    }
}

void JobTracker::jobReport() 
{
    int num = 1;
    for(const auto &entry: jobTracker)
    {
        cout <<"\033[32m"<< num <<"\033[0m";
        cout<< ": "<< entry.first << ": ";
        for (size_t i = 0; i < entry.second.size(); ++i)
        {
           cout<< entry.second[i].first;

            if (i < entry.second.size() - 1)
            {
                cout << " -> ";
            }
            
        }
        num++;
        cout << endl;
    }
    totalJob = num;
}