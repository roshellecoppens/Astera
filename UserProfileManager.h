#pragma once

#include "User.h"
#include <string>

using namespace std;


class UserProfileManager
{

public:

    // Saves user information into a file
    void saveUser(const User& user);

    // Loads saved user information
    User loadUser();

    // Checks if a saved profile exists
    bool profileExists();

};