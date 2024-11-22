#include "JobTracker.hpp"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <limits>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


void clearTerminal()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Unix/Linux/Mac
#endif
}

void menue()
{
    cout << "############ MAIN MENUE ############" << endl;
    cout << "1. Add Job" << endl;
    cout << "2. Update Application" << endl;
    cout << "3. Search Job" << endl;
    cout << "4. Job report" << endl;
    cout << "5. Remove a Job" << endl;
    cout << "6. Exit" << endl;
}

int main()
{
    JobTracker jt;
    int choice = 0;
    string prcessStep = "";
    string jobtitle = "";
    string CompanyName = "";
    string stage = "";
    string exitCode = "";
    bool end = false;

    jt.loadFromJson("my_jobs.json");

    while (!end)
    {
        clearTerminal();
        menue();
        cout << "Select From Menue: ";
        cin >> choice;
        cout << endl;
        clearTerminal();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice)
        {
        case 1:
        {
            cout << "######### ADD JOB #########" << endl;
            cout << "\033[33mType 'quit' anytime to go back to main menue.\033[0m" << endl;
            sleep_for(seconds(5));
            while (true)
            {
               
                // Prompt for job title
                cout << "\033[36mEnter Job Position: \033[0m";
                getline(cin, jobtitle); // No cin.ignore() here
                if (jobtitle == "quit")
                    break;

                // Prompt for company name
                cout << "\033[36mEnter Company Name: \033[0m";
                getline(cin, CompanyName); // No cin.ignore() here
                if (CompanyName == "quit")
                    break;

                jt.addJob(jobtitle, CompanyName); // Add the job
                cout << endl;
            }

            break;
        }
        case 2:
        {
            cout << "######### UPDATE APPLICATION #########" << endl;
            cout << "\033[33mType 'quit' anytime to go back to main menue.\033[0m" << endl;
            sleep_for(chrono::seconds(5));

            while (true) // Infinite loop for repeated updates
            {
                cout << "\033[36mEnter Job Position: \033[0m";
                getline(cin, jobtitle);
                if (jobtitle == "quit") // Check if the user wants to quit
                    break;

                cout << "\033[36mEnter Company Name: \033[0m";
                getline(cin, CompanyName);
                if (CompanyName == "quit") // Check if the user wants to quit
                    break;

                cout << "\033[36mWhat Stage of Interview are you on:\033[0m ";
                getline(cin, stage);
                if (stage == "quit") // Check if the user wants to quit
                    break;

                jt.updateJob(jobtitle, CompanyName, stage); // Update the job
                cout << endl;
            }
            break;
        }
         case 3:
        {
            cout << "######### SEARCH JOB #########" << endl;
            cout << "\033[33mType 'quit' anytime to go back to main menue.\033[0m" << endl;
            sleep_for(seconds(5));

            while (true) // Infinite loop to repeatedly prompt for input
            {
                cout << "\033[36mEnter Company Name: \033[0m";
                getline(cin, CompanyName);
                if (CompanyName == "quit") // Check if the user wants to quit
                    break;

                jt.searchByCompany(CompanyName); // Remove the job
                cout << endl;
            }
            break;
        }
        case 4:
        {
            cout << "YOUR JOB REPORT:" << endl;
            jt.jobReport();
            cout << endl;
            sleep_for(seconds(10));
            break;
        }
        case 5:
        {

            cout << "######### REMOVE A JOB #########" << endl;
            cout << "\033[33mType 'quit' anytime to go back to main menue.\033[0m" << endl;
            sleep_for(seconds(5));

            while (true) // Infinite loop to repeatedly prompt for input
            {
                cout << "\033[36mEnter Job Position: \033[0m";
                getline(cin, jobtitle);
                if (jobtitle == "quit") // Check if the user wants to quit
                    break;

                cout << "\033[36mEnter Company Name: \033[0m";
                getline(cin, CompanyName);
                if (CompanyName == "quit") // Check if the user wants to quit
                    break;

                jt.removeJob(jobtitle, CompanyName); // Remove the job
                cout << endl;
            }

            break;
        }
        default:
            end = true;
            jt.saveToJson();
            sleep_for(seconds(5));
        }
    }
    clearTerminal();
}