#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "User.h"
#include "Activity.h"
#include "Trait.h"
#include "ScheduleItem.h"

using namespace std;


// Converts text into lowercase so Astera can compare words consistently
string toLower(string text)
{
    transform(text.begin(), text.end(), text.begin(), ::tolower);

    return text;
}


// Displays Astera's introduction
void welcomeUser()
{
    cout << "=====================================\n";
    cout << "         Welcome to Astera\n";
    cout << "=====================================\n\n";
}


// Displays the user's planned day
// Shows when activities happen and why they were chosen
void displayPlan(const vector<ScheduleItem>& plan)
{
    cout << "\n=====================================\n";
    cout << "          Your Astera Day\n";
    cout << "=====================================\n\n";


    for (const ScheduleItem& item : plan)
    {
        cout << item.time << "\n";
        cout << "- " << item.activity.name << "\n";
        cout << "  Category: " << item.activity.category << "\n";

        cout << "  Duration: "
            << item.activity.duration
            << " minutes\n";


        cout << "  Helps develop:\n";

        for (const string& trait : item.activity.develops)
        {
            cout << "   - " << trait << "\n";
        }

        cout << "\n";
    }
}


// Collects the user's name
string askName()
{
    string name;

    cout << "What is your name?\n";
    cin >> name;

    return name;
}


// Collects the user's desired mood
string askMood()
{
    string mood;

    cout << "How do you want your day to feel today?\n";
    cin >> mood;

    return mood;
}


// Collects the user's spending limit
int askBudget()
{
    int budget;

    cout << "How much money would you like to spend today?\n";
    cin >> budget;

    return budget;
}


// Collects personal qualities the user wants to develop
vector<string> askGoals()
{
    vector<string> goals;

    string goal;

    cout << "\nWhat qualities would you like to develop?\n";
    cout << "Example: confidence, curiosity, discipline\n";
    cout << "Enter one goal:\n";

    cin >> goal;

    goals.push_back(goal);

    return goals;
}


// Checks whether an activity supports the user's goals
bool matchesGoal(const Activity& activity, const User& user)
{
    for (const string& goal : user.goals)
    {
        for (const string& trait : activity.develops)
        {
            if (toLower(goal) == toLower(trait))
            {
                return true;
            }
        }
    }

    return false;
}


// Calculates how suitable an activity is
// Higher scores mean a better recommendation
int calculateScore(const Activity& activity, const User& user)
{
    int score = 0;


    // Mood compatibility
    if (activity.mood == user.mood)
    {
        score += 1;
    }


    // Budget compatibility
    if (activity.cost <= user.budget)
    {
        score += 1;
    }


    // Growth goal compatibility
    for (const string& goal : user.goals)
    {
        for (const string& trait : activity.develops)
        {
            if (toLower(goal) == toLower(trait))
            {
                score += 3;
            }
        }
    }


    return score;
}


// Finds the highest scoring activity
void recommendActivities(const vector<Activity>& activities, const User& user)
{
    cout << "\nAstera's recommendation:\n\n";


    int bestScore = 0;
    Activity bestActivity;


    for (const Activity& activity : activities)
    {
        int score = calculateScore(activity, user);


        if (score > bestScore)
        {
            bestScore = score;
            bestActivity = activity;
        }
    }


    if (bestScore > 0)
    {
        cout << "- " << bestActivity.name << "\n";
        cout << "  Category: " << bestActivity.category << "\n";
        cout << "  Cost: GBP " << bestActivity.cost << "\n";
        cout << "  Match Score: " << bestScore << "\n\n";


        cout << "  Helps develop:\n";

        for (const string& trait : bestActivity.develops)
        {
            cout << "   - " << trait << "\n";
        }
    }
    else
    {
        cout << "Astera could not find a suitable recommendation today.\n";
    }
}

// Assigns a suitable time of day based on activity type
string chooseTime(const Activity& activity)
{
    if (activity.category == "Fitness")
    {
        return "18:30";
    }

    else if (activity.category == "Learning")
    {
        return "10:00";
    }

    else if (activity.category == "Dance")
    {
        return "19:00";
    }

    else
    {
        return "11:00";
    }
}

// Creates a simple day plan
vector<ScheduleItem> createDailyPlan(const vector<Activity>& activities, const User& user)
{
    vector<ScheduleItem> plan;


    for (const Activity& activity : activities)
    {
        int score = calculateScore(activity, user);


        if (score > 0)
        {
            ScheduleItem item;

            item.activity = activity;
            item.time = chooseTime(activity);

            plan.push_back(item);
        }
    }

    sort(plan.begin(), plan.end(),
        [](const ScheduleItem& a, const ScheduleItem& b)
        {
            return a.time < b.time;
        });


    return plan;
}

// Stores an activity together with its recommendation score
// Allows Astera to rank activities from most suitable to least suitable
struct ScoredActivity
{
    Activity activity;
    int score;
};

// Calculates scores for all activities and ranks them from most suitable to least suitable
vector<ScoredActivity> rankActivities(
    const vector<Activity>& activities,
    const User& user)
{
        vector<ScoredActivity> rankedActivities;

        for (const Activity& activity : activities)
        {
            int score = calculateScore(activity, user);

            if (score > 0)
            {
                ScoredActivity scored;

                scored.activity = activity;
                scored.score = score;

                rankedActivities.push_back(scored);
            }
        }

        sort(rankedActivities.begin(), rankedActivities.end(),
            [](const ScoredActivity & a, const ScoredActivity & b)
        {
            return a.score > b.score;
        });
            
        return rankedActivities;
}

// Creates a daily plan using ranked activities
// Sorts the final schedule into chronological order
vector<ScheduleItem> createRankedPlan(
    const vector<ScoredActivity>& rankedActivities)
{
    vector<ScheduleItem> plan;


    for (const ScoredActivity& scored : rankedActivities)
    {
        ScheduleItem item;

        item.activity = scored.activity;
        item.time = chooseTime(scored.activity);

        plan.push_back(item);
    }


    sort(plan.begin(), plan.end(),
        [](const ScheduleItem& a, const ScheduleItem& b)
        {
            return a.time < b.time;
        });


    return plan;
}


int main()
{
    welcomeUser();


    User user;

    user.name = askName();
    user.mood = askMood();
    user.budget = askBudget();
    user.goals = askGoals();



    Activity museum;

    museum.name = "Visit a museum";
    museum.mood = "curious";
    museum.cost = 0;
    museum.category = "Learning";
    museum.duration = 120;

    museum.develops.push_back("Knowledge");
    museum.develops.push_back("Curiosity");
    museum.develops.push_back("Creativity");



    Activity library;

    library.name = "Visit a library";
    library.mood = "studious";
    library.cost = 0;
    library.category = "Learning";
    library.duration = 90;

    library.develops.push_back("Research Skills");
    library.develops.push_back("Patience");
    library.develops.push_back("Curiosity");



    Activity eveningRun;

    eveningRun.name = "Go for an evening run";
    eveningRun.mood = "productive";
    eveningRun.cost = 0;
    eveningRun.category = "Fitness";
    eveningRun.duration = 35;

    eveningRun.develops.push_back("Discipline");
    eveningRun.develops.push_back("Resilience");
    eveningRun.develops.push_back("Clarity");



    Activity salsaClass;

    salsaClass.name = "Beginner salsa class";
    salsaClass.mood = "fun";
    salsaClass.cost = 8;
    salsaClass.category = "Dance";
    salsaClass.duration = 120;

    salsaClass.develops.push_back("Coordination");
    salsaClass.develops.push_back("Confidence");
    salsaClass.develops.push_back("Rhythm");



    vector<Activity> activities;

    activities.push_back(museum);
    activities.push_back(library);
    activities.push_back(eveningRun);
    activities.push_back(salsaClass);


    // Ranks activities based on scores calculated from the user's preferences
    vector<ScoredActivity> ranked = rankActivities(activities, user);


    cout << "\nAstera Ranking:\n\n";


    for (const ScoredActivity& item : ranked)
    {
        cout << item.activity.name
            << " - Score: "
            << item.score
            << "\n";
    }


    recommendActivities(activities, user);

    vector<ScheduleItem> dailyPlan = createRankedPlan(ranked);

    displayPlan(dailyPlan);


    return 0;
}