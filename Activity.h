#pragma once

#include <string>
#include <vector>

using namespace std;

struct Activity
{
    string name;
    string mood;
    int cost;
    string category;
    int duration;

    string preferredTime;

    vector<string> develops;
};