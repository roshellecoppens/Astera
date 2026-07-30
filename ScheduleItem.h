#pragma once

#include <string>
#include "Activity.h"


class ScheduleItem
{
public:

    // Time the activity happens
    string time;

    // The planned activity
    Activity activity;

    // Why this time was chosen
    string partOfDay;

};