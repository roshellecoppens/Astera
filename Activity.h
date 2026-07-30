#pragma once

#include <string>
#include <vector>


class Activity
{
public:

    std::string name;
    std::string mood;
    int cost;
    std::string category;

    std::vector<std::string> develops;

    // Estimated time required in minutes
    int duration;
};