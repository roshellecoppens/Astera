#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "User.h"
#include "UserProfileManager.h"
#include "Activity.h"
#include "ActivityDatabase.h"
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


// Asks the user for personal qualities they want to develop
vector<string> askGoals()
{
    vector<string> goals;

    string goal;


    cout << "\nWhat qualities would you like to develop?\n";
    cout << "Example: confidence, curiosity, discipline\n";
    cout << "Type 'done' when finished.\n\n";


    while (true)
    {
        cout << "Enter a goal: ";
        cin >> goal;


        if (toLower(goal) == "done")
        {
            break;
        }


        goals.push_back(goal);
    }


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

// Generates unique schedule times
// Prevents multiple activities being assigned to the same time
string generateTime(int index)
{
    vector<string> availableTimes =
    {
        "10:00",
        "13:00",
        "16:00",
        "18:30",
        "19:00"
    };


    if (index < availableTimes.size())
    {
        return availableTimes[index];
    }

    return "20:00";
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

// Selects the highest scoring activities for the user's day
// Prevents Astera from overwhelming the user with too many choices
vector<ScoredActivity> getTopActivities(
    const vector<ScoredActivity>& rankedActivities,
    int amount)
{
    vector<ScoredActivity> topActivities;


    for (int i = 0; i < amount && i < rankedActivities.size(); i++)
    {
        topActivities.push_back(rankedActivities[i]);
    }


    return topActivities;
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

        item.time = generateTime(plan.size());

        plan.push_back(item);
    }


    sort(plan.begin(), plan.end(),
        [](const ScheduleItem& a, const ScheduleItem& b)
        {
            return a.time < b.time;
        });


    return plan;
}


// Displays a menu for the user if it exists, and asks them to choose how to proceed with their profile
int profileMenu()
{
    int choice;

    cout << "\nWhat would you like to do?\n";

    cout << "1. Continue with saved profile\n";
    cout << "2. Update profile\n";
    cout << "3. Create new profile\n\n";

    cout << "Choice: ";

    cin >> choice;

    return choice;
}


User updateProfile(User user)
{
    cout << "\nUpdating profile...\n\n";

    cout << "New mood:\n";
    cin >> user.mood;

    cout << "New budget:\n";
    cin >> user.budget;

    user.goals = askGoals();

    return user;
}



int main()
{
    welcomeUser();


    User user;

    UserProfileManager profileManager;


    if (profileManager.profileExists())
    {
        cout << "Saved profile found.\n\n";

        user = profileManager.loadUser();


        cout << "Welcome back "
            << user.name
            << "!\n";


        int choice = profileMenu();


        if (choice == 1)
        {
            cout << "\nContinuing with saved profile...\n";
        }


        else if (choice == 2)
        {
            user = updateProfile(user);

            profileManager.saveUser(user);

            cout << "\nProfile updated successfully.\n";
        }


        else if (choice == 3)
        {
            cout << "\nCreating new profile...\n\n";


            user.name = askName();
            user.mood = askMood();
            user.budget = askBudget();
            user.goals = askGoals();


            profileManager.saveUser(user);


            cout << "\nNew profile saved successfully.\n";
        }


        else
        {
            cout << "\nInvalid choice. Continuing with saved profile.\n";
        }
    }


    else
    {
        cout << "No saved profile found.\n";
        cout << "Creating new profile...\n\n";


        user.name = askName();
        user.mood = askMood();
        user.budget = askBudget();
        user.goals = askGoals();


        profileManager.saveUser(user);


        cout << "\nProfile saved successfully.\n";
    }



    // Loads the activity database and retrieves all activities
    ActivityDatabase database;

    vector<Activity> activities = database.getActivities();



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



    vector<ScoredActivity> topActivities = getTopActivities(ranked, 3);

    vector<ScheduleItem> dailyPlan = createRankedPlan(topActivities);



    displayPlan(dailyPlan);



    return 0;
}