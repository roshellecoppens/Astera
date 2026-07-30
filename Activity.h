#pragma once

#include <string>
#include <vector>


class Activity
{
public:
    Activity() noexcept : cost(0.0), duration(0) { }

    std::string name;
    std::string mood;
    double cost;
    std::string category;

    std::vector<std::string> develops;

    // Estimated time required in minutes
    int duration;
};