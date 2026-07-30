#pragma once

#include "User.h"
#include <string>

using namespace std;


class UserProfileManager
{

public:

    void saveUser(const User& user);

    User loadUser();

    bool profileExists();

};