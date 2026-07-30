#include "ActivityDatabase.h"


vector<Activity> ActivityDatabase::getActivities()
{
    vector<Activity> activities;


    Activity museum;

    museum.name = "Visit a museum";
    museum.mood = "curious";
    museum.cost = 0;
    museum.category = "Learning";
    museum.duration = 120;

    museum.develops.push_back("Knowledge");
    museum.develops.push_back("Curiosity");
    museum.develops.push_back("Creativity");


    activities.push_back(museum);



    Activity library;

    library.name = "Visit a library";
    library.mood = "studious";
    library.cost = 0;
    library.category = "Learning";
    library.duration = 90;

    library.develops.push_back("Research Skills");
    library.develops.push_back("Patience");
    library.develops.push_back("Curiosity");


    activities.push_back(library);



    Activity eveningRun;

    eveningRun.name = "Go for an evening run";
    eveningRun.mood = "productive";
    eveningRun.cost = 0;
    eveningRun.category = "Fitness";
    eveningRun.duration = 35;

    eveningRun.develops.push_back("Discipline");
    eveningRun.develops.push_back("Resilience");
    eveningRun.develops.push_back("Clarity");


    activities.push_back(eveningRun);



    Activity salsaClass;

    salsaClass.name = "Beginner salsa class";
    salsaClass.mood = "fun";
    salsaClass.cost = 8;
    salsaClass.category = "Dance";
    salsaClass.duration = 120;

    salsaClass.develops.push_back("Coordination");
    salsaClass.develops.push_back("Confidence");
    salsaClass.develops.push_back("Rhythm");


    activities.push_back(salsaClass);



    return activities;
}