#pragma once

#include <string>
#include <vector>

using namespace std;

// Represents the person Astera is helping
class User
{
public:

    // Basic information
    string name;

    // How the user wants today to feel
    string mood;

    // Maximum amount they want to spend
    int budget;

    // Personal qualities the user wants to develop
    vector<string> goals;
};