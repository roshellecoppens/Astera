#include "UserProfileManager.h"

#include <fstream>


// Saves user information into a file
void UserProfileManager::saveUser(const User& user)
{
    ofstream file("user_profile.txt");


    if (file.is_open())
    {
        file << user.name << endl;
        file << user.mood << endl;
        file << user.budget << endl;


        for (const string& goal : user.goals)
        {
            file << goal << endl;
        }


        file.close();
    }
}


// Loads saved user information
User UserProfileManager::loadUser()
{
    User user;


    ifstream file("user_profile.txt");


    if (file.is_open())
    {
        getline(file, user.name);

        getline(file, user.mood);


        string budget;

        getline(file, budget);

        user.budget = stoi(budget);



        string goal;

        while (getline(file, goal))
        {
            user.goals.push_back(goal);
        }


        file.close();
    }


    return user;
}